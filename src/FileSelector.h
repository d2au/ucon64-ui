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
#ifndef FILESELECTOR_H
#define FILESELECTOR_H

#include <fx.h>


enum
  {
    SELECTFILE_NOOKCANCEL = 1 << 24,
    SELECTFILE_NOENTRYBLOCK = 2 << 24,
    SELECTFILE_SMALLDIRBOX = 4 << 24
  };

class FileSelector : public FXFileSelector
{
public:
  FileSelector(FXComposite *p, FXObject *target = NULL, FXSelector sel = 0,
      FXuint opts = 0, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
  void clearFilenames(void);
  void setNumVisibleFilter(FXint num);
#if   (FOX_MAJOR * 10 + FOX_MINOR) < 13
  void allowPatternEntry(FXbool allow);
#endif

protected:
  FileSelector() {}
};

#endif
