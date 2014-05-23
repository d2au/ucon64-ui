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
#include "FileSelector.h"


FileSelector::FileSelector(FXComposite *a_p, FXObject *a_target, FXSelector a_sel,
    FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h)
  : FXFileSelector(a_p, a_target, a_sel, a_opts, a_x, a_y, a_w, a_h)
{
  FXWindow *child;

  // disable default hot-keys...
  child = entryblock->childAtRowCol(0, 0);      // label
  ((FXLabel *) child)->setText("File Name:");   // capital N, because FXFileDialog does the same...
  child = entryblock->childAtRowCol(1, 0);      // label
  ((FXLabel *) child)->setText("File Filter:"); // capital F, because FXFileDialog does the same...

  if (a_opts & SELECTFILE_NOOKCANCEL)
    {
      child = entryblock->childAtRowCol(0, 2);  // button
      child->hide();
      cancel->hide();
    }
  if (a_opts & SELECTFILE_NOENTRYBLOCK)
    entryblock->hide();
  if (a_opts & SELECTFILE_SMALLDIRBOX)
    {
      child = navbuttons->childAtIndex(1);      // label
      child->hide();
#ifdef  __unix__ // without a dirbox it's impossible (under Windows) to reach another drive
      dirbox->hide();
#endif
    }
}


void
FileSelector::clearFilenames(void)
{
  int n = filebox->getNumItems();
  for (int i = 0; i < n; i++)
    filebox->deselectItem(i);
}


void
FileSelector::setNumVisibleFilter(FXint a_num)
{
  filefilter->setNumVisible(a_num);
}


#if   (FOX_MAJOR * 10 + FOX_MINOR) < 13
void
FileSelector::allowPatternEntry(FXbool a_allow)
{
  filefilter->setComboStyle(a_allow ? COMBOBOX_NORMAL : COMBOBOX_STATIC);
}
#endif
