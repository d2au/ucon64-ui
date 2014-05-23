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
#include <fx.h>
#include "OptionsWindow.h"
#include "TextFieldOption.h"
#include "CheckButtonOption.h"
#include "RegistryEntry.h"
#include "MainWindow.h"
#include "CommandLineHandler.h"
#include "EditTextWindow.h"
#include "Frontend.h"
#include "FileDialog.h"


FXDEFMAP(OptionsWindow) OptionsWindowMap[] =
{
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_UCON64_PATH, OptionsWindow::onuCON64Path),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_CONFIGFILE_DIR, OptionsWindow::onConfigFileDir),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_CONFIG_DIR, OptionsWindow::onConfigDir),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_DAT_DIR, OptionsWindow::onDatDir),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_DISCMAGE_PATH, OptionsWindow::onDiscmagePath),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_BACKUP, OptionsWindow::onBackup),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_USEBACKUP, OptionsWindow::onUseBackup),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_GBALOADER_PATH, OptionsWindow::onGBALoaderPath),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_F2A, OptionsWindow::onF2A),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_EDIT_CONFIGFILE, OptionsWindow::onEditConfigFile),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_BROWSE, OptionsWindow::onBrowse),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_SNES, OptionsWindow::onSNES),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_GENESIS, OptionsWindow::onGenesis),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_GB, OptionsWindow::onGB),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_GBA, OptionsWindow::onGBA),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_N64, OptionsWindow::onN64),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_NES, OptionsWindow::onNES),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_SMS, OptionsWindow::onSMS),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_PCE, OptionsWindow::onPCE),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_NGP, OptionsWindow::onNGP),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_MISC, OptionsWindow::onMisc),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_TIPTEXT, OptionsWindow::onTipText),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_TEST, OptionsWindow::onTest),
  FXMAPFUNC(SEL_COMMAND, OptionsWindow::ID_SAVE, OptionsWindow::onSave)
};

FXIMPLEMENT(OptionsWindow, FXTopWindow, OptionsWindowMap, ARRAYNUMBER(OptionsWindowMap))


