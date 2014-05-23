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
#ifndef PROCESS_H
#define PROCESS_H

#ifdef  __unix__
// See note about USE_THREAD in OutputChecker.h
//#define USE_SIGNAL
#endif

#include <stdio.h>
#include <sys/types.h>
#ifdef  USE_SIGNAL
#include <signal.h>
#endif

#ifdef  _MSC_VER
typedef int pid_t;
#endif


class Process
{
public:
  Process(const char **argv, const char **envp, bool fakePipe);
  virtual ~Process();

  pid_t execute(void);
  int outputAvailable(void);
  int readStdout(unsigned char *buf, int size);
  int readStderr(unsigned char *buf, int size);
  int writeStdin(unsigned char *buf, int size);
  void wait(void);
  bool running(void);
  void abort(void);
  pid_t getPid(void) { return m_pid; }
  void setStdPipes(int *pipe1, int *pipe2, int *pipe3);
  int getStdoutHandle(void) { return m_pipe2[0]; }
  int getStderrHandle(void) { return m_pipe3[0]; }

protected:
  Process() {}

  void cloneArgumentArray(char **&dest, const char **src);
  void openPipeFiles(int *pipe2, int *pipe3);
  int readPipe(int fd, unsigned char *buf, int size);
  int writePipe(int fd, unsigned char *buf, int size);

  FILE *m_errorFile;
  char **m_argv, **m_envp;
  int m_pipe1[2], m_pipe2[2], m_pipe3[2];
  pid_t m_pid;
  bool m_fakePipe, m_pipe1Local, m_pipe2Local, m_pipe3Local;

#ifdef  __unix
#ifdef  USE_SIGNAL
  static void childTerminated(int signum);

  struct sigaction m_orgAction;
  static int m_childRunning;
#endif
#endif
};

#endif
