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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "FileSelector.h"
#include "OptionsWindow.h"
#include "RegistryEntry.h"
#include "snes.h"
#include "genesis.h"
#include "gb.h"
#include "gba.h"
#include "n64.h"
#include "nes.h"
#include "sms.h"
#include "pce.h"
#include "ngp.h"
#include "misc.h"


class MainWindow : public FXMainWindow
{
  FXDECLARE(MainWindow)
public:
  MainWindow(FXApp *app, const FXString &name, FXIcon *icon = NULL,
      FXIcon *icon2 = NULL, FXuint opts = DECOR_ALL, FXint x = 0, FXint y = 0,
      FXint w = 0, FXint h = 0, FXint pl = 0, FXint pr = 0,
      FXint pt = 0, FXint pb = 0, FXint hs = 0, FXint vs = 0);
  virtual ~MainWindow();
  virtual void create();
  void updateTitle(bool showSelectedFilesNo);
  FXString *getSelectedFilenames(void);
  void clearSelectedFilenames(void);
  void selectOutputTab(void);
  void enableAbortButton(void);
  void disableAbortButton(void);
  long onResetParPort(FXObject *src, FXSelector sel, void *data);
  long onOutputDir(FXObject *src, FXSelector sel, void *data);
  long onSelectFile(FXObject *src, FXSelector sel, void *data);
  long onAbout(FXObject *src, FXSelector sel, void *data);
  long onOptions(FXObject *src, FXSelector sel, void *data);
  long onCloseOptionsWindow(FXObject *src, FXSelector selector, void *data);
  void writeToRegistry(void);
  CommandLineHandler *getCommandLineHandler(void) { return m_commandLineHandler; }
  FXText *getOutputText(void) { return m_outputText; }
  OptionsWindow *getOptionsWindow(void) { return m_optionsWindow; }
  FXTextField *getOutputDir(void) { return m_outputDir; }
  FXTabBook *getTabBook(void) { return m_tabBook; }

  enum
    {
      ID_RESETPARPORT = FXMainWindow::ID_LAST,
      ID_OUTPUTDIR,
      ID_SELECTFILE,
      ID_ABOUT,
      ID_OPTIONS,
      ID_LAST
    };
  enum
    {
      TAB_BROWSE = 0,
      TAB_SNES = 2,
      TAB_GENESIS = 4,
      TAB_GB = 6,
      TAB_GBA = 8,
      TAB_N64 = 10,
      TAB_NES = 12,
      TAB_SMS = 14,
      TAB_PCE = 16,
      TAB_NGP = 18,
      TAB_MISC = 20,
      TAB_OUTPUT = 22
    };

protected:
  MainWindow() {}
  FXFont *searchFont(char *name, int preferredSize);

  FXMenuPane *m_programMenu;                    // has to me a member, or else a
  FXTabBook *m_tabBook;                         //  segmentation fault occurs at exit
  FXText *m_outputText;
  FXButton *m_abortButton;
  FXTextField *m_outputDir;
  RegistryEntry *m_regOutputDir;
  FileSelector *m_fileSelector;
  CommandLineHandler *m_commandLineHandler;
  OptionsWindow *m_optionsWindow;
  SNESFrame *m_SNESFrame;
  GenesisFrame *m_GenesisFrame;
  GBFrame *m_GBFrame;
  GBAFrame *m_GBAFrame;
  N64Frame *m_N64Frame;
  NESFrame *m_NESFrame;
  SMSFrame *m_SMSFrame;
  PCEFrame *m_PCEFrame;
  NGPFrame *m_NGPFrame;
  MiscFrame *m_miscFrame;
  int m_nSelectedFiles;
};

#else
class MainWindow;
#endif
