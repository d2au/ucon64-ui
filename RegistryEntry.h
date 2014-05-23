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
#ifndef REGISTRYENTRY_H
#define REGISTRYENTRY_H

#include <FXRegistry.h>
#include <FXString.h>


class RegistryEntry
{
  friend class TextFieldOption;
public:
  RegistryEntry(FXRegistry *registry, char *section, char *key, char *def);
  virtual ~RegistryEntry();
  const FXchar *readStringEntry(void);
  void writeStringEntry(const FXchar *value);

protected:
  RegistryEntry() {}
  FXRegistry *m_registry;
  char *m_section, *m_key, *m_default;
};

#endif
