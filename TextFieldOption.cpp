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
#include <string.h>
#include <fx.h>
#include "RegistryEntry.h"
#include "TextFieldOption.h"


FXDEFMAP(TextFieldOption) TextFieldOptionMap[] =
{
  FXMAPFUNC(SEL_COMMAND, TextFieldOption::ID_OVERRIDE, TextFieldOption::onOverride)
};

FXIMPLEMENT(TextFieldOption, FXObject, TextFieldOptionMap, ARRAYNUMBER(TextFieldOptionMap))


TextFieldOption::TextFieldOption(FXTextField *a_textField, RegistryEntry *a_regEntry,
    FXCheckButton *a_useTextField, FXButton *a_changeTextField)
{
  m_textField = a_textField;
  m_regTextField = a_regEntry;
  m_useTextField = a_useTextField;
  m_changeTextField = a_changeTextField;

  m_textField->setText(m_regTextField->readStringEntry());
  if (m_useTextField)
    {
      char *key = new char[80];
      strcpy(key, "Use");
      strcat(key, m_regTextField->m_key);
      m_regCheckButton = new RegistryEntry(m_regTextField->m_registry,
        m_regTextField->m_section, key, "no");

      if (strcmp(m_regCheckButton->readStringEntry(), "yes") == 0)
        m_useTextField->setCheck();     // default state of FXCheckButton is unchecked

      if (!m_useTextField->getCheck())
        {
          m_textField->disable();       // default state of FXTextField is enabled
          if (m_changeTextField)
            m_changeTextField->disable();
        }

      m_useTextField->setTarget(this);
      m_useTextField->setSelector(ID_OVERRIDE);
    }
  else
    m_regCheckButton = NULL;
}


TextFieldOption::~TextFieldOption()
{
/* TODO: Do something sensible here
  m_textField = NULL;
  m_regTextField = NULL;
  m_useTextField = NULL;
  m_regCheckButton = NULL;
*/
}


void
TextFieldOption::setLayoutHints(FXuint a_layoutHints)
{
  m_textField->setLayoutHints(a_layoutHints);
}


TextFieldOption *
TextFieldOption::add(FXComposite *a_parent, char *a_label, int a_nColumns,
    bool a_addChangeButton, FXObject *a_changeButtonTarget, int a_changeButtonID,
    bool a_addCheckButton, char *a_section, char *a_key, char *a_default)
{
  new FXLabel(a_parent, a_label);

  FXTextField *textField = new FXTextField(a_parent, a_nColumns, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);

  FXButton *button;
  if (a_addChangeButton)
    button = new FXButton(a_parent, "Change", NULL, a_changeButtonTarget,
      a_changeButtonID);
  else
    {
      new FXLabel(a_parent, "");                // spacer (for OptionsWindow)
      button = NULL;
    }

  FXCheckButton *checkButton;
  if (a_addCheckButton)
    checkButton = new FXCheckButton(a_parent, "", NULL, 0,
      CHECKBUTTON_NORMAL | LAYOUT_CENTER_X);
  else
    checkButton = NULL;

  return new TextFieldOption(textField,
    new RegistryEntry(&a_parent->getApp()->reg(), a_section, a_key, a_default),
      checkButton, button);
}


FXString
TextFieldOption::getText(void)
{
  return m_textField->getText();
}


void
TextFieldOption::setText(const FXString &a_text)
{
  m_textField->setText(a_text);
}


FXbool
TextFieldOption::enabled(void)
{
  if (m_useTextField)
    return m_useTextField->getCheck();
  else
    return TRUE;
}


void
TextFieldOption::writeToRegistry(void)
{
  m_regTextField->writeStringEntry(m_textField->getText().text());
  if (m_useTextField)
    m_regCheckButton->writeStringEntry(m_useTextField->getCheck() ? "yes" : "no");
}


long
TextFieldOption::onOverride(FXObject *, FXSelector, void *)
{
  if (m_useTextField->getCheck())
    {
      m_textField->enable();
      if (m_changeTextField)
        m_changeTextField->enable();
    }
  else
    {
      m_textField->disable();
      if (m_changeTextField)
        m_changeTextField->disable();
    }
  return 1;
}