OptionsWindow::OptionsWindow(FXWindow *a_owner, const FXString &a_name,
    FXIcon *a_icon, FXIcon *a_icon2, FXuint a_opts, FXint a_x, FXint a_y,
    FXint a_w, FXint a_h, FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb,
    FXint a_hs, FXint a_vs)
  : FXTopWindow(a_owner, a_name, a_icon, a_icon2, a_opts, a_x, a_y, a_w, a_h,
        a_pl, a_pr, a_pt, a_pb, a_hs, a_vs)
{
  setTarget(a_owner);
  setSelector(ID_CLOSE);

  m_mainWindow = (MainWindow *) a_owner;
  m_toolTip = NULL;

  FXApp *app = getApp();
  m_registry = &app->reg();

  FXVerticalFrame *windowArea = new FXVerticalFrame(this, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXHorizontalFrame *tabBookArea = new FXHorizontalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXTabBook *m_tabBook = new FXTabBook(tabBookArea, NULL, 0,
    LAYOUT_FILL | PACK_UNIFORM_WIDTH | PACK_UNIFORM_HEIGHT);//, 0, 0, 0, 0, 0, 0, 0, 0);

  new FXTabItem(m_tabBook, "&uCON64");
  FXVerticalFrame *uCON64Tab = new FXVerticalFrame(m_tabBook, FRAME_RAISED);

  FXMatrix *nonOverrideArea = new FXMatrix(uCON64Tab, 3,
    MATRIX_BY_COLUMNS | LAYOUT_FILL, 0, 0, 0, 0, 0, 0, DEFAULT_PAD, 0);//, 0, 0);

  m_uCON64Path = TextFieldOption::add(nonOverrideArea, "Path to uCON64 executable:",
    30, true, this, ID_UCON64_PATH, false, "Main Settings", "uCON64ExePath", "");

  m_configFileDir = TextFieldOption::add(nonOverrideArea,
    "uCON64 configuration file directory:", 30, true, this, ID_CONFIGFILE_DIR,
    false, "Main Settings", "uCON64ConfigFileDir", ".");

  new FXButton(nonOverrideArea, "Edit configuration file", NULL, this, ID_EDIT_CONFIGFILE);

  FXFont *boldFont = ((Frontend *) app)->getBoldFont();

  FXGroupBox *overrideAreaContainer = new FXGroupBox(uCON64Tab,
    "Override configuration file settings", FRAME_GROOVE | LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  overrideAreaContainer->setFont(boldFont);
  FXMatrix *overrideArea = new FXMatrix(overrideAreaContainer, 4,
    MATRIX_BY_COLUMNS | LAYOUT_FILL, 0, 0, 0, 0, DEFAULT_PAD, DEFAULT_PAD, 0, DEFAULT_PAD);//, 0, 0);
  new FXLabel(overrideArea, "");                // spacer
  new FXLabel(overrideArea, "", NULL, LAYOUT_FILL_COLUMN); // spacer
  new FXLabel(overrideArea, "");                // spacer
  FXLabel *override = new FXLabel(overrideArea, "Override?");
  override->setFont(boldFont);

  m_configDir = TextFieldOption::add(overrideArea, "uCON64 configuration directory:",
    30, true, this, ID_CONFIG_DIR, true, "Override Settings", "uCON64ConfigDir", ".");

  m_datDir = TextFieldOption::add(overrideArea, "uCON64 DAT file directory:",
    30, true, this, ID_DAT_DIR, true, "Override Settings", "uCON64DatDir", ".");

  m_discmagePath = TextFieldOption::add(overrideArea, "Path to discmage DLL:",
    30, true, this, ID_DISCMAGE_PATH, true, "Override Settings", "DiscmagePath", "");

  m_parPortAddress = TextFieldOption::add(overrideArea, "(Parallel) port base address:",
    4, false, NULL, 0, true, "Override Settings", "ParPortAddress", "378");
  m_parPortAddress->setLayoutHints(FRAME_SUNKEN | LAYOUT_FILL_COLUMN);

#if     defined __unix__ && !defined __CYGWIN__
  m_parPortDevice = TextFieldOption::add(overrideArea, "Parallel port device:",
    12, false, NULL, 0, true, "Override Settings", "ParPortDevice", "/dev/parport0");
#endif

  new FXLabel(overrideArea, "Create backups?");
  FXGroupBox *backupAreaContainer = new FXGroupBox(overrideArea, "",
    GROUPBOX_NORMAL | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXHorizontalFrame *backupArea = new FXHorizontalFrame(backupAreaContainer,
    LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_backupYes = new FXRadioButton(backupArea, "yes", this, ID_BACKUP);
  m_backupNo = new FXRadioButton(backupArea, "no", this, ID_BACKUP);
  new FXLabel(overrideArea, "");
  m_useBackup = new FXCheckButton(overrideArea, "", this, ID_USEBACKUP,
    CHECKBUTTON_NORMAL | LAYOUT_CENTER_X);

  m_regBackup = new RegistryEntry(m_registry, "Override Settings", "Backup", "yes");
  if (strcmp(m_regBackup->readStringEntry(), "yes") == 0)
    m_backupYes->setCheck();                    // default state of FXRadioButton is unchecked
  else
    m_backupNo->setCheck();

  m_regUseBackup = new RegistryEntry(m_registry, "Override Settings", "UseBackup", "no");
  if (strcmp(m_regUseBackup->readStringEntry(), "no") == 0)
    {
      m_backupYes->disable();                   // default state of FXCheckButton is enabled
      m_backupNo->disable();
    }
  else
    m_useBackup->setCheck();                    // default state of FXCheckButton is unchecked

  new FXHorizontalSeparator(overrideArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  new FXLabel(overrideArea, "", NULL, LAYOUT_FILL_COLUMN);
  new FXLabel(overrideArea, "");
  new FXLabel(overrideArea, "");

  m_GBALoaderPath = TextFieldOption::add(overrideArea, "GBA multi-game loader:",
    30, true, this, ID_GBALOADER_PATH, true, "Override Settings", "GBALoaderPath", "");

  m_F2AFirmwarePath = TextFieldOption::add(overrideArea, "F2A firmware:",
    30, true, this, ID_F2A, true, "Override Settings", "F2AFirmwarePath", "");

  m_F2AClientUSBPath = TextFieldOption::add(overrideArea, "F2A GBA client (USB):",
    30, true, this, ID_F2A, true, "Override Settings", "F2AClientUSBPath", "");

  m_F2AClientParPortPath = TextFieldOption::add(overrideArea, "F2A GBA client (parallel port):",
    30, true, this, ID_F2A, true, "Override Settings", "F2AClientParPortPath", "");

  m_F2ALogoPath = TextFieldOption::add(overrideArea, "F2A logo:",
    30, true, this, ID_F2A, true, "Override Settings", "F2ALogoPath", "");


  new FXTabItem(m_tabBook, "&Frontend");
  FXVerticalFrame *frontendTab = new FXVerticalFrame(m_tabBook, FRAME_RAISED);

  FXGroupBox *enabledTabsContainer = new FXGroupBox(frontendTab, "Enabled tab sheets",
    FRAME_GROOVE | LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  enabledTabsContainer->setFont(boldFont);
  FXMatrix *enabledTabsArea = new FXMatrix(enabledTabsContainer, 3, MATRIX_BY_ROWS,
    0, 0, 0, 0);//, 0, 0, 0, 0, 0, 0);
  m_BrowseTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Browse", this, ID_BROWSE),
    new RegistryEntry(m_registry, "Main Settings", "EnableBrowseTab", "no"));
  m_SNESTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "SNES", this, ID_SNES),
    new RegistryEntry(m_registry, "Main Settings", "EnableSNESTab", "yes"));
  m_GenesisTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Genesis", this, ID_GENESIS),
    new RegistryEntry(m_registry, "Main Settings", "EnableGenesisTab", "yes"));
  m_GBTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Game Boy", this, ID_GB),
    new RegistryEntry(m_registry, "Main Settings", "EnableGameBoyTab", "yes"));
  m_GBATab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Game Boy Advance", this, ID_GBA),
    new RegistryEntry(m_registry, "Main Settings", "EnableGameBoyAdvanceTab", "yes"));
  m_N64Tab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Nintendo 64", this, ID_N64),
    new RegistryEntry(m_registry, "Main Settings", "EnableNintendo64Tab", "yes"));
  m_NESTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "NES", this, ID_NES),
    new RegistryEntry(m_registry, "Main Settings", "EnableNESTab", "yes"));
  m_SMSTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "SMS", this, ID_SMS),
    new RegistryEntry(m_registry, "Main Settings", "EnableSMSTab", "yes"));
  m_PCETab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "PC-Engine", this, ID_PCE),
    new RegistryEntry(m_registry, "Main Settings", "EnablePCETab", "yes"));
  m_NGPTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Neo Geo Pocket", this, ID_NGP),
    new RegistryEntry(m_registry, "Main Settings", "EnableNGPTab", "no"));
  m_miscTab = new CheckButtonOption(
    new FXCheckButton(enabledTabsArea, "Misc", this, ID_MISC),
    new RegistryEntry(m_registry, "Main Settings", "EnableMiscTab", "yes"));

  FXVerticalFrame *miscArea = new FXVerticalFrame(frontendTab, LAYOUT_FILL,
    0, 0, 0, 0, 0);
  // I put the following option here, because to the user it means a difference
  //  in how the frontend behaves
  m_runInFrontendMode = new CheckButtonOption(
    new FXCheckButton(miscArea, "Run uCON64 in frontend mode (causes several options to open a progress window)"),
    new RegistryEntry(m_registry, "Main Settings", "RunInFrontendMode", "yes"));
  m_useIntermediaryFiles = new CheckButtonOption(
    new FXCheckButton(miscArea, "Use intermediary files for output (use with care!)"),
    new RegistryEntry(m_registry, "Main Settings", "UseIntermediaryFiles", "no"));
  m_enableTipText = new CheckButtonOption(
    new FXCheckButton(miscArea, "Enable hint messages (\"tooltips\")", this, ID_TIPTEXT),
    new RegistryEntry(m_registry, "Main Settings", "EnableTipText", "yes"));
  if (m_enableTipText->enabled())
    handle(m_enableTipText, FXSEL(SEL_COMMAND, ID_TIPTEXT), (void *) -1); // NOT 1! See onTipText() 

  FXHorizontalFrame *buttonArea = new FXHorizontalFrame(windowArea,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, DEFAULT_PAD);//, 0, 0);
  new FXButton(buttonArea, "Test", NULL, this, ID_TEST);
  new FXButton(buttonArea, "Save settings", NULL, this, ID_SAVE);
}


