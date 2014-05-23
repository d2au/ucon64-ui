/*
  uCON64 frontend, a graphical frontend for the ROM tool uCON64.
  Copyright (c) 2004, 2005 dbjh

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <stdio.h>
#include <stdlib.h>
#ifdef  __unix__
#include <unistd.h>
#endif
#include <fx.h>
#include "OutputChecker.h"
#include "Process.h"
#ifdef  FORK_IPC
#include "ProcessIPC.h"
#endif
#include "MainWindow.h"


FXDEFMAP(OutputChecker) OutputCheckerMap[] =
{
  FXMAPFUNC(SEL_CHORE, OutputChecker::ID_CHECK, OutputChecker::check),
  FXMAPFUNC(SEL_IO_READ, OutputChecker::ID_CHECK, OutputChecker::check)
};

FXIMPLEMENT(OutputChecker, FXObject, OutputCheckerMap, ARRAYNUMBER(OutputCheckerMap))


#ifndef FORK_IPC
OutputChecker::OutputChecker(Process *a_process,
#else
OutputChecker::OutputChecker(ProcessIPC *a_process,
#endif
    FXText *a_output, ProgressDialog *a_progressDialog)
{
  m_process = a_process;
  m_output = a_output;
  m_progressDialog = a_progressDialog;
  m_checkState = 0;
  m_startOfLine = 0;
  m_column = 0;
  m_lineLen = 0;
}


FXint
OutputChecker::run()
{
  while (1)
    {
      check(NULL, 0, NULL);
      if (!m_checkState)
        break;
    }
  return 1;
}


int
OutputChecker::fixEndOfLine(char *a_line, int a_length)
// overwrite carriage return (not handled nicely by FOX for FXText)
{
  if (a_length >= 2 && a_line[a_length - 2] == '\r') // line ends with \r\n
    {
      a_length--;
      a_line[a_length - 1] = '\n';
      a_line[a_length] = 0;
    }
  else if (a_length >= 1 && a_line[a_length - 1] == '\r') // line ends with \r
    {
      a_length--;
      a_line[a_length] = 0;
    }

  return a_length;
}


void
OutputChecker::addOutput(char *a_buf, int a_size)
/*
  This function is needed in order to handle uCON64 output that doesn't end with
  a newline, for example a progress gauge. By default, CommandLineHandler adds
  --frontend to the command line, so usually it's only needed for --xcmct=2. It
  is also needed for the gauge when creating or updating index files (--frontend
  doesn't change anything about that).
*/
{
  int n = 0, sizeLeft = a_size, strLen = -1, l;

  while (n < sizeLeft)
    {
      if (a_buf[n] == '\n' || a_buf[n] == '\r')
        {
          strLen = n;
          if (a_buf[n] == '\n')
            {
              if (m_column == 0 && strLen == 0) // handle DOS newline
                {
                  m_output->appendText(&a_buf[n], 1);
                  m_lineLen++;
                }
              else
                strLen++;
            }

          if (m_column + strLen > m_lineLen)
            {
              l = 0;
              if (m_column < m_lineLen)
                {
                  l = m_lineLen - m_column;
                  m_output->replaceText(m_startOfLine + m_column, l, a_buf, l);
                }
              m_output->appendText(a_buf + l, strLen - l);
              m_lineLen = strLen;
              m_column = strLen;
            }
          else if (strLen > 0)
            {
              m_output->replaceText(m_startOfLine + m_column, strLen, a_buf, strLen);
              m_column += strLen;
            }

          if (a_buf[n] == '\n')
            {
              m_startOfLine += m_lineLen;
              m_lineLen = 0;
            }
          m_column = 0;

          a_buf += n + 1;
          sizeLeft -= n + 1;
          n = -1;
        }
      n++;
    }

  if (strLen == -1)                             // no \r or \n? (happens with "fake pipe")
    {
      strLen = a_size;
      if (m_column + strLen > m_lineLen)
        {
          l = 0;
          if (m_column < m_lineLen)
            {
              l = m_lineLen - m_column;
              m_output->replaceText(m_startOfLine + m_column, l, a_buf, l);
            }
          m_output->appendText(a_buf + l, strLen - l);
          m_lineLen = strLen;
          m_column = strLen;
        }
      else if (strLen > 0)
        {
          m_output->replaceText(m_startOfLine + m_column, strLen, a_buf, strLen);
          m_column += strLen;
        }
    }
//  fputc('.', stdout); fflush(stdout);
}


