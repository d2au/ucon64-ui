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
#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <fx.h>


// The original reason for this class is to have a file dialog without a
//  default Windows icon...
class FileDialog : public FXFileDialog
{
public:
  FileDialog(FXWindow *owner, const FXString &name, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 500, FXint h = 300);
  static FXString getOpenFilename(FXWindow *owner, const FXString &caption,
      const FXString &path, const FXString &patterns = "*", FXint initial = 0);
  static FXString getSaveFilename(FXWindow *owner, const FXString &caption,
      const FXString &path, const FXString &patterns = "*", FXint initial = 0);
  static FXString getOpenDirectory(FXWindow *owner, const FXString &caption,
      const FXString &path);

protected:
  FileDialog() {}
};

#endif
