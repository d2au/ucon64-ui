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
#include <string.h>
#include <fx.h>
#include "MainWindow.h"
#include "Process.h"
#ifdef  FORK_IPC
#include "ProcessIPC.h"
#endif
#include "OutputChecker.h"
#include "ProgressDialog.h"
#include "CommandLineHandler.h"
#include "OptionsWindow.h"
#include "TextFieldOption.h"
#include "FileDialog.h"

#ifndef USE_THREAD
#define USE_CHORE
#endif


FXDEFMAP(CommandLineHandler) CommandLineHandlerMap[] =
{
  FXMAPFUNC(SEL_COMMAND, CommandLineHandler::ID_ABORT, CommandLineHandler::onAbort)
};

FXIMPLEMENT(CommandLineHandler, FXObject, CommandLineHandlerMap, ARRAYNUMBER(CommandLineHandlerMap))


CommandLineHandler::CommandLineHandler(MainWindow *a_mainWindow)
{
  m_mainWindow = a_mainWindow;
  m_progressDialog = new ProgressDialog(m_mainWindow, "Please wait...",
    DECOR_TITLE | DECOR_BORDER, 300, 200);
  m_outputChecker = NULL;
  m_process = NULL;
  m_options = NULL;
  m_nOptions = 0;
  m_environment = NULL;
  m_fileNeeded = false;
  m_selectedFilenames = NULL;
}


CommandLineHandler::~CommandLineHandler()
{
  int n;

  if (m_options)
    for (n = 0; n < m_nOptions; n++)
      free(m_options[n]);
  if (m_environment)
    for (n = 0; m_environment[n]; n++)
      free(m_environment[n]);
  if (m_process)
    {
      if (m_outputChecker)
        {
#ifdef  USE_THREAD
          m_outputChecker->cancel();
#else
#ifdef  USE_CHORE
          if (m_mainWindow->getApp()->hasChore(m_outputChecker, OutputChecker::ID_CHECK))
            m_mainWindow->getApp()->removeChore(m_outputChecker, OutputChecker::ID_CHECK);
#else
          m_mainWindow->getApp()->removeInput((FXInputHandle) m_process->getStdoutHandle(),
            INPUT_READ);
          m_mainWindow->getApp()->removeInput((FXInputHandle) m_process->getStderrHandle(),
            INPUT_READ);
#endif
#endif // USE_THREAD
          delete m_outputChecker;
        }
      delete m_process;
    }
}


void
CommandLineHandler::setOption(char *a_option, bool a_fileNeeded)
// a call to setOption() marks a new run of uCON64
{
  int n;

  for (n = 0; n < m_nOptions; n++)
    {
      free(m_options[n]);
      m_options[n] = NULL;
    }
  if (m_environment)
    for (n = 0; m_environment[n]; n++)
      {
        free(m_environment[n]);
        m_environment[n] = NULL;
      }

  m_nOptions = 2;                               // <uCON64 exe> <a_option>
  if ((m_options = (char **) realloc(m_options, m_nOptions * sizeof (char *))) == NULL)
    {
      fprintf(stderr, "ERROR: Reallocation failed\n");
      exit(1);
    }
  // The first time this method is called n equals 0
  if (n == 0)
    memset(m_options, 0, m_nOptions * sizeof (char *));

  if (a_option[0])                              // empty string?
    {
      if ((m_options[m_nOptions - 1] = (char *) malloc(strlen(a_option) + 1)) == NULL)
        {
          fprintf(stderr, "ERROR: Not enough memory\n");
          exit(1);
        }
      strcpy(m_options[m_nOptions - 1], a_option);
    }
  else
    m_nOptions--;
  m_fileNeeded = a_fileNeeded;
  m_addFilesCalled = false;
}


