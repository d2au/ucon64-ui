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
#ifndef OPTIONSWINDOW_H
#define OPTIONSWINDOW_H

#include "TextFieldOption.h"
#include "CheckButtonOption.h"
#include "MainWindow.h"


class OptionsWindow : public FXTopWindow
{
  FXDECLARE(OptionsWindow)
public:
  OptionsWindow(FXWindow *owner, const FXString &name, FXIcon *icon = NULL,
      FXIcon *icon2 = NULL, FXuint opts = DECOR_ALL,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = 0, FXint pr = 0, FXint pt = 0, FXint pb = 0,
      FXint hs = 0, FXint vs = 0);
  virtual ~OptionsWindow();
  void addFiles(void);
  long onuCON64Path(FXObject *src, FXSelector selector, void *data);
  long onConfigFileDir(FXObject *src, FXSelector selector, void *data);
  long onConfigDir(FXObject *src, FXSelector selector, void *data);
  long onDatDir(FXObject *src, FXSelector selector, void *data);
  long onDiscmagePath(FXObject *src, FXSelector selector, void *data);
  long onBackup(FXObject *src, FXSelector selector, void *data);
  long onUseBackup(FXObject *src, FXSelector selector, void *data);
  long onGBALoaderPath(FXObject *src, FXSelector selector, void *data);
  long onF2A(FXObject *src, FXSelector selector, void *data);
  long onEditConfigFile(FXObject *src, FXSelector selector, void *data);
  long onBrowse(FXObject *src, FXSelector selector, void *data);
  long onSNES(FXObject *src, FXSelector selector, void *data);
  long onGenesis(FXObject *src, FXSelector selector, void *data);
  long onGB(FXObject *src, FXSelector selector, void *data);
  long onGBA(FXObject *src, FXSelector selector, void *data);
  long onN64(FXObject *src, FXSelector selector, void *data);
  long onNES(FXObject *src, FXSelector selector, void *data);
  long onSMS(FXObject *src, FXSelector selector, void *data);
  long onPCE(FXObject *src, FXSelector selector, void *data);
  long onNGP(FXObject *src, FXSelector selector, void *data);
  long onMisc(FXObject *src, FXSelector selector, void *data);
  long onTipText(FXObject *src, FXSelector selector, void *data);
  long onTest(FXObject *src, FXSelector selector, void *data);
  long onSave(FXObject *src, FXSelector selector, void *data);
  TextFieldOption *getuCON64Path(void) { return m_uCON64Path; }
  TextFieldOption *getConfigFileDir(void) { return m_configFileDir; }
  TextFieldOption *getConfigDir(void) { return m_configDir; }
  TextFieldOption *getDatDir(void) { return m_datDir; }
  TextFieldOption *getDiscmagePath(void) { return m_discmagePath; }
  TextFieldOption *getParPortAddress(void) { return m_parPortAddress; }
  TextFieldOption *getParPortDevice(void) { return m_parPortDevice; }
  TextFieldOption *getGBALoaderPath(void) { return m_GBALoaderPath; }
  TextFieldOption *getF2AFirmwarePath(void) { return m_F2AFirmwarePath; }
  TextFieldOption *getF2AClientUSBPath(void) { return m_F2AClientUSBPath; }
  TextFieldOption *getF2AClientParPortPath(void) { return m_F2AClientParPortPath; }
  TextFieldOption *getF2ALogoPath(void) { return m_F2ALogoPath; }
  FXbool useBackup(void) { return m_useBackup->getCheck(); }
  FXbool backupEnabled(void) { return m_backupYes->getCheck(); }
  FXbool useBrowseTab(void) { return m_BrowseTab->enabled(); }
  FXbool useSNESTab(void) { return m_SNESTab->enabled(); }
  FXbool useGenesisTab(void) { return m_GenesisTab->enabled(); }
  FXbool useGBTab(void) { return m_GBTab->enabled(); }
  FXbool useGBATab(void) { return m_GBATab->enabled(); }
  FXbool useN64Tab(void) { return m_N64Tab->enabled(); }
  FXbool useNESTab(void) { return m_NESTab->enabled(); }
  FXbool useSMSTab(void) { return m_SMSTab->enabled(); }
  FXbool usePCETab(void) { return m_PCETab->enabled(); }
  FXbool useNGPTab(void) { return m_NGPTab->enabled(); }
  FXbool useMiscTab(void) { return m_miscTab->enabled(); }
  FXbool runInFrontendMode(void) { return m_runInFrontendMode->enabled(); }
  FXbool useIntermediaryFiles(void) { return m_useIntermediaryFiles->enabled(); }

  enum
    {
      ID_CLOSE = FXTopWindow::ID_LAST,
      ID_UCON64_PATH,
      ID_CONFIGFILE_DIR,
      ID_CONFIG_DIR,
      ID_DAT_DIR,
      ID_DISCMAGE_PATH,
      ID_BACKUP,
      ID_USEBACKUP,
      ID_GBALOADER_PATH,
      ID_F2A,
      ID_EDIT_CONFIGFILE,
      ID_BROWSE,
      ID_SNES,
      ID_GENESIS,
      ID_GB,
      ID_GBA,
      ID_N64,
      ID_NES,
      ID_SMS,
      ID_PCE,
      ID_NGP,
      ID_MISC,
      ID_TIPTEXT,
      ID_TEST,
      ID_SAVE,
      ID_LAST
    };

protected:
  OptionsWindow() {}

  MainWindow *m_mainWindow;
  FXRegistry *m_registry;
  TextFieldOption *m_uCON64Path, *m_configFileDir, *m_configDir, *m_datDir,
    *m_discmagePath, *m_parPortAddress, *m_parPortDevice, *m_GBALoaderPath,
    *m_F2AFirmwarePath, *m_F2AClientUSBPath, *m_F2AClientParPortPath,
    *m_F2ALogoPath;
  CheckButtonOption *m_BrowseTab, *m_SNESTab, *m_GenesisTab, *m_GBTab,
    *m_GBATab, *m_N64Tab, *m_NESTab, *m_SMSTab, *m_PCETab, *m_NGPTab,
    *m_miscTab, *m_runInFrontendMode, *m_useIntermediaryFiles, *m_enableTipText;
  FXRadioButton *m_backupYes, *m_backupNo;
  FXCheckButton *m_useBackup;
  RegistryEntry *m_regBackup, *m_regUseBackup;
  FXToolTip *m_toolTip;
};

#else
class OptionsWindow;
#endif
