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
#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <fx.h>


class ComboBox : public FXComboBox
{
  FXDECLARE(ComboBox)
public:
  ComboBox(FXComposite *p, FXint cols, FXObject *tgt = NULL, FXSelector sel = 0,
      FXuint opts = COMBOBOX_NORMAL, FXint x = 0, FXint y = 0,
      FXint w = 0, FXint h = 0, FXint pl = DEFAULT_PAD, FXint pr = DEFAULT_PAD,
      FXint pt = DEFAULT_PAD, FXint pb = DEFAULT_PAD);
  long onUpdate(FXObject *, FXSelector, void *);
  long onCommand(FXObject *, FXSelector, void *);

  enum
    {
      ID_SELF = FXComboBox::ID_LAST,
      ID_LAST
    };

protected:
  ComboBox() {}
  FXint m_currentItem;
  FXObject *m_target;
  FXSelector m_selector;
};

#endif
