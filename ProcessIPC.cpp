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
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/mman.h>
#include "Process.h"
#include "ProcessIPC.h"


#define MMAP_FILE


unsigned char *ProcessIPC::m_sharedMemory = 0;
unsigned int ProcessIPC::m_sharedMemorySize = 0;
char ProcessIPC::m_sharedMemoryName[] = "";
int ProcessIPC::m_pipe1[2] = { 0, 0 }, ProcessIPC::m_pipe2[2] = { 0, 0 },
    ProcessIPC::m_pipeProcessStdout[2] = { 0, 0 },
    ProcessIPC::m_pipeProcessStderr[2] = { 0, 0 };


ProcessIPC::ProcessIPC(int a_sharedMemorySize)
// this constructor should be called before initializing FOX
{
#ifndef MMAP_FILE
  strcpy(m_sharedMemoryName, "/shared_memory_frontend");
#else
  strcpy(m_sharedMemoryName, "shared_memory_frontend");
#endif
  m_sharedMemorySize = a_sharedMemorySize;
  m_sharedMemory = initSharedMemory(m_sharedMemoryName, m_sharedMemorySize);

  if (pipe(m_pipe1) || pipe(m_pipe2))
    {
      fputs("ERROR: Could not create \"shared memory pipes\"\n", stderr);
      exit(1);
    }
  if (pipe(m_pipeProcessStdout) || pipe(m_pipeProcessStderr))
    {
      fputs("ERROR: Could not create \"stdio pipes\"\n", stderr);
      exit(1);
    }

  m_iPipe[0] = m_pipe1[0];
  m_iPipe[1] = m_pipe1[1];
  m_oPipe[0] = m_pipe2[0];
  m_oPipe[1] = m_pipe2[1];
  m_process = 0;
  m_localProcess = true;
  m_argv = NULL;
  m_argvSize = 0;
  m_envp = NULL;
  m_envpSize = 0;
}


