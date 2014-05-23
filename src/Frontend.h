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
#ifndef FRONTEND_H
#define FRONTEND_H

#include <fx.h>
#if   (FOX_MAJOR * 10 + FOX_MINOR) < 12
#error uf needs FOX version 1.2 or later
#endif

#ifdef  __unix__ // Tip text works alright under Windows with FOX 1.3.21
#if   (FOX_MAJOR * 1000 + FOX_MINOR * 100 + FOX_LEVEL) == 1321
#ifdef  __GNUC__
#warning Tip text does not work with FOX 1.3.21
#else
#pragma message ("Tip text does not work with FOX 1.3.21")
#endif
#endif // FOX 1.3.21
#endif // __unix__

#include "MainWindow.h"
#include "CommandLineHandler.h"


#define UF_MAJOR 1
#define UF_MINOR 1


class Frontend : public FXApp
{
public:
  Frontend(const FXString &name = "", const FXString &vendor = "");
  virtual ~Frontend();
  virtual void init(int &argc, char **argv, FXbool connect = TRUE);
  virtual void create();
  virtual void exit(FXint code = 0);
  MainWindow *getMainWindow(void) { return m_mainWindow; }
  FXFont *getSmallFont() { return m_smallFont; }
  FXFont *getTinyFont() { return m_tinyFont; }
  FXFont *getBoldFont() { return m_boldFont; }

  FXGIFIcon *m_smallIcon, *m_bigIcon, *m_veryBigIcon;
  FXIcon *m_noIcon;

protected:
  void createFonts(void);

  MainWindow *m_mainWindow;
  FXFont *m_smallFont, *m_tinyFont, *m_boldFont;
};

#endif
