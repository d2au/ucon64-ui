/*
  uCON64 frontend, a graphical frontend for the ROM tool uCON64.
  Copyright (c) 2004 dbjh

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
#ifndef COMMANDLINEHANDLER_H
#define COMMANDLINEHANDLER_H

#include <fx.h>
#include "MainWindow.h"
#include "Process.h"
#ifdef  FORK_IPC
#include "ProcessIPC.h"
#endif
#include "OutputChecker.h"
#include "ProgressDialog.h"
#include "OptionsWindow.h"


class CommandLineHandler : public FXObject
{
  FXDECLARE(CommandLineHandler)
public:
  CommandLineHandler(MainWindow *mainWindow);
  virtual ~CommandLineHandler();
  void setOption(char *options, bool fileNeeded);
  void addOption(char *option);
  bool busy(void);
  void run(void);
  FXString *getSelectedFilenames(void);
  void setSelectedFilenames(FXString *selectedFilenames);
  int addFiles(int filesToSkip = 0);
  long onAbort(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_ABORT,
      ID_LAST
    };

protected:
  CommandLineHandler() {}
  int addElement(char **&array, int size, char *value);
  bool buildCommandLine(char **&options, OptionsWindow *optionsWindow);
  void buildEnvironment(char **&environment, OptionsWindow *optionsWindow);

  MainWindow *m_mainWindow;
  ProgressDialog *m_progressDialog;
  OutputChecker *m_outputChecker;
#ifndef FORK_IPC
  Process *m_process;
#else
  ProcessIPC *m_process;
#endif
  char **m_options, **m_environment;
  bool m_fileNeeded, m_addFilesCalled;
  int m_nOptions;
  FXString *m_selectedFilenames;
};

#else
class CommandLineHandler;
#endif