void
CommandLineHandler::addOption(char *a_option)
{
  m_nOptions++;
  if (m_nOptions > 2)
    if ((m_options = (char **) realloc(m_options, m_nOptions * sizeof (char *))) == NULL)
      {
        fprintf(stderr, "ERROR: Reallocation failed\n");
        exit(1);
      }
  if ((m_options[m_nOptions - 1] = (char *) malloc(strlen(a_option) + 1)) == NULL)
    {
      fprintf(stderr, "ERROR: Not enough memory\n");
      exit(1);
    }
  strcpy(m_options[m_nOptions - 1], a_option);
}


int
CommandLineHandler::addElement(char **&a_array, int a_size, char *a_value)
{
  a_size++;
  if ((a_array = (char **) realloc(a_array, a_size * sizeof (char *))) == NULL)
    {
      fprintf(stderr, "ERROR: Reallocation failed\n");
      exit(1);
    }
  if ((a_array[a_size - 1] = (char *) malloc(strlen(a_value) + 1)) == NULL)
    {
      fprintf(stderr, "ERROR: Not enough memory\n");
      exit(1);
    }
  strcpy(a_array[a_size - 1], a_value);
  return a_size;
}


FXString *
CommandLineHandler::getSelectedFilenames(void)
{
  if (m_selectedFilenames)
    return m_selectedFilenames;

  m_selectedFilenames = m_mainWindow->getSelectedFilenames();
  if (m_fileNeeded)
    if (m_selectedFilenames == NULL)
      {
        FileDialog *fileDialog = new FileDialog(m_mainWindow,
          "Select one or more files");
        fileDialog->setSelectMode(SELECTFILE_MULTIPLE_ALL);
        if (!fileDialog->execute())
          return NULL;
        m_selectedFilenames = fileDialog->getFilenames();
        delete fileDialog;
      }
  return m_selectedFilenames;
}


void
CommandLineHandler::setSelectedFilenames(FXString *a_selectedFilenames)
/*
  This function is only used by MultiGameFileSelector. It's necessary because
  FileSelector can't store selections for different directories.
  MultiGameFileSelector builds an array of FXString objects without
  (FX)FileSelector.
*/
{
  m_selectedFilenames = a_selectedFilenames;
}


int
CommandLineHandler::addFiles(int a_filesToSkip)
{
  FXString *filenames = getSelectedFilenames();
  if (m_fileNeeded && filenames == NULL)
    return -1;

  int n = 0;
  if (filenames)
    {
      char *ptr;
#ifndef __unix__
      char file[FILENAME_MAX + 3];
#endif
      while (filenames[n][0])
        {
          if (n >= a_filesToSkip)
            {
              // Putting the file name between quotes is necessary under
              //  Windows... (if the name contains one or more spaces), but is
              //  not allowed under Linux...
#ifdef  __unix__
              ptr = (char *) filenames[n].text();
#else
              sprintf(file, "\"%s\"", filenames[n].text());
              ptr = file;
#endif
              addOption(ptr);
            }
          n++;
        }
      delete [] filenames;
    }
  m_selectedFilenames = NULL;
  m_addFilesCalled = true;
  return n;
}


bool
CommandLineHandler::busy(void)
{
  if (m_process)
    {
      //m_process->wait();
      if (m_process->running())
        {
          // I put this here for convenience (instead of in every place where
          //  CommandLineHandler::busy() is called)
          FXMessageBox::information(m_mainWindow, MBOX_OK, "uCON64 is still busy",
            "Please wait until uCON64 is finished or press the Abort button");
          return true;
        }

      // OutputChecker::check() does this, but it doesn't hurt to do it here too
      m_mainWindow->disableAbortButton();

      if (m_outputChecker)
        {
#ifdef  USE_THREAD
          m_outputChecker->cancel();
#else
#ifdef  USE_CHORE
          if (m_mainWindow->getApp()->hasChore(m_outputChecker, OutputChecker::ID_CHECK))
            m_mainWindow->getApp()->removeChore(m_outputChecker, OutputChecker::ID_CHECK);
#else
          m_mainWindow->getApp()->removeInput((FXInputHandle) m_process->getStdoutHandle(),
            INPUT_READ);
          m_mainWindow->getApp()->removeInput((FXInputHandle) m_process->getStderrHandle(),
            INPUT_READ);
#endif
#endif // USE_THREAD
          delete m_outputChecker;
          m_outputChecker = NULL;
        }
      delete m_process;
      m_process = NULL;
    }
  return false;
}


