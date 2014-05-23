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
#include <fx.h>
#include "FileDialog.h"
#include "Frontend.h"


FileDialog::FileDialog(FXWindow *a_owner, const FXString &a_name, FXuint a_opts,
    FXint a_x, FXint a_y, FXint a_w, FXint a_h)
  : FXFileDialog(a_owner, a_name, a_opts, a_x, a_y, a_w, a_h)
{
#if 1
  // Windows: If there's no close button in the title bar then there won't be a
  //  default icon, which is what I want
  options &= ~DECOR_CLOSE;
#else
  setMiniIcon(((Frontend *) getApp())->m_noIcon);
#endif

#if   (FOX_MAJOR * 10 + FOX_MINOR) >= 13
  allowPatternEntry(TRUE);
#endif
}


// the following three functions are copied from FXFileDialog.cpp of FOX 1.2.9
FXString
FileDialog::getOpenFilename(FXWindow *a_owner, const FXString &a_caption,
    const FXString &a_path, const FXString &a_patterns, FXint a_initial)
{
  FileDialog opendialog(a_owner, a_caption);
  FXString filename;
  opendialog.setSelectMode(SELECTFILE_EXISTING);
  opendialog.setFilename(a_path);
  opendialog.setPatternList(a_patterns);
  opendialog.setCurrentPattern(a_initial);
  if (opendialog.execute())
    {
      filename = opendialog.getFilename();
      if (FXStat::isFile(filename))
        return filename;
    }
  return FXString::null;
}


FXString
FileDialog::getSaveFilename(FXWindow *a_owner, const FXString &a_caption,
    const FXString &a_path, const FXString &a_patterns, FXint a_initial)
{
  FileDialog savedialog(a_owner, a_caption);
  savedialog.setSelectMode(SELECTFILE_ANY);
  savedialog.setFilename(a_path);
  savedialog.setPatternList(a_patterns);
  savedialog.setCurrentPattern(a_initial);
  if (savedialog.execute())
    return savedialog.getFilename();
  return FXString::null;
}


FXString
FileDialog::getOpenDirectory(FXWindow *a_owner, const FXString &a_caption,
    const FXString &a_path)
{
  FileDialog dirdialog(a_owner, a_caption);
  FXString dirname;
  dirdialog.setSelectMode(SELECTFILE_DIRECTORY);
  dirdialog.setFilename(a_path);
  if(dirdialog.execute())
    {
      dirname = dirdialog.getFilename();
      if (FXStat::isDirectory(dirname))
        return dirname;
    }
  return FXString::null;
}
