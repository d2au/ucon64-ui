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
#include <stdio.h>
#include <string.h>
#ifdef  __unix__
#include <unistd.h>                             // tcgetattr(), tcsetattr()
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#ifdef  FORK_IPC
#include "ProcessIPC.h"
#endif
#endif
#include "Frontend.h"


#ifdef  __unix__
struct termios org_tty;
int org_tty_set = 0;
#endif


int
main(int argc, char **argv)
{
#ifdef  FORK_IPC
  ProcessIPC *process = new ProcessIPC(8 * 1024);
  pid_t pid = process->startDispatcher();
#endif

#ifdef  __unix__
  if (isatty(STDIN_FILENO))
    if (tcgetattr(STDIN_FILENO, &org_tty) == 0)
      org_tty_set = 1;
#endif

  Frontend frontend("uCON64 frontend", "dbjh@users.sourceforge.net");
  frontend.init(argc, argv);
  frontend.create();
  int exitStatus = frontend.run();

#ifdef  __unix__
  /*
    We could have aborted uCON64, so it has no chance of restoring the tty
    settings. We *try* to restore the tty settings, so we don't display an error
    if we can't.
    tcsetattr() blocks the current process if it was started in the background
    ("./uf &"). However, the frontend/uCON64 doesn't work correctly in that
    case, removing the need to add code to handle the problem.
  */
  if (org_tty_set)
    tcsetattr(STDIN_FILENO, TCSANOW, &org_tty);
#endif

#ifdef  FORK_IPC
  process->stopDispatcher();
  waitpid(pid, NULL, 0);
#endif

  return exitStatus;
}