OptionsWindow::~OptionsWindow()
{
}


long
OptionsWindow::onuCON64Path(FXObject *, FXSelector, void *)
{
  FXString startPath = m_uCON64Path->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select uCON64 executable",
    startPath);
  if (path.text()[0])
    m_uCON64Path->setText(path);
  return 1;
}


long
OptionsWindow::onConfigFileDir(FXObject *, FXSelector, void *)
{
  FXString startPath = m_configFileDir->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenDirectory(this,
    "Select uCON64 configuration file directory", startPath);
  if (path.text()[0])
    m_configFileDir->setText(path);
  return 1;
}


long
OptionsWindow::onConfigDir(FXObject *, FXSelector, void *)
{
  FXString startPath = m_configDir->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenDirectory(this,
    "Select uCON64 configuration directory", startPath);
  if (path.text()[0])
    m_configDir->setText(path);
  return 1;
}


long
OptionsWindow::onDatDir(FXObject *, FXSelector, void *)
{
  FXString startPath = m_datDir->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenDirectory(this,
    "Select uCON64 DAT file directory", startPath);
  if (path.text()[0])
    m_datDir->setText(path);
  return 1;
}


long
OptionsWindow::onDiscmagePath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_discmagePath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select discmage DLL",
    startPath);
  if (path.text()[0])
    m_discmagePath->setText(path);
  return 1;
}


