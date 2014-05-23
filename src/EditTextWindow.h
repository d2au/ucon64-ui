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
#ifndef EDITTEXTWINDOW_H
#define EDITTEXTWINDOW_H

#include <stdio.h>
#include <fx.h>


class EditTextWindow : public FXTopWindow
{
  FXDECLARE(EditTextWindow)
public:
  EditTextWindow(FXWindow *owner, const FXString &name, char *filename,
      FXIcon *icon = NULL, FXIcon *icon2 = NULL, FXuint opts = DECOR_ALL,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = 0, FXint pr = 0, FXint pt = 0, FXint pb = 0,
      FXint hs = 0, FXint vs = 0);
  virtual ~EditTextWindow();
  long onSave(FXObject *src, FXSelector selector, void *data);
  long onClose(FXObject *src, FXSelector sel, void *data);

  enum
    {
      ID_SAVE = FXTopWindow::ID_LAST,
      ID_LAST
    };

protected:
  EditTextWindow() {}

  char m_filename[FILENAME_MAX];
  FXText *m_contents;
};

#endif
