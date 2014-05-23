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
#include <string.h>
#include <fx.h>
#include "RegistryEntry.h"
#include "CheckButtonOption.h"


CheckButtonOption::CheckButtonOption(FXCheckButton *a_checkButton,
    RegistryEntry *a_regEntry)
{
  m_checkButton = a_checkButton;
  m_regEntry = a_regEntry;

  if (strcmp(m_regEntry->readStringEntry(), "yes") == 0)
    m_checkButton->setCheck();          // default state of FXCheckButton is unchecked
}


CheckButtonOption::~CheckButtonOption()
{
  delete m_checkButton;
  delete m_regEntry;
}


FXbool
CheckButtonOption::enabled(void)
{
  return m_checkButton->getCheck();
}


void
CheckButtonOption::writeToRegistry(void)
{
  m_regEntry->writeStringEntry(m_checkButton->getCheck() ? "yes" : "no");
}
