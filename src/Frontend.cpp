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
#include <stdlib.h>
#include <fx.h>
#include "Frontend.h"
#include "MainWindow.h"
#include "icons.h"


Frontend::Frontend(const FXString &a_name, const FXString &a_vendor)
  : FXApp(a_name, a_vendor)
{
  m_mainWindow = NULL;

  m_smallIcon = new FXGIFIcon(this, icon16, FXRGB(255, 255, 255), IMAGE_ALPHACOLOR);
  m_bigIcon = new FXGIFIcon(this, icon32, FXRGB(255, 255, 255), IMAGE_ALPHACOLOR);
  m_veryBigIcon = new FXGIFIcon(this, icon64, FXRGB(0, 0, 0), IMAGE_ALPHACOLOR);
  FXColor *noIconData = new FXColor[16 * 16];
  if (!noIconData)
    {
      fputs("ERROR: Not enough memory for icon\n", stderr);
      exit(1);
    }
  for (int n = 0; n < 16 * 16; n++)
    noIconData[n] = FXRGB(0, 0, 0);
  m_noIcon = new FXIcon(this, noIconData, FXRGB(0, 0, 0), IMAGE_ALPHACOLOR, 16, 16);
}


Frontend::~Frontend()
{
}


void
Frontend::init(int &a_argc, char **a_argv, FXbool a_connect)
{
  reg().setAsciiMode(TRUE);
  FXApp::init(a_argc, a_argv, a_connect);
}


void
Frontend::create()
{
  createFonts();                                // should *not* be called in the c'tor
  FXString mainTitle = FXStringFormat("uCON64 frontend %d.%d", UF_MAJOR, UF_MINOR);
  m_mainWindow = new MainWindow(this, mainTitle, m_bigIcon, m_smallIcon,
    DECOR_ALL & ~DECOR_SHRINKABLE);
  FXApp::create();                              // must be called *after*
  m_mainWindow->setFocus();                     //  creating our windows
}


void
Frontend::exit(FXint)
{
  m_mainWindow->writeToRegistry();
  // an FXMainWindow deletes itself
  m_mainWindow = NULL;
  FXApp::exit();                                // calls reg().write()
}


void
Frontend::createFonts(void)
/*
  For some reason this function doesn't work correctly when called from the
  constructor. Under Windows XP the fonts won't be set correctly. Under Linux
  there's no problem.
*/
{
  // FOX (<= 1.2) defaults to an ugly bold font under X
  FXFont *font = getNormalFont();
  FXFontDesc fontDesc;
  font->getFontDesc(fontDesc);
/*
  printf("face: %s; size: %d; weight: %d; slant: %d; setwidth: %d; encoding: %d; flags: %d\n",
         fontDesc.face, fontDesc.size, fontDesc.weight, fontDesc.slant,
         fontDesc.setwidth, fontDesc.encoding, fontDesc.flags);
  fflush(stdout);
*/
  fontDesc.weight = FXFont::Medium; // FONTWEIGHT_REGULAR
  font = new FXFont(this, fontDesc);
  setNormalFont(font);

  fontDesc.size -= 10;
  m_smallFont = new FXFont(this, fontDesc);

  fontDesc.size -= 10;
  m_tinyFont = new FXFont(this, fontDesc);

  fontDesc.size += 20;
  fontDesc.weight = FXFont::Bold;
  m_boldFont = new FXFont(this, fontDesc);
}
