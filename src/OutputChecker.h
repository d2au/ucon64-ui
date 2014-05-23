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
#ifndef OUTPUTCHECKER_H
#define OUTPUTCHECKER_H

#include <fx.h>
#include <FXThread.h>
#include "Process.h"
#ifdef  FORK_IPC
#include "ProcessIPC.h"
#endif
#include "ProgressDialog.h"


//#define USE_THREAD
/*
  If you define USE_THREAD also define FORK_IPC in Makefile (if you want a
  stable app). It seems as if it's best to also define USE_SIGNAL in Process.h.
  Without USE_SIGNAL, sometimes there are problems with ProgressDialog.
*/


class OutputChecker : public FXThread, public FXObject
{
  FXDECLARE(OutputChecker)
public:
#ifndef FORK_IPC
  OutputChecker(Process *process,
#else
  OutputChecker(ProcessIPC *process,
#endif
      FXText *output, ProgressDialog *progressDialog);
  virtual FXint run();
  long check(FXObject *, FXSelector, void *);

  enum
    {
      ID_CHECK,
      ID_LAST
    };

protected:
  OutputChecker() {}
  int fixEndOfLine(char *line, int length);
  void addOutput(char *buf, int length);

#ifndef FORK_IPC
  Process *m_process;
#else
  ProcessIPC *m_process;
#endif
  FXText *m_output;
  ProgressDialog *m_progressDialog;
  int m_checkState, m_startOfLine, m_column, m_lineLen;
};

#endif
