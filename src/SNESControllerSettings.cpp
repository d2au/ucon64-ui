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
#include "SNESControllerSettings.h"
#include "Frontend.h"


FXDEFMAP(SNESControllerSettings) SNESControllerSettingsMap[] =
{
  FXMAPFUNC(SEL_COMMAND, SNESControllerSettings::ID_USESETTINGS, SNESControllerSettings::onUseSettings),
  FXMAPFUNC(SEL_COMMAND, SNESControllerSettings::ID_OK, SNESControllerSettings::onOK)
};

FXIMPLEMENT(SNESControllerSettings, FXDialogBox, SNESControllerSettingsMap, ARRAYNUMBER(SNESControllerSettingsMap))


SNESControllerSettings::SNESControllerSettings(FXWindow *a_owner,
    const FXString &a_name, FXuint a_opts, FXint a_x, FXint a_y,
    FXint a_w, FXint a_h, FXint a_pl, FXint a_pr,
    FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXDialogBox(a_owner, a_name, a_opts, a_x, a_y, a_w, a_h,
        a_pl, a_pr, a_pt, a_pb, a_hs, a_vs)
{
  FXVerticalFrame *windowArea = new FXVerticalFrame(this, LAYOUT_FILL,
    0, 0, 0, 0);

  m_useSettings = new FXCheckButton(windowArea,
    "Store controller settings in NSRT header", this, ID_USESETTINGS);
  FXLabel *note = new FXLabel(windowArea,
    "Note: An NSRT header also stores the internal name and region code", NULL,
    LABEL_NORMAL, 0, 0, 0, 0, DEFAULT_PAD, DEFAULT_PAD, DEFAULT_PAD, 10);
  note->setFont(((Frontend *) getApp())->getSmallFont());

  FXMatrix *controllerArea = new FXMatrix(windowArea, 2,
     MATRIX_BY_COLUMNS, 0, 0, 0, 0, 0, 0, DEFAULT_PAD, DEFAULT_PAD);

  m_controller1Label = new FXLabel(controllerArea, "Port 1 controller type:");
  m_controller1 = new ComboBox(controllerArea, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_controller1->appendItem("gamepad");
  m_controller1->appendItem("mouse");
  m_controller1->appendItem("mouse / gamepad");
  m_controller1->appendItem("multitap");
  m_controller1->setNumVisible(4);

  m_controller2Label = new FXLabel(controllerArea, "Port 2 controller type:");
  m_controller2 = new ComboBox(controllerArea, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_controller2->appendItem("gamepad");
  m_controller2->appendItem("mouse");
  m_controller2->appendItem("mouse / gamepad");
  m_controller2->appendItem("super scope");
  m_controller2->appendItem("super scope / gamepad");
  m_controller2->appendItem("Konami's justifier");
  m_controller2->appendItem("multitap");
  m_controller2->appendItem("mouse / super scope / gamepad");
  m_controller2->setNumVisible(8);

  new FXButton(windowArea, "OK", NULL, this, ID_OK,
    BUTTON_NORMAL | LAYOUT_CENTER_X, 0, 0, 0, 0, 10, 10);

  handle(m_useSettings, FXSEL(SEL_COMMAND, ID_USESETTINGS), (void *) 0);
}


SNESControllerSettings::~SNESControllerSettings()
{
  delete m_useSettings;
  delete m_controller1Label;
  delete m_controller1;
  delete m_controller2Label;
  delete m_controller2;
}


long
SNESControllerSettings::onUseSettings(FXObject *, FXSelector, void *a_data)
{
  if ((int) a_data)
    {
      m_controller1Label->enable();
      m_controller1->enable();
      m_controller2Label->enable();
      m_controller2->enable();
    }
  else
    {
      m_controller1Label->disable();
      m_controller1->disable();
      m_controller2Label->disable();
      m_controller2->disable();
    }
  return 1;
}


long
SNESControllerSettings::onOK(FXObject *, FXSelector, void *)
{
  getApp()->stopModal(this, TRUE);
  hide();
  return 1;
}


FXbool
SNESControllerSettings::useSettings(void)
{
  return m_useSettings->getCheck();
}


int
SNESControllerSettings::getController1Type(void)
{
  int item = m_controller1->getCurrentItem();
  // multitap is item 3 in the combobox, but has code 6
  return item == 3 ? 6 : item;
}


int
SNESControllerSettings::getController2Type(void)
{
  return m_controller2->getCurrentItem();
}