unsigned char *
ProcessIPC::initSharedMemory(const char *a_name, int a_size)
{
  int shmem_fd;
  unsigned char *shmem_ptr = 0;

#ifndef MMAP_FILE
  if ((shmem_fd = shm_open(a_name, O_CREAT | O_RDWR | O_TRUNC,
         S_IRUSR | S_IWUSR)) == -1)
    {
      fputs("ERROR: Could not create shared memory object\n", stderr);
      exit(1);
    }

  if (ftruncate(shmem_fd, a_size) == -1)
    {
      fputs("ERROR: Could not resize shared memory\n", stderr);
      exit(1);
    }
#else
  if ((shmem_fd = open(a_name, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0)
    {
      fputs("ERROR: Could not open file for shared memory\n", stderr);
      exit(1);
    }

  unsigned char *buffer[a_size];
  memset(buffer, 0, a_size);

  if (write(shmem_fd, buffer, a_size) < 0)
    {
      fputs("ERROR: Could not write to file for shared memory\n", stderr);
      exit(1);
    }
#endif

  if ((shmem_ptr = (unsigned char *) mmap(0, a_size, PROT_READ | PROT_WRITE,
         MAP_SHARED, shmem_fd, 0)) == MAP_FAILED)
    {
      fputs("ERROR: Mapping memory failed\n", stderr);
      exit(1);
    }

  return shmem_ptr;
}


ProcessIPC::ProcessIPC(const char **a_argv, const char **a_envp, bool a_fakePipe)
// this constructor should be called where you would normally call the
//  constructor of Process
{
  m_iPipe[0] = m_pipe2[0];
  m_iPipe[1] = m_pipe2[1];
  m_oPipe[0] = m_pipe1[0];
  m_oPipe[1] = m_pipe1[1];
  m_process = 0;
  m_localProcess = false;
  m_argv = NULL;
  m_argvSize = 0;
  m_envp = NULL;
  m_envpSize = 0;

  if (!a_argv[0])
    {
      fprintf(stderr, "ERROR: Cannot start a process without a program name\n");
      exit(1);
    }

  passArgumentArray(a_argv, PROCESS_ADDARGVAR);
  passArgumentArray(a_envp, PROCESS_ADDENVVAR);
  *((bool *) m_sharedMemory) = a_fakePipe;
  call(PROCESS_CTOR);
}


void
ProcessIPC::passArgumentArray(const char **a_array, unsigned char a_method)
{
  int n;
  unsigned int size;

  for (n = 0; a_array[n]; n++)
    {
      size = strlen(a_array[n]) + 1;            // ASCII-z
      if (size > m_sharedMemorySize)
        {
          fprintf(stderr, "ERROR: Not enough shared memory\n");
          exit(1);
        }
      memcpy(m_sharedMemory, a_array[n], size);
      call(a_method);
      if (*((int *) m_sharedMemory))
        {
          fprintf(stderr, "ERROR: Could not add argument \"%s\"\n", a_array[n]);
          exit(1);
        }
    }

  *m_sharedMemory = 0;
  call(a_method);
  if (*((int *) m_sharedMemory))
    {
      fprintf(stderr, "ERROR: Could not add final argument\n");
      exit(1);
    }
}


void
ProcessIPC::addArgumentIPC(char **&a_array, int &a_nElements)
{
  int size;

  a_nElements++;
  if ((a_array = (char **) realloc(a_array, a_nElements * sizeof (char *)))
        == NULL)
    {
      fprintf(stderr, "ERROR: Not enough memory\n");
      *((int *) m_sharedMemory) = -1;
      return;
    }

  if (*m_sharedMemory)
    {
      size = strlen((char *) m_sharedMemory) + 1; // ASCII-z
      if ((a_array[a_nElements - 1] = (char *) malloc(size)) == NULL)
        {
          fprintf(stderr, "ERROR: Not enough memory\n");
          *((int *) m_sharedMemory) = -1;
          return;
        }
      memcpy(a_array[a_nElements - 1], m_sharedMemory, size);
    }
  else // *m_sharedMemory == 0 indicates last element
    a_array[a_nElements - 1] = 0;

  *((int *) m_sharedMemory) = 0;
}


void
ProcessIPC::ctorIPC(void)
{
  m_process = new Process((const char **) m_argv, (const char **) m_envp,
    *((bool *) m_sharedMemory));
  m_process->setStdPipes(NULL, m_pipeProcessStdout, m_pipeProcessStderr);
}


ProcessIPC::~ProcessIPC()
{
  if (!m_localProcess)
    call(PROCESS_DTOR);
}


void
ProcessIPC::dtorIPC(void)
{
  int n;

  for (n = 0; m_argv[n]; n++)
    free(m_argv[n]);
  free(m_argv);
  m_argv = NULL;
  m_argvSize = 0;

  for (n = 0; m_envp[n]; n++)
    free(m_envp[n]);
  free(m_envp);
  m_envp = NULL;
  m_envpSize = 0;

  delete m_process;
  m_process = NULL;
}


void
ProcessIPC::call(unsigned char a_method)
{
  fd_set rfds;

  write(m_oPipe[1], &a_method, 1);

  // now wait for acknowledgement
  FD_ZERO(&rfds);
  FD_SET(m_iPipe[0], &rfds);
  if (select(m_iPipe[0] + 1, &rfds, NULL, NULL, NULL) <= 0)
    {
      fprintf(stderr, "ERROR: select(), 1: %s\n", strerror(errno));
      exit(1);
    }
  // remove byte from pipe
  read(m_iPipe[0], &a_method, 1);
}


pid_t
ProcessIPC::startDispatcher(void)
{
  int pid;
  unsigned char method;
  fd_set rfds;
#ifdef  USE_SIGNAL
  sigset_t newSignalMask, orgSignalMask;
#endif

  switch (pid = fork())
    {
    case 0:                                     // child
      while (1)
        {
          FD_ZERO(&rfds);
          FD_SET(m_iPipe[0], &rfds);

#ifdef  USE_SIGNAL
          // block possible signal SIGCHLD
          sigemptyset(&newSignalMask);
#if 0     // we *have* to catch SIGCHLD
          sigaddset(&newSignalMask, SIGCHLD);
#endif
          sigprocmask(SIG_BLOCK, &newSignalMask, &orgSignalMask);
#endif
          if (select(m_iPipe[0] + 1, &rfds, NULL, NULL, NULL) <= 0)
            {
              fprintf(stderr, "ERROR: select(), 2: %s\n", strerror(errno));
              //exit(1);
            }
#ifdef  USE_SIGNAL
          sigprocmask(SIG_BLOCK, &orgSignalMask, NULL);
#endif

          read(m_iPipe[0], &method, 1);
          switch (method)
            {
            case PROCESS_ADDARGVAR:
              addArgumentIPC(m_argv, m_argvSize);
              break;
            case PROCESS_ADDENVVAR:
              addArgumentIPC(m_envp, m_envpSize);
              break;
            case PROCESS_CTOR:
              ctorIPC();
              break;
            case PROCESS_DTOR:
              dtorIPC();
              break;
            case PROCESS_EXECUTE:
              executeIPC();
              break;
            case PROCESS_OUTPUTAVAILABLE:
              outputAvailableIPC();
              break;
            case PROCESS_READSTDOUT:
              readStdoutIPC();
              break;
            case PROCESS_READSTDERR:
              readStderrIPC();
              break;
            case PROCESS_WRITESTDIN:
              writeStdinIPC();
              break;
            case PROCESS_WAIT:
              waitIPC();
            case PROCESS_RUNNING:
              runningIPC();
              break;
            case PROCESS_ABORT:
              abortIPC();
              break;
            case PROCESS_GETPID:
              getPidIPC();
              break;
            case PROCESS_STOPDISPATCHER:
              fputs("NOTE: Stopping dispatcher...", stdout);
              fflush(stdout);
              write(m_oPipe[1], &method, 1);    // send ack here, because we call exit()
              exit(0);
            default:
              fprintf(stderr, "ERROR: Unsupported method code (%d)\n", method);
              break;
            }
          write(m_oPipe[1], &method, 1);        // send acknowledgement, value doesn't matter
        }
      break;
    case -1:
      fputs("ERROR: Could not create process\n", stderr);
      exit(1);
      break;
    default:                                    // parent
      break;
    }

  return pid;
}


void
ProcessIPC::stopDispatcher(void)
{
  fd_set rfds;
  unsigned char method = PROCESS_STOPDISPATCHER;

  write(m_iPipe[1], &method, 1);

  FD_ZERO(&rfds);
  FD_SET(m_oPipe[0], &rfds);
  if (select(m_oPipe[0] + 1, &rfds, NULL, NULL, NULL) <= 0)
    {
      fprintf(stderr, "ERROR: select(), 3: %s\n", strerror(errno));
      exit(1);
    }
  fputs("done\n", stdout);
#if 0 // Do NOT read from m_oPipe anymore. It will block on fast computers.
  read(m_oPipe[0], &method, 1);
#endif

  munmap(m_sharedMemory, m_sharedMemorySize);
#ifndef MMAP_FILE
  shm_unlink(m_sharedMemoryName);
#else
  unlink(m_sharedMemoryName);
#endif
}


pid_t
ProcessIPC::execute(void)
{
  call(PROCESS_EXECUTE);
  return *((pid_t *) m_sharedMemory);
}


void
ProcessIPC::executeIPC(void)
{
  pid_t pid = m_process->execute();
  *((pid_t *) m_sharedMemory) = pid;
}


int
ProcessIPC::outputAvailable(void)
{
  call(PROCESS_OUTPUTAVAILABLE);
  return *((int *) m_sharedMemory);
}


void
ProcessIPC::outputAvailableIPC(void)
{
  int status = m_process->outputAvailable();
  *((int *) m_sharedMemory) = status;
}


int
ProcessIPC::readStdout(unsigned char *a_buf, int a_size)
{
  if (sizeof (int) + a_size + sizeof (a_size) > m_sharedMemorySize)
    return 0;
  *((int *) (m_sharedMemory + sizeof (int))) = a_size;
  call(PROCESS_READSTDOUT);

  // Process::readPipe() terminates the buffer => copy one byte more than the
  //  returned value
  memcpy(a_buf, m_sharedMemory + 2 * sizeof (int), *((int *) m_sharedMemory) + 1);
  return *((int *) m_sharedMemory);
}


void
ProcessIPC::readStdoutIPC(void)
{
  // return value, size, buffer
  int n = m_process->readStdout(m_sharedMemory + 2 * sizeof (int),
            *((int *) (m_sharedMemory + sizeof (int))));
  *((int *) m_sharedMemory) = n;
}


int
ProcessIPC::readStderr(unsigned char *a_buf, int a_size)
{
  if (sizeof (int) + a_size + sizeof (a_size) > m_sharedMemorySize)
    return 0;
  *((int *) (m_sharedMemory + sizeof (int))) = a_size;
  call(PROCESS_READSTDERR);

  // Process::readPipe() terminates the buffer => copy one byte more than the
  //  returned value
  memcpy(a_buf, m_sharedMemory + 2 * sizeof (int), *((int *) m_sharedMemory) + 1);
  return *((int *) m_sharedMemory);
}


void
ProcessIPC::readStderrIPC(void)
{
  // return value, size, buffer
  int n = m_process->readStderr(m_sharedMemory + 2 * sizeof (int),
            *((int *) (m_sharedMemory + sizeof (int))));
  *((int *) m_sharedMemory) = n;
}


int
ProcessIPC::writeStdin(unsigned char *a_buf, int a_size)
{
  if (sizeof (int) + a_size + sizeof (a_size) > m_sharedMemorySize)
    return 0;
  *((int *) (m_sharedMemory + sizeof (int))) = a_size;
  memcpy(m_sharedMemory + 2 * sizeof (int), a_buf, a_size);
  call(PROCESS_WRITESTDIN);

  return *((int *) m_sharedMemory);
}


void
ProcessIPC::writeStdinIPC(void)
{
  // return value, size, buffer
  int n = m_process->writeStdin(m_sharedMemory + 2 * sizeof (int),
            *((int *) (m_sharedMemory + sizeof (int))));
  *((int *) m_sharedMemory) = n;
}


void
ProcessIPC::wait(void)
{
  call(PROCESS_WAIT);
}


void
ProcessIPC::waitIPC(void)
{
  m_process->wait();
}


bool
ProcessIPC::running(void)
{
  call(PROCESS_RUNNING);
  return *((bool *) m_sharedMemory);
}


void
ProcessIPC::runningIPC(void)
{
  bool running = m_process->running();
  *((bool *) m_sharedMemory) = running;
}


void
ProcessIPC::abort(void)
{
  call(PROCESS_ABORT);
}


void
ProcessIPC::abortIPC(void)
{
  m_process->abort();
}


pid_t
ProcessIPC::getPid(void)
{
  call(PROCESS_GETPID);
  return *((pid_t *) m_sharedMemory);
}


void
ProcessIPC::getPidIPC(void)
{
  pid_t pid = m_process->getPid();
  *((pid_t *) m_sharedMemory) = pid;
}