bool
CommandLineHandler::buildCommandLine(char **&a_options,
    OptionsWindow *a_optionsWindow)
{
  // complete command line
  TextFieldOption *textFieldOption;
  int org_nOptions = m_nOptions;
  char *ptr, buf[4096];
  FXString text;

  m_nOptions = 0;
  textFieldOption = a_optionsWindow->getuCON64Path();
  text = textFieldOption->getText().trim();
  ptr = (char *) text.text();
  if (!FXStat::exists(ptr))
    {
      FXMessageBox::error(m_mainWindow, MBOX_OK, "uCON64 path is not correct",
        "Please specify a correct uCON64 path in the options window");
      return false;
    }
  addOption(ptr);
  m_nOptions = org_nOptions;

  if (a_optionsWindow->runInFrontendMode())
    addOption("--frontend");                    // causes transfer options to
                                                //  open a progress window
  textFieldOption = a_optionsWindow->getParPortAddress();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "--port=%s", text.text());
      addOption(buf);
    }

  text = m_mainWindow->getOutputDir()->getText().trim();
#ifdef  __unix__
  sprintf(buf, "-o=%s", text.text());
#else
  sprintf(buf, "-o=\"%s\"", text.text());
#endif
  addOption(buf);

  if (!m_addFilesCalled)
    if (addFiles() == -1)
      return false;

  a_options = (char **) realloc(a_options, (m_nOptions + 1) * sizeof (char *));
  a_options[m_nOptions] = 0;
  return true;
}


void
CommandLineHandler::buildEnvironment(char **&a_environment,
    OptionsWindow *a_optionsWindow)
{
  int n = 0;
  char *ptr, buf[4096];
  TextFieldOption *textFieldOption;
  FXString text;

  if ((ptr = getenv("PATH")) == NULL)
#ifdef  __unix__
    sprintf(buf, "PATH=%s", "/cygdrive/c/cygwin/bin:.");
#else
    sprintf(buf, "PATH=%s", "c:\\cygwin\\bin;.");
#endif
  else
#ifdef  __unix__
    sprintf(buf, "PATH=%s%s", ptr, ":/cygdrive/c/cygwin/bin:.");
#else
    sprintf(buf, "PATH=%s%s", ptr, ";c:\\cygwin\\bin;.");
#endif
  n = addElement(a_environment, n, buf);

  if ((ptr = getenv("HOME")) != NULL)
    sprintf(buf, "HOME=%s", ptr);
  else if ((ptr = getenv("USERPROFILE")) != NULL)
    sprintf(buf, "HOME=%s", ptr);
  else if ((ptr = getenv("HOMEDRIVE")) != NULL)
    {
      char *ptr2 = getenv("HOMEPATH");
      sprintf(buf, "HOME=%s"PATHSEPSTRING"%s", ptr, ptr2 ? ptr2 : "");
    }
  n = addElement(a_environment, n, buf);

  textFieldOption = a_optionsWindow->getConfigFileDir();
  text = textFieldOption->getText().trim();
  sprintf(buf, "UCON64_HOME=%s", text.text());
  n = addElement(a_environment, n, buf);

  textFieldOption = a_optionsWindow->getConfigDir();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "ucon64_configdir=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getDatDir();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "ucon64_datdir=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getDiscmagePath();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "discmage_path=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
#if     defined __unix__ && !defined __CYGWIN__
  textFieldOption = a_optionsWindow->getParPortDevice();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "parport_dev=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
#endif
  if (a_optionsWindow->useBackup())
    {
      sprintf(buf, "backups=%d", a_optionsWindow->backupEnabled() ? 1 : 0);
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getGBALoaderPath();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "gbaloader=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getF2AFirmwarePath();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "f2afirmware=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getF2AClientUSBPath();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "iclientu=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getF2AClientParPortPath();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "iclientp=%s", text.text());
      n = addElement(a_environment, n, buf);
    }
  textFieldOption = a_optionsWindow->getF2ALogoPath();
  if (textFieldOption->enabled())
    {
      text = textFieldOption->getText().trim();
      sprintf(buf, "ilogo=%s", text.text());
      n = addElement(a_environment, n, buf);
    }

  a_environment = (char **) realloc(a_environment, (n + 1) * sizeof (char *));
  a_environment[n] = 0;
}


