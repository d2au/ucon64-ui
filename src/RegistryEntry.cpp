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


RegistryEntry::RegistryEntry(FXRegistry *a_registry, char *a_section,
    char *a_key, char *a_default)
{
  m_registry = a_registry;
  m_section = a_section;
  m_key = a_key;
  m_default = a_default;
}


RegistryEntry::~RegistryEntry()
{
  m_registry = NULL;
  m_section = NULL;
  m_key = NULL;
  m_default = NULL;
}


const FXchar *
RegistryEntry::readStringEntry(void)
{
  return m_registry->readStringEntry(m_section, m_key, m_default);
}


void
RegistryEntry::writeStringEntry(const FXchar *a_value)
{
  m_registry->writeStringEntry(m_section, m_key, a_value);
}