long
OptionsWindow::onBackup(FXObject *a_src, FXSelector, void *)
{
  m_backupYes->setCheck(a_src == m_backupYes);
  m_backupNo->setCheck(a_src == m_backupNo);
  return 1;
}


long
OptionsWindow::onUseBackup(FXObject *, FXSelector, void *)
{
  if (m_useBackup->getCheck())
    {
      m_backupYes->enable();
      m_backupNo->enable();
    }
  else
    {
      m_backupYes->disable();
      m_backupNo->disable();
    }
  return 1;
}


long
OptionsWindow::onGBALoaderPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_GBALoaderPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this,
    "Select GBA multi-game loader", startPath);
  if (path.text()[0])
    m_GBALoaderPath->setText(path);
  return 1;
}


long
OptionsWindow::onF2A(FXObject *a_src, FXSelector, void *)
{
  TextFieldOption *option;

  if (a_src == m_F2AFirmwarePath->getChangeButton())
    option = m_F2AFirmwarePath;
  else if (a_src == m_F2AClientUSBPath->getChangeButton())
    option = m_F2AClientUSBPath;
  else if (a_src == m_F2AClientParPortPath->getChangeButton())
    option = m_F2AClientParPortPath;
  else // if (a_src == m_F2ALogoPath->getChangeButton())
    option = m_F2ALogoPath;

  FXString startPath = option->getText();
  if (startPath.empty())
    startPath = ".";

  /*
    If the current text field contains only a period (or is empty), start in
    the directory of the first F2A text field that is set to a different
    value (may be the current directory). Precedence decreases from top to
    bottom.
  */
  if (startPath[0] == '.' && startPath[1] == 0)
    {
      FXString startPath2, startPath3;

      startPath2 = m_F2ALogoPath->getText();
      if (!startPath2.empty()) // && !FXFile::identical(startPath2, startPath))
        startPath3 = startPath2;

      startPath2 = m_F2AClientParPortPath->getText();
      if (!startPath2.empty()) // && !FXFile::identical(startPath2, startPath))
        startPath3 = startPath2;

      startPath2 = m_F2AClientUSBPath->getText();
      if (!startPath2.empty()) // && !FXFile::identical(startPath2, startPath))
        startPath3 = startPath2;

      startPath2 = m_F2AFirmwarePath->getText();
      if (!startPath2.empty()) // && !FXFile::identical(startPath2, startPath))
        startPath3 = startPath2;

      if (!startPath3.empty())
        startPath = startPath3;
    }
  // adding a path separator is necessary to make FOX start *in* the directory,
  //  not in the directory one level up in the directory hierarchy
  startPath += PATHSEP;

  FXString path = FileDialog::getOpenFilename(this, "Select a file", startPath);
  if (path.text()[0])
    option->setText(path);
  return 1;
}


long
OptionsWindow::onEditConfigFile(FXObject *, FXSelector, void *)
{
  char configFileName[FILENAME_MAX];
  FXString text = m_configFileDir->getText().trim();
  sprintf(configFileName, "%s"PATHSEPSTRING".ucon64rc", text.text());
  if (!FXStat::exists(FXString(configFileName)))
    {
      FXMessageBox::error(this, MBOX_OK, "Specified configuration file does not exist",
        "Please specify a correct configuration file directory\n"
        "You could also try to press the Test button first");
      return 1;
    }

  Frontend *fe = (Frontend *) getApp();
  EditTextWindow *editWindow = new EditTextWindow(this, "Edit configuration file",
    configFileName, fe->m_bigIcon, fe->m_smallIcon);
  editWindow->create();
  editWindow->show(PLACEMENT_OWNER);
  return 1;
}