void
CommandLineHandler::run(void)
{
  OptionsWindow *optionsWindow = m_mainWindow->getOptionsWindow();
  if (!buildCommandLine(m_options, optionsWindow)) // it's *completed* really
    return;
  buildEnvironment(m_environment, optionsWindow);

#ifndef FORK_IPC
  m_process = new Process
#else
  m_process = new ProcessIPC
#endif
    ((const char **) m_options, (const char **) m_environment,
    optionsWindow->useIntermediaryFiles() ? true : false); // VC++: can't cast to bool

  FXText *outputText = m_mainWindow->getOutputText();
  outputText->setText("Waiting for output...", 21);
  m_progressDialog->reset();

  m_mainWindow->getApp()->beginWaitCursor();
  m_process->execute();
  m_outputChecker = new OutputChecker(m_process, outputText, m_progressDialog);

#ifdef  USE_THREAD
  m_outputChecker->start();
  m_outputChecker->detach();
#else
#ifdef  USE_CHORE
  // Using FXApp::addChore() gives much better results than using
  //  FXApp::addInput() (GUI is much more responsive, especially under Windows)
  m_mainWindow->getApp()->addChore(m_outputChecker, OutputChecker::ID_CHECK);
#else
  // Using FXApp::addInput() isn't 100% "safe", because OutputChecker won't be
  //  called anymore as soon as the process stops producing output
  m_mainWindow->getApp()->addInput((FXInputHandle) m_process->getStdoutHandle(),
    INPUT_READ, m_outputChecker, OutputChecker::ID_CHECK);
  m_mainWindow->getApp()->addInput((FXInputHandle) m_process->getStderrHandle(),
    INPUT_READ, m_outputChecker, OutputChecker::ID_CHECK);
#endif
#endif // USE_THREAD

  m_mainWindow->enableAbortButton();
  m_mainWindow->selectOutputTab();
  /*
    If the browse tab is not enabled, undo any file selection so that the
    functions (buttons) that require a file name come up with a file dialog. If
    the browse tab is enabled users can undo the selection themselves if they
    want a file dialog again.
  */
  if (!m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_BROWSE)->shown())
    m_mainWindow->clearSelectedFilenames();
}


long
CommandLineHandler::onAbort(FXObject *, FXSelector, void *)
{
  if (m_process)
    {
      m_process->abort();
#ifdef  USE_THREAD
      m_outputChecker->cancel();
#else
#ifdef  USE_CHORE
      if (m_mainWindow->getApp()->hasChore(m_outputChecker, OutputChecker::ID_CHECK))
        m_mainWindow->getApp()->removeChore(m_outputChecker, OutputChecker::ID_CHECK);
#else
      m_mainWindow->getApp()->removeInput((FXInputHandle) m_process->getStdoutHandle(),
        INPUT_READ);
      m_mainWindow->getApp()->removeInput((FXInputHandle) m_process->getStderrHandle(),
        INPUT_READ);
#endif
#endif // USE_THREAD
    }
  m_progressDialog->hide();
  m_mainWindow->disableAbortButton();
  m_mainWindow->getApp()->endWaitCursor();
  return 1;
}
