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
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#ifdef  __unix__
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#else
#include <windows.h>
#endif
#include "Process.h"


//#define COPY_ARGUMENT_ARRAYS


Process::Process(const char **a_argv, const char **a_envp, bool a_fakePipe)
{
#if 0
  if ((m_errorFile = fopen("process_errors.txt", "ab")) == NULL)
    {
      fputs("WARNING: Could not open error message file\n", stdout);
//      exit(1);
    }
#else
  m_errorFile = NULL;
#endif

#ifdef  COPY_ARGUMENT_ARRAYS
  m_argv = NULL;
  m_envp = NULL;
  cloneArgumentArray(m_argv, a_argv);
  cloneArgumentArray(m_envp, a_envp);
#else
  m_argv = (char **) a_argv;
  m_envp = (char **) a_envp;
#endif

  m_pipe1[0] = 0;
  m_pipe1[1] = -1;                              // ugly (we don't change stdin)
  m_pipe2[0] = 0;
  m_pipe2[1] = 0;
  m_pipe3[0] = 0;
  m_pipe3[1] = 0;
  m_pipe1Local = true;
  m_pipe2Local = true;
  m_pipe3Local = true;

  // Using intermediary files makes things go much slower, but at least we catch
  //  *all* output
  m_fakePipe = a_fakePipe;
  m_pid = 0;

#ifdef  __unix__
#ifdef  USE_SIGNAL
  struct sigaction action;

  m_childRunning = 0;

  memset(&action , 0, sizeof (action));
  action.sa_handler = childTerminated;
  action.sa_flags = SA_RESTART;                 // "restarts" for example printf()
  if (sigaction(SIGCHLD, &action, &m_orgAction) == 1)
    {
      fputs("ERROR: Could not install signal handler\n", stderr);
      exit(1);
    }
#endif
#endif
}


void
Process::cloneArgumentArray(char **&a_dest, const char **a_src)
{
  int n, n_max = 0, size;

  for (n = 0; a_src[n]; n++)
    {
      if (n == n_max)
        {
          n_max += 10;
          if ((a_dest = (char **) realloc(a_dest, n_max * sizeof (char *)))
                 == NULL)
            {
              fprintf(stderr, "ERROR: Not enough memory\n");
              exit(1);
            }
        }
      size = strlen(a_src[n]) + 1;              // ASCII-z
      if ((a_dest[n] = (char *) malloc(size)) == NULL)
        {
          fprintf(stderr, "ERROR: Not enough memory\n");
          exit(1);
        }
      memcpy(a_dest[n], a_src[n], size);
    }

  a_dest = (char **) realloc(a_dest, (n + 1) * sizeof (char *));
  a_dest[n] = 0;
}


Process::~Process()
{
#ifdef  COPY_ARGUMENT_ARRAYS
  int n;

  for (n = 0; m_argv[n]; n++)
    free(m_argv[n]);
  free(m_argv);
  for (n = 0; m_envp[n]; n++)
    free(m_envp[n]);
  free(m_envp);
#endif
  m_argv = NULL;
  m_envp = NULL;

  if (running())
    abort();
#ifdef  __unix__
#ifdef  USE_SIGNAL
  else
    waitpid(m_pid, NULL, WNOHANG);              // we don't want to keep zombies
                                                //  running around
  sigaction(SIGCHLD, &m_orgAction, NULL);
#endif

  if (m_pipe2Local)
    {
      close(m_pipe2[1]);
      close(m_pipe2[0]);
    }
  if (m_pipe3Local)
    {
      close(m_pipe3[1]);
      close(m_pipe3[0]);
    }
#else // __unix__
  CloseHandle((HANDLE) m_pid);                  // PROCESS_INFORMATION.hProcess,
  CloseHandle((HANDLE) m_pipe2[1]);             //  see comment at end of execute()
  CloseHandle((HANDLE) m_pipe2[0]);
  CloseHandle((HANDLE) m_pipe3[1]);
  CloseHandle((HANDLE) m_pipe3[0]);
#endif

  if (m_errorFile)
    fclose(m_errorFile);
}


#ifdef  USE_SIGNAL
int Process::m_childRunning = 0;

