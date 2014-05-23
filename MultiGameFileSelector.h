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
#ifndef MULTIGAMEFILESELECTOR_H
#define MULTIGAMEFILESELECTOR_H

#include <stdio.h>
#include <fx.h>


class MultiGameFileSelector : public FXDialogBox//FXTopWindow
{
  FXDECLARE(MultiGameFileSelector)
public:
  MultiGameFileSelector(FXWindow *owner, int blockSize,
      const FXString &name, FXIcon *icon = NULL, FXIcon *icon2 = NULL,
      FXuint opts = DECOR_ALL & ~DECOR_SHRINKABLE,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = 0, FXint pr = 0, FXint pt = 0, FXint pb = 0,
      FXint hs = 0, FXint vs = 0);
  virtual ~MultiGameFileSelector();
//  int execute(FXuint placement = PLACEMENT_OWNER); //PLACEMENT_CURSOR
  long onAddFile(FXObject *src, FXSelector selector, void *data);
  long onRemoveFile(FXObject *src, FXSelector selector, void *data);
  long onCreate(FXObject *src, FXSelector selector, void *data);
//  long onClose(FXObject *src, FXSelector selector, void *data);

  enum
    {
//      ID_ADDFILE = FXTopWindow::ID_LAST,
      ID_ADDFILE = FXDialogBox::ID_LAST,
      ID_REMOVEFILE,
      ID_CREATE,
      ID_LAST
    };

protected:
  MultiGameFileSelector() {}

  FileSelector *m_fileSelector;
  FXButton *m_removeButton;
  FXList *m_selectedFiles;
  FXLabel *m_totalSizeLabel;
  int m_totalSize, m_blockSize;
};

#endif
