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
#ifndef PROCESSIPC_H
#define PROCESSIPC_H

#include <stdio.h>
#include <sys/types.h>
#include "Process.h"


class ProcessIPC
{
public:
  ProcessIPC(int sharedMemorySize);
  ProcessIPC(const char **argv, const char **envp, bool fakePipe);
  virtual ~ProcessIPC();

  pid_t startDispatcher(void);
  void stopDispatcher(void);
  pid_t execute(void);
  int outputAvailable(void);
  int readStdout(unsigned char *buf, int size);
  int readStderr(unsigned char *buf, int size);
  int writeStdin(unsigned char *buf, int size);
  void wait(void);
  bool running(void);
  void abort(void);
  pid_t getPid(void);
  int getStdoutHandle(void) { return m_pipeProcessStdout[0]; }
  int getStderrHandle(void) { return m_pipeProcessStderr[0]; }

protected:
  ProcessIPC() {}

  unsigned char *initSharedMemory(const char *name, int size);
  void passArgumentArray(const char **array, unsigned char method);
  void addArgumentIPC(char **&array, int &nElements);
  void ctorIPC(void);
  void dtorIPC(void);
  void call(unsigned char method);
  void executeIPC(void);
  void outputAvailableIPC(void);
  void readStdoutIPC(void);
  void readStderrIPC(void);
  void writeStdinIPC(void);
  void waitIPC(void);
  void runningIPC(void);
  void abortIPC(void);
  void getPidIPC(void);

  Process *m_process;
  bool m_localProcess;
  static unsigned char *m_sharedMemory;
  static unsigned int m_sharedMemorySize;
  static char m_sharedMemoryName[FILENAME_MAX];
  static int m_pipe1[2], m_pipe2[2], m_pipeProcessStdout[2],
    m_pipeProcessStderr[2];
  int m_iPipe[2], m_oPipe[2], m_argvSize, m_envpSize;
  char **m_argv, **m_envp;

  enum
  {
    PROCESS_ADDARGVAR,
    PROCESS_ADDENVVAR,
    PROCESS_CTOR,
    PROCESS_DTOR,
    PROCESS_EXECUTE,
    PROCESS_OUTPUTAVAILABLE,
    PROCESS_READSTDOUT,
    PROCESS_READSTDERR,
    PROCESS_WRITESTDIN,
    PROCESS_WAIT,
    PROCESS_RUNNING,
    PROCESS_ABORT,
    PROCESS_GETPID,
    PROCESS_STOPDISPATCHER
  };
};

#endif