#define BUFSIZE 160 //81
long
OutputChecker::check(FXObject *, FXSelector a_selector, void *)
{
  char *endptr, buf[BUFSIZE];
  int progress = 0, n = 0, m;
  MainWindow *mainWindow = (MainWindow *) m_progressDialog->getOwner();

  switch (m_checkState)
    {
    case 0:
      m_checkState++;                           // falling through
    case 1:
#if 0 // TODO: Find a more reliable method to handle this problem (this happens regularly with a correct exe)
      if (!m_process->running())
        {
          m_output->appendText("\nERROR: Could not start uCON64", 30);
          FXMessageBox::error(mainWindow, MBOX_OK, "Could not start uCON64",
            "Please specify a different uCON64 executable in the options window");
          m_progressDialog->handle(this, FXSEL(SEL_COMMAND, ProgressDialog::ID_CANCEL), NULL);
          m_checkState++;
        }
#endif
      m_checkState++;
      break;
    case 2:
      if (m_process->outputAvailable())
        {
          m_output->setText("", 0);             // clear output frame
          m_checkState++;
        }
      break;
    case 3:
      if (m_progressDialog->cancelled())
        {
//          char c = 'q';
//          m_process->writeStdin((unsigned char *) &c, 1);
#ifdef  USE_THREAD // && defined FORK_IPC, perhaps && !defined USE_SIGNAL
          if (m_progressDialog->shown())
            m_progressDialog->hide();
#endif
          mainWindow->disableAbortButton();
          m_process->abort();
          m_checkState = 0;
          break;
        }

      if (m_process->outputAvailable() & 1)
        {
          n = m_process->readStdout((unsigned char *) buf, BUFSIZE);
          if (n)
            {
              m = strtol(buf, &endptr, 10);
              // "endptr - buf <= 3" is necessary if the internal names consist
              //  of numeric characters only
              if ((*endptr == '\n' || *endptr == '\r') &&
                  endptr > buf && endptr - buf <= 3)
                {
                  if (*endptr == '\r')          // get newline too under Windows
                    m_process->readStdout((unsigned char *) buf, 2);
                  progress = m;
                  if (!m_progressDialog->shown())
                    {
                      m_progressDialog->show();
                      m_progressDialog->setFocus(); // "necessary" under Windows
                    }
                  m_progressDialog->setProgress(progress);
                }
              else
                {
                  addOutput(buf, n);
                  if (n <= 2)
                    {
                      m_output->makePositionVisible(m_output->getLength());
                      m_output->update();
                    }
                }
            }
        }
      if (!(m_process->running() && (progress ? progress < 100 : 1)))
        m_checkState++;
      break;
    // At this point the process isn't running and/or progress is 100%
    case 4:
#if 0
      if (m_progressDialog->cancelled())
        {
          m_process->abort();
          m_checkState = 0;
          break;
        }

      if (m_process->outputAvailable() & 2)
        {
          n = m_process->readStderr((unsigned char *) buf, BUFSIZE);
          if (n)
            {
              n = fixEndOfLine(buf, n);
//              FXColor color = m_output->getTextColor();
//              m_output->setTextColor(FXRGB(255, 0, 0));
              m_output->appendText(buf, n);//, true);
//              m_output->setTextColor(color);
            }
        }
      if (!(m_process->running() && (progress ? progress < 100 : 1)))
        m_checkState++;
      break;
#else
      m_checkState++;                           // falling through...
#endif
    case 5:
      // Make the progress bar reach 100% in case we missed some output
      if (!m_process->running() && progress && !m_progressDialog->cancelled())
        {
          progress = 100;
          m_progressDialog->setProgress(progress);
        }
      m_checkState++;
      break;
    case 6:
      // Handle output that hasn't been read yet (process has ended or nearly so)
      n = 0;
      if (m_process->outputAvailable() & 1)
        {
          n = m_process->readStdout((unsigned char *) buf, BUFSIZE);
          if (n)
            {
              m = strtol(buf, &endptr, 10);
              if ((*endptr == '\n' || *endptr == '\r') &&
                  endptr > buf && endptr - buf <= 3)
                {
                  if (*endptr == '\r')
                    m_process->readStdout((unsigned char *) buf, 2);
                  progress = m;
                  m_progressDialog->setProgress(progress);
                }
              else
                addOutput(buf, n);
            }
        }
      if (!n)
        m_checkState++;
      break;
    case 7:
      if (m_process->outputAvailable() & 2)
        if ((n = m_process->readStderr((unsigned char *) buf, BUFSIZE)))
          {
            n = fixEndOfLine(buf, n);
            m_output->appendText(buf, n);
          }
      if (!n)
        m_checkState++;
      break;
    case 8:
      if (!m_process->running())
        {
          if (m_progressDialog->shown())
            m_progressDialog->hide();
          mainWindow->disableAbortButton();
          m_checkState = 0;
        }
      else
        {
          m_progressDialog->reset();
          m_checkState = 3;
        }
      m_output->makePositionVisible(m_output->getLength());
      m_output->update();
      break;
    }

  int selType = FXSELTYPE(a_selector);
  if (selType == SEL_CHORE)
    {
      if (m_checkState)
        m_output->getApp()->addChore(this, ID_CHECK);
    }
  else if (selType == SEL_IO_READ)
    if (!m_checkState)
      {
        m_output->getApp()->removeInput((FXInputHandle) m_process->getStdoutHandle(),
          INPUT_READ);
        m_output->getApp()->removeInput((FXInputHandle) m_process->getStderrHandle(),
          INPUT_READ);
      }

  if (!m_checkState)
    m_output->getApp()->endWaitCursor();

  return 1;
}
