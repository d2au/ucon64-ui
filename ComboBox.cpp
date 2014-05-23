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
#include <fx.h>
#include "ComboBox.h"


FXDEFMAP(ComboBox) ComboBoxMap[] =
{
  FXMAPFUNC(SEL_UPDATE, ComboBox::ID_SELF, ComboBox::onUpdate),
  FXMAPFUNC(SEL_COMMAND, ComboBox::ID_SELF, ComboBox::onCommand)
};

FXIMPLEMENT(ComboBox, FXComboBox, ComboBoxMap, ARRAYNUMBER(ComboBoxMap))


ComboBox::ComboBox(FXComposite *a_p, FXint a_cols, FXObject *a_target,
    FXSelector a_selector, FXuint a_opts, FXint a_x, FXint a_y,
    FXint a_w, FXint a_h, FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb)
  : FXComboBox(a_p, a_cols, a_target, a_selector, a_opts, a_x, a_y, a_w, a_h,
        a_pl, a_pr, a_pt, a_pb)
{
  setTarget(this);
  setSelector(ID_SELF);
  m_target = a_target;
  m_selector = a_selector;                      // ID
  m_currentItem = -1;
}


long
ComboBox::onUpdate(FXObject *, FXSelector, void *)
/*
  We have to add this handler for an FXComboBox, because its current item isn't
  necessarily shown. Instead of copying this handler for every instance of
  FXComboBox, we create a derived class, hence this class.
*/
{
  FXint currentItem = getCurrentItem();
  if (m_currentItem != currentItem)
    {
      setCurrentItem(currentItem);
      m_currentItem = currentItem;
      // Send a SEL_COMMAND so that the internal state of the program will match
      //  with what is displayed
      FXString item = getItem(m_currentItem); // send item text for correctness
      if (m_target)
        m_target->handle(this, FXSEL(SEL_COMMAND, m_selector), (void *) item.text()); // cast const away
    }
  return 1;
}


long
ComboBox::onCommand(FXObject *, FXSelector, void *a_data)
{
  m_currentItem = getCurrentItem();     // avoid another indirect SEL_COMMAND call
  if (m_target)                         //  (from ComboBox::onUpdate())
    m_target->handle(this, FXSEL(SEL_COMMAND, m_selector), a_data);
  return 1;
}
