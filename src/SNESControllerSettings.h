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
#ifndef SNESCONTROLLERSETTINGS_H
#define SNESCONTROLLERSETTINGS_H

#include <fx.h>
#include "ComboBox.h"


class SNESControllerSettings : public FXDialogBox
{
  FXDECLARE(SNESControllerSettings)
public:
  SNESControllerSettings(FXWindow *owner,
      const FXString &name, FXuint opts = DECOR_TITLE | DECOR_BORDER,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = 0, FXint pr = 0, FXint pt = 0, FXint pb = 0,
      FXint hs = 0, FXint vs = 0);
  virtual ~SNESControllerSettings();
  long onUseSettings(FXObject *src, FXSelector selector, void *data);
  long onOK(FXObject *src, FXSelector selector, void *data);
  FXbool useSettings(void);
  int getController1Type(void);
  int getController2Type(void);

  enum
    {
      ID_USESETTINGS = FXDialogBox::ID_LAST,
      ID_OK,
      ID_LAST
    };

protected:
  SNESControllerSettings() {}

  FXCheckButton *m_useSettings;
  FXLabel *m_controller1Label, *m_controller2Label;
  ComboBox *m_controller1, *m_controller2;
};

#endif