void
Process::childTerminated(int a_signum)
{
  if (a_signum != SIGCHLD)
    return;

//  fputs("SIGCHLD\n", stderr);

  m_childRunning = 0;
}
#endif


void
Process::setStdPipes(int *a_pipe1, int *a_pipe2, int *a_pipe3)
// This function should be called before calling execute(). It's only useful
//  for ProcessIPC when CommandLineHandler uses FXApp::addInput()
{
  if (!m_fakePipe)
    {
      if (a_pipe1)
        {
          m_pipe1[0] = a_pipe1[0];
          m_pipe1[1] = a_pipe1[1];
          m_pipe1Local = false;
        }
      if (a_pipe2)
        {
          m_pipe2[0] = a_pipe2[0];
          m_pipe2[1] = a_pipe2[1];
          m_pipe2Local = false;
        }
      if (a_pipe3)
        {
          m_pipe3[0] = a_pipe3[0];
          m_pipe3[1] = a_pipe3[1];
          m_pipe3Local = false;
        }
    }
}


pid_t
Process::execute(void)
{
  if (m_fakePipe)
    openPipeFiles(m_pipe2, m_pipe3);

#ifdef  __unix__
  int fd, n;

  if (!(m_pipe1[0] || m_pipe1[1]))
    if (pipe(m_pipe1))
      {
        fputs("ERROR: Could not create pipe for standard input stream\n", stderr);
        exit(1);
      }
  if (!(m_pipe2[0] || m_pipe2[1]))
    if (pipe(m_pipe2))
      {
        fputs("ERROR: Could not create pipe for standard output stream\n", stderr);
        exit(1);
      }
  if (!(m_pipe3[0] || m_pipe3[1]))
    if (pipe(m_pipe3))
      {
        fputs("ERROR: Could not create pipe for standard error stream\n", stderr);
        exit(1);
      }

  switch (m_pid = fork())
    {
    case 0:
      printf("Executing: %s", m_argv[0]);
      for (n = 1; m_argv[n]; n++)
        printf(" %s", m_argv[n]);
      fputc('\n', stdout);
      fflush(stdout);

      if (m_pipe1[0])
        {
          close(0);                             // close standard input stream
          fd = dup(m_pipe1[0]);
          if (fd == -1 || fd != 0)
            {
              fputs("ERROR: Could not change standard input stream\n", stderr);
              exit(1);
            }
        }
      if (m_pipe2[1])
        {
          close(1);                             // close standard output stream
          fd = dup(m_pipe2[1]);
          if (fd == -1 || fd != 1)
            {
              fputs("ERROR: Could not change standard output stream\n", stderr);
              exit(1);
            }
        }
      if (m_pipe3[1])
        {
          close(2);                             // close standard error stream
          fd = dup(m_pipe3[1]);
          if (fd == -1 || fd != 2)
            {
              // We have no standard stream left...
              if (m_errorFile)
                fputs("ERROR: Could not change standard error stream\n", m_errorFile);
              exit(1);
            }
        }

      setuid(geteuid());
      setgid(getegid());
      if (execve(m_argv[0], (char * const *) m_argv, (char * const *) m_envp))
        { // error in info page: return value can be != 1
          if (m_errorFile)
            {
              fprintf(m_errorFile, "ERROR %#08x: %s\n", errno, strerror (errno));
              for (n = 0; m_argv[n]; n++)
                fprintf(m_errorFile, "m_argv[%d]: %s\n", n, m_argv[n]);
              for (n = 0; m_envp[n]; n++)
                fprintf(m_errorFile, "m_envp[%d]: %s\n", n, m_envp[n]);
            }
          exit(1);
        }
      break;
    case -1:
      fputs("ERROR: Could not create process\n", stderr);
      exit(1);
      break;
    default:
#ifdef  USE_SIGNAL
      m_childRunning = 1;
#endif
      break;
    }

  return m_pid;

#else // __unix__

  int n, len, len2;
  char *commandLine, *environment;
  SECURITY_ATTRIBUTES sa;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  // create command line
  len = 0;
  for (n = 0; m_argv[n]; n++)
    len += strlen(m_argv[n]) + 1;               // +1 for space
  len++;                                        // +1 for string terminator
  if ((commandLine = (char *) malloc(len)) == NULL)
    {
      fprintf(stderr, "ERROR: Not enough memory for command line (%d bytes)\n", len);
      exit(1);
    }
  len = 0;
  for (n = 0; m_argv[n]; n++)
    {
      len2 = strlen(m_argv[n]);
      len += len2;
      memcpy(commandLine + len - len2, m_argv[n], len2);
      commandLine[len++] = ' ';
    }
  commandLine[len] = 0;

  // create environment
  len = 0;
  for (n = 0; m_envp[n]; n++)
    len += strlen(m_envp[n]) + 1;               // +1 for zero (string terminator)
  len++;                                        // +1 for closing zero
  if ((environment = (char *) malloc(len)) == NULL)
    {
      fprintf(stderr, "ERROR: Not enough memory for environment (%d bytes)\n", len);
      exit(1);
    }
  len = 0;
  for (n = 0; m_envp[n]; n++)
    {
      len2 = strlen(m_envp[n]) + 1;
      len += len2;
      memcpy(environment + len - len2, m_envp[n], len2);
    }
  environment[len] = 0;

  memset(&sa, 0, sizeof (sa));
  sa.nLength = sizeof (sa);
  sa.lpSecurityDescriptor = NULL;
  sa.bInheritHandle = TRUE;

  memset(&si, 0, sizeof (si));
  si.cb = sizeof (si);
  si.dwFlags = STARTF_USESTDHANDLES;
  // Under Windows 98, when starting from command.com or explorer.exe,
  //  GetStdHandle() seems to always return INVALID_HANDLE_VALUE
  si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
  si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
  if (!(m_pipe1[0] || m_pipe1[1]))
    if (!CreatePipe((PHANDLE) &m_pipe1[0], (PHANDLE) &m_pipe1[1], &sa, 0))
      {
        fputs("ERROR: Could not create pipe for standard input device\n", stderr);
        exit(1);
      }
  if (!(m_pipe2[0] || m_pipe2[1]))
    if (!CreatePipe((PHANDLE) &m_pipe2[0], (PHANDLE) &m_pipe2[1], &sa, 0))
      {
        fputs("ERROR: Could not create pipe for standard output device\n", stderr);
        exit(1);
      }
  if (!(m_pipe3[0] || m_pipe3[1]))
    if (!CreatePipe((PHANDLE) &m_pipe3[0], (PHANDLE) &m_pipe3[1], &sa, 0))
      {
        fputs("ERROR: Could not create pipe for standard error device\n", stderr);
        exit(1);
      }

  if (m_pipe1[0])
    si.hStdInput = (HANDLE) m_pipe1[0];
  if (m_pipe2[1])
    si.hStdOutput = (HANDLE) m_pipe2[1];
  if (m_pipe3[1])
    si.hStdError = (HANDLE) m_pipe3[1];

  if (//si.hStdInput == INVALID_HANDLE_VALUE ||
      si.hStdOutput == INVALID_HANDLE_VALUE ||
      si.hStdError == INVALID_HANDLE_VALUE)
    {
      fputs("ERROR: Could not get or create handles for stdin, stdout or stderr device\n", stderr);
      exit(1);
    }

  memset(&pi, 0, sizeof (pi));

  printf("Executing: %s\n", commandLine);
  fflush(stdout);

  if (!CreateProcess(NULL, commandLine, NULL, NULL, TRUE, DETACHED_PROCESS,
                     environment, NULL, &si, &pi))
    {
      char *msg = "ERROR: Could not create process\n";
      fputs(msg, stderr);
      if (m_errorFile)
        {
          fputs(msg, m_errorFile);
          for (n = 0; m_argv[n]; n++)
            fprintf(m_errorFile, "m_argv[%d]: %s\n", n, m_argv[n]);
          for (n = 0; m_envp[n]; n++)
            fprintf(m_errorFile, "m_envp[%d]: %s\n", n, m_envp[n]);
        }
//      exit(1);
    }

  free(commandLine);
  free(environment);

  // According to MSDN these handles must be closed. We only close pi.hThread,
  //  because we need pi.hProcess to refer to the process.
//  CloseHandle (pi.hProcess);
  CloseHandle(pi.hThread);

  m_pid = (pid_t) pi.hProcess; //pi.dwProcessId;
  return m_pid;
#endif
}


