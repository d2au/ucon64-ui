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
#ifndef TEXTFIELDOPTION_H
#define TEXTFIELDOPTION_H

#include <fx.h>
#include "RegistryEntry.h"


class TextFieldOption : public FXObject
{
  FXDECLARE(TextFieldOption)
public:
  TextFieldOption(FXTextField *textField, RegistryEntry *registryEntry,
      FXCheckButton *useTextField = NULL, FXButton *changeTextField = NULL);
  virtual ~TextFieldOption();
  void setLayoutHints(FXuint layoutHints);
  FXString getText(void);
  void setText(const FXString &text);
  FXbool enabled(void);
  void writeToRegistry(void);
  static TextFieldOption *add(FXComposite *parent, char *label, int nColumns,
      bool addChangeButton, FXObject *changeButtonTarget, int changeButtonID,
      bool addCheckButton, char *section, char *key, char *def);
  long onOverride(FXObject *src, FXSelector selector, void *data);
  FXButton *getChangeButton(void) { return m_changeTextField; }
  FXCheckButton *getCheckButton(void) { return m_useTextField; }

  enum
    {
      ID_OVERRIDE,
      ID_LAST
    };

protected:
  TextFieldOption() {}
  FXTextField *m_textField;
  FXButton *m_changeTextField;
  FXCheckButton *m_useTextField;
  RegistryEntry *m_regTextField, *m_regCheckButton;
};

#endif