long
OptionsWindow::onBrowse(FXObject *, FXSelector, void *)
{
  // 0 = FXTabItem, 1 = FXHorizontalFrame, 2 = FXTabItem, etc.
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_BROWSE);
  if (m_BrowseTab->enabled())
    {
      child->show();
      m_mainWindow->updateTitle(false);
    }
  else
    {
      child->hide();
      m_mainWindow->updateTitle(true);
    }
  child->recalc();
  return 1;
}


long
OptionsWindow::onSNES(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_SNES);
  if (m_SNESTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onGenesis(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_GENESIS);
  if (m_GenesisTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onGB(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_GB);
  if (m_GBTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onGBA(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_GBA);
  if (m_GBATab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onN64(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_N64);
  if (m_N64Tab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onNES(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_NES);
  if (m_NESTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onSMS(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_SMS);
  if (m_SMSTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onPCE(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_PCE);
  if (m_PCETab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onNGP(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_NGP);
  if (m_NGPTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onMisc(FXObject *, FXSelector, void *)
{
  FXWindow *child = m_mainWindow->getTabBook()->childAtIndex(MainWindow::TAB_MISC);
  if (m_miscTab->enabled())
    child->show();
  else
    child->hide();
  child->recalc();
  return 1;
}


long
OptionsWindow::onTipText(FXObject *, FXSelector, void *a_data)
{
  int value = (int) a_data;
  if (value)
    {
      if (!m_toolTip)
        {
          FXApp *app = getApp();
          m_toolTip = new FXToolTip(app);           // enable tool tip messages
          if (value != -1)
            m_toolTip->create();                    // must not be called at construction time!
          app->setToolTipTime(4000);                // time (in ms) to display message
          app->setToolTipPause(1000);
        }
    }
  else
    {
      delete m_toolTip;
      m_toolTip = NULL;
    }
  return 1;
}


long
OptionsWindow::onTest(FXObject *, FXSelector, void *)
{
  CommandLineHandler *commandLineHandler = m_mainWindow->getCommandLineHandler();
  if (commandLineHandler->busy())
    return 1;

  commandLineHandler->setOption("--version", false);
  commandLineHandler->run();
  return 1;
}


long
OptionsWindow::onSave(FXObject *, FXSelector, void *)
{
  // uCON64 tab sheet
  m_uCON64Path->writeToRegistry();
  m_configFileDir->writeToRegistry();
  m_configDir->writeToRegistry();
  m_datDir->writeToRegistry();
  m_discmagePath->writeToRegistry();
  m_parPortAddress->writeToRegistry();
#if     defined __unix__ && !defined __CYGWIN__
  m_parPortDevice->writeToRegistry();
#endif
  m_regBackup->writeStringEntry(m_backupYes->getCheck() ? "yes" : "no");
  m_regUseBackup->writeStringEntry(m_useBackup->getCheck() ? "yes" : "no");
  m_GBALoaderPath->writeToRegistry();
  m_F2AFirmwarePath->writeToRegistry();
  m_F2AClientUSBPath->writeToRegistry();
  m_F2AClientParPortPath->writeToRegistry();
  m_F2ALogoPath->writeToRegistry();

  // Frontend tab sheet
  m_BrowseTab->writeToRegistry();
  m_SNESTab->writeToRegistry();
  m_GenesisTab->writeToRegistry();
  m_GBTab->writeToRegistry();
  m_GBATab->writeToRegistry();
  m_N64Tab->writeToRegistry();
  m_NESTab->writeToRegistry();
  m_SMSTab->writeToRegistry();
  m_PCETab->writeToRegistry();
  m_NGPTab->writeToRegistry();
  m_miscTab->writeToRegistry();
  m_runInFrontendMode->writeToRegistry();
  m_useIntermediaryFiles->writeToRegistry();
  m_enableTipText->writeToRegistry();

  m_registry->write();                          // commit changes
  FXMessageBox::information(this, MBOX_OK, "Saved settings",
    "Wrote settings to registry file");
  return 1;
}