void
Process::openPipeFiles(int *a_pipe2, int *a_pipe3)
{
#ifdef  __unix__
#ifdef  __APPLE__
#define O_SYNC 0
#endif
  if ((a_pipe2[1] = open("stdout", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0666)) < 0)
    {
      fputs("ERROR: Could not open file that stores standard output data\n", stderr);
      exit(1);
    }
  if ((a_pipe2[0] = open("stdout", O_RDONLY)) < 0)
    {
      fputs("ERROR: Could not open file that stores standard output data\n", stderr);
      exit(1);
    }

  if ((a_pipe3[1] = open("stderr", O_WRONLY | O_CREAT | O_TRUNC | O_SYNC, 0666)) < 0)
    {
      fputs("ERROR: Could not open file that stores standard error data\n", stderr);
      exit(1);
    }
  if ((a_pipe3[0] = open("stderr", O_RDONLY)) < 0)
    {
      fputs("ERROR: Could not open file that stores standard error data\n", stderr);
      exit(1);
    }
#else // __unix__
  SECURITY_ATTRIBUTES sa = { sizeof (SECURITY_ATTRIBUTES), NULL, TRUE };

  if ((a_pipe2[1] = (int) CreateFile("stdout", GENERIC_WRITE,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                                     CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
        == (int) INVALID_HANDLE_VALUE)
    {
      fputs("ERROR: Could not open file that stores standard output data\n", stderr);
      exit(1);
    }
  if ((a_pipe2[0] = (int) CreateFile("stdout", GENERIC_READ,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
        == (int) INVALID_HANDLE_VALUE)
    {
      fputs("ERROR: Could not open file that stores standard output data\n", stderr);
      exit(1);
    }

  if ((a_pipe3[1] = (int) CreateFile("stderr", GENERIC_WRITE,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                                     CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL))
        == (int) INVALID_HANDLE_VALUE)
    {
      fputs("ERROR: Could not open file that stores standard error data\n", stderr);
      exit(1);
    }
  if ((a_pipe3[0] = (int) CreateFile("stderr", GENERIC_READ,
                                     FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                                     OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL))
        == (int) INVALID_HANDLE_VALUE)
    {
      fputs("ERROR: Could not open file that stores standard error data\n", stderr);
      exit(1);
    }
#endif
}


int
Process::readPipe(int a_fd, unsigned char *a_buf, int a_size)
// a_size must be >= 2
{
  int n = 0, m;
#if 0//def  __unix__
  if (m_fakePipe)
    {
      // When end of file is reached for a_fd, we have to clear the end of file
      //  indicator or else read() won't read any new bytes.
      int fd2 = dup(a_fd);
      FILE *dummy;
      if ((dummy = fdopen(fd2, "r")) != NULL)
        {
          clearerr(dummy);
          fclose(dummy);
        }
    }
#endif

  do
    {
#ifdef  __unix__
      m = read(a_fd, a_buf + n, 1);
#else
      if (!ReadFile((HANDLE) a_fd, a_buf + n, 1, (LPDWORD) &m, NULL))
        {
          fputs("ERROR: Could not read from pipe\n", stderr);
          exit(1);
        }
#endif
      n += m;
    }
  while (!((n && (a_buf[n - 1] == '\n' || a_buf[n - 1] == '\r')) ||
            n >= a_size - 1 || m <= 0));
  a_buf[n] = 0;

  return n;
}


int
Process::writePipe(int a_fd, unsigned char *a_buf, int a_size)
{
  // Writing to pipe1 doesn't result in the wanted behaviour. For some
  //  reason it's difficult to make the key strokes arrive at the process.
#ifdef  __unix__
  return write(a_fd, a_buf, a_size);
#else
  int m;
  if (!WriteFile((HANDLE) a_fd, a_buf, a_size, (LPDWORD) &m, NULL))
    {
      fputs("ERROR: Could not write to pipe\n", stderr);
      exit(1);
    }
  return m;
#endif
}


int
Process::outputAvailable(void)
{
  // PeekNamedPipe() does not work for files, returning 1 | 2 is alright
  if (m_fakePipe)
    return 1 | 2;

  int status = 0;
#ifdef  __unix__
  fd_set rfds;
  struct timeval tv = { 0, 50 };

  FD_ZERO(&rfds);
  FD_SET(m_pipe2[0], &rfds);
  FD_SET(m_pipe3[0], &rfds);
  if (select((m_pipe3[0] > m_pipe2[0] ? m_pipe3[0] : m_pipe2[0]) + 1, &rfds,
        NULL, NULL, &tv) < 0)
    {
      fprintf(stderr, "ERROR: select(): %s\n", strerror(errno));
      // Don't exit. On my PC displaying info about a dump often interrupts this
      //  select() call...
      return 0;
    }
  if (FD_ISSET(m_pipe2[0], &rfds))
    status |= 1;
  if (FD_ISSET(m_pipe3[0], &rfds))
    status |= 2;
#else
  unsigned char byte;
  DWORD n;

  if (!PeekNamedPipe((HANDLE) m_pipe2[0], &byte, 1, &n, NULL, NULL))
    {
      fputs("ERROR: Could not check pipe\n", stderr);
      exit(1);
    }
  if (n)
    status |= 1;
  if (!PeekNamedPipe((HANDLE) m_pipe3[0], &byte, 1, &n, NULL, NULL))
    {
      fputs("ERROR: Could not check pipe\n", stderr);
      exit(1);
    }
  if (n)
    status |= 2;
#endif
  return status;
}


int
Process::readStdout(unsigned char *a_buf, int a_size)
{
  return readPipe(m_pipe2[0], a_buf, a_size);
}


int
Process::readStderr(unsigned char *a_buf, int a_size)
{
  return readPipe(m_pipe3[0], a_buf, a_size);
}


int
Process::writeStdin(unsigned char *a_buf, int a_size)
{
  return writePipe(m_pipe1[1], a_buf, a_size);
}


void
Process::wait(void)
{
#ifdef  __unix__
#if 0//def  USE_SIGNAL
  while (m_childRunning == 1)
    usleep(1000);
#else
  // see comment in running()
  waitpid(m_pid, NULL, 0);
#endif
#else // __unix__
  WaitForSingleObject((HANDLE) m_pid, INFINITE);
#endif
}


bool
Process::running(void)
{
#ifdef  __unix__
#ifdef  USE_SIGNAL
  return m_childRunning == 1;
#else
  int status;

  /*
    waitpid() doesn't work if we're running in a thread (no child processes),
    even if __WALL is used. We solve the problem by installing a signal handler
    instead.
  */
  return waitpid(m_pid, &status, WNOHANG) == 0;
#endif // USE_SIGNAL
#else
  // Returns WAIT_TIMEOUT while the process is running, WAIT_OBJECT_0 when
  //  it's finished. Using a time-out interval of 0 does not work.
  DWORD status = WaitForSingleObject((HANDLE) m_pid, 10);
  if (status != WAIT_FAILED)
    return status != WAIT_OBJECT_0;
  else
    return 0;
#endif
}


void
Process::abort(void)
{
  fputs("Stopping process...", stdout);
  fflush(stdout);

#ifdef  __unix__
  int status;

  if (waitpid(m_pid, &status, WNOHANG))
    {
      puts("process already exited");
      fflush(stdout);
      return;
    }

  kill(m_pid, SIGQUIT); // SIGKILL
  // if WNOHANG is used, if the time we wait is too short we will end up with a
  //  zombie process
  usleep(1000);// * 100);
//  sleep(1);
#ifdef  USE_SIGNAL
  m_childRunning = 0;
#endif
  waitpid(m_pid, &status, 0);//WNOHANG);
#else // __unix__
  TerminateProcess((HANDLE) m_pid, 1); // exit code 1
#endif

  puts("done");
  fflush(stdout);
}
