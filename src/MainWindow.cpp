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
#include <stdio.h>
#include <fx.h>
#include "MainWindow.h"
#include "Process.h"
#ifdef  FORK_IPC
#include "ProcessIPC.h"
#endif
#include "OutputChecker.h"
#include "ProgressDialog.h"
#include "CommandLineHandler.h"
#include "FileSelector.h"
#include "OptionsWindow.h"
#include "Frontend.h"
#include "RegistryEntry.h"
#include "FileDialog.h"
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


FXDEFMAP(MainWindow) MainWindowMap[] =
{
  FXMAPFUNC(SEL_COMMAND, MainWindow::ID_RESETPARPORT, MainWindow::onResetParPort),
  FXMAPFUNC(SEL_COMMAND, MainWindow::ID_OUTPUTDIR, MainWindow::onOutputDir),
  FXMAPFUNC(SEL_COMMAND, MainWindow::ID_SELECTFILE, MainWindow::onSelectFile),
  FXMAPFUNC(SEL_COMMAND, MainWindow::ID_ABOUT, MainWindow::onAbout),
  FXMAPFUNC(SEL_COMMAND, MainWindow::ID_OPTIONS, MainWindow::onOptions),
  FXMAPFUNC(SEL_CLOSE, OptionsWindow::ID_CLOSE, MainWindow::onCloseOptionsWindow)
};

FXIMPLEMENT(MainWindow, FXMainWindow, MainWindowMap, ARRAYNUMBER(MainWindowMap))


MainWindow::MainWindow(FXApp *a_app, const FXString &a_name, FXIcon *a_icon,
    FXIcon *a_icon2, FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h,
    FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXMainWindow(a_app, a_name, a_icon, a_icon2, a_opts, a_x, a_y, a_w, a_h,
        a_pl, a_pr, a_pt, a_pb, a_hs, a_vs)
{
  m_commandLineHandler = new CommandLineHandler(this);
  Frontend *fe = (Frontend *) getApp();
  m_optionsWindow = new OptionsWindow(this, "Options", fe->m_bigIcon,
    fe->m_smallIcon, DECOR_ALL & ~DECOR_SHRINKABLE);

  FXMenuBar *menuBar = new FXMenuBar(this, LAYOUT_TOP | LAYOUT_FILL_X);
  new FXHorizontalSeparator(this, LAYOUT_TOP | LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_programMenu = new FXMenuPane(this);
  new FXMenuCommand(m_programMenu, "&Select file", NULL, this, ID_SELECTFILE);
  new FXMenuCommand(m_programMenu, "&Options", NULL, this, ID_OPTIONS);
  new FXMenuCommand(m_programMenu, "&About", NULL, this, ID_ABOUT);
  // the next statement is the reason why m_programMenu has to be a class member
  new FXMenuCommand(m_programMenu, "&Quit\tAlt-F4", NULL, a_app, FXApp::ID_QUIT);
  new FXMenuTitle(menuBar, "&Frontend", NULL, m_programMenu);
  
  FXVerticalFrame *windowArea = new FXVerticalFrame(this, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXHorizontalFrame *tabBookArea = new FXHorizontalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_tabBook = new FXTabBook(tabBookArea, NULL, 0,
    LAYOUT_FILL | PACK_UNIFORM_WIDTH | PACK_UNIFORM_HEIGHT);//, 0, 0, 0, 0, 0, 0, 0, 0);

  FXTabItem *tabItem;

  m_nSelectedFiles = 0;
  tabItem = new FXTabItem(m_tabBook, "&Browse");
  if (!m_optionsWindow->useBrowseTab())
    {
      tabItem->hide();
      updateTitle(true);
    }
  FXHorizontalFrame *browseTab = new FXHorizontalFrame(m_tabBook, FRAME_RAISED);
  m_fileSelector = new FileSelector(browseTab, NULL, 0,
    LAYOUT_FILL | SELECTFILE_NOOKCANCEL);
  m_fileSelector->setSelectMode(SELECTFILE_MULTIPLE_ALL);
  m_fileSelector->setPatternList(
    "All files (*)\n"
    "SNES files (*.smc,*.swc,*.fig,*.ufo,*.?gm,*.gd3,sf*.048,sf*.058,sf*.078)\n"
    "Genesis/Mega Drive files (*.smd,*.md,md*.000,md*.008,md*.018,md*.038)\n"
    "Game Boy files (*.gb,gb*.030,gb*.040,gb*.048,gb*.058)\n"
    "Game Boy Advance files (*.gba)\n"
    "Nintendo 64 files (*.v64,*.z64,*.rom)\n"
    "NES/Famicom files (*.nes,*.unf,*.unif,*.prm,*.prg,*.chr)\n"
    "SMS/GG files (*.smd,*.gg,*.sms,gg*.040,gg*.048,gg*.060,gg*.078)\n"
    "PCE/TG-16 files (*.pce,*.msg,pc*.030,pc*.040,pc*.048,pc*.058)\n"
    "Compressed files (*.zip,*.gz)");
  m_fileSelector->setMatchMode(m_fileSelector->getMatchMode() | FXPath::CaseFold);
  m_fileSelector->allowPatternEntry(TRUE);
  m_fileSelector->setNumVisibleFilter(4);       // FOX 1.3 displays too many items

  tabItem = new FXTabItem(m_tabBook, "&SNES");
  if (!m_optionsWindow->useSNESTab())
    tabItem->hide();
  m_SNESFrame = new SNESFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "&Genesis");
  if (!m_optionsWindow->useGenesisTab())
    tabItem->hide();
  m_GenesisFrame = new GenesisFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "G&B");
  if (!m_optionsWindow->useGBTab())
    tabItem->hide();
  m_GBFrame = new GBFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "GB&A");
  if (!m_optionsWindow->useGBATab())
    tabItem->hide();
  m_GBAFrame = new GBAFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "&N64");
  if (!m_optionsWindow->useN64Tab())
    tabItem->hide();
  m_N64Frame = new N64Frame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "N&ES");
  if (!m_optionsWindow->useNESTab())
    tabItem->hide();
  m_NESFrame = new NESFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "S&MS");
  if (!m_optionsWindow->useSMSTab())
    tabItem->hide();
  m_SMSFrame = new SMSFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "P&CE");
  if (!m_optionsWindow->usePCETab())
    tabItem->hide();
  m_PCEFrame = new PCEFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "NG&P");
  if (!m_optionsWindow->useNGPTab())
    tabItem->hide();
  m_NGPFrame = new NGPFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  tabItem = new FXTabItem(m_tabBook, "&Misc");
  if (!m_optionsWindow->useMiscTab())
    tabItem->hide();
  m_miscFrame = new MiscFrame(m_commandLineHandler, m_tabBook, FRAME_RAISED);

  new FXTabItem(m_tabBook, "&Output");
  FXHorizontalFrame *outputTab = new FXHorizontalFrame(m_tabBook, FRAME_RAISED);
  m_outputText = new FXText(outputTab, NULL, 0, LAYOUT_FILL);
  m_outputText->setEditable(false);
  m_outputText->setText("No output available yet");
#if 1
  FXFont *font;
  struct st_fontRequest
  {
    char *name;
    int size;
  } fontRequests[] =
    {
      { "lucidatypewriter", 90 }, // first 4 are Linux fonts, next 6 are meant
      { "fixed [alias]", 90 },    //  for Windows
      { "fixed [misc]", 90 },
      { "courier [adobe]", 90 },
      { "lucida console", 90 },   // scalable
      { "nsimsun", 100 },         // idem
      { "courier new", 90 },      // idem
      { "mingliu", 100 },         // idem
      { "courier", 90 },          // fixed (98)
      { "terminal", 100 }         // reported as fixed (90), but appears to be scalable
    };
  int n;
  for (n = 0; n < 10; n++)
    if ((font = searchFont(fontRequests[n].name, fontRequests[n].size)))
      {
        m_outputText->setFont(font);
        break;
      }
#endif

  FXHorizontalFrame *bottomArea = new FXHorizontalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 2 * DEFAULT_PAD, 2 * DEFAULT_PAD, 0, 0, 0, 0);

  FXHorizontalFrame *bottomButtonArea = new FXHorizontalFrame(bottomArea,
    LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0, 2 * DEFAULT_PAD, 0);

  m_abortButton = new FXButton(bottomButtonArea, "Abort uCON64", NULL,
    m_commandLineHandler, CommandLineHandler::ID_ABORT);
  m_abortButton->disable();
#ifndef DISABLE_COPIER
  new FXButton(bottomButtonArea, "Reset parallel port", NULL, this, ID_RESETPARPORT);
#endif

  FXHorizontalFrame *outputDirArea = new FXHorizontalFrame(bottomArea, LAYOUT_RIGHT,
#ifndef __APPLE__    
    0, 0, 0, 0, 0, 0, 0, DEFAULT_PAD);//, 0, 0);
#else
    0, 0, 0, 0, 0, 16, 0, DEFAULT_PAD);
#endif
  new FXLabel(outputDirArea, "Output directory:");
  m_outputDir = new FXTextField(outputDirArea, 30, NULL, 0, FRAME_SUNKEN);
  new FXButton(outputDirArea, "Change", NULL, this, ID_OUTPUTDIR);
  m_regOutputDir = new RegistryEntry(&a_app->reg(), "Main Settings", "OutputDir", ".");
  m_outputDir->setText(m_regOutputDir->readStringEntry());
}


void
MainWindow::updateTitle(bool a_showSelectedFilesNo)
{
  FXString mainTitle;
  // Note: m_nSelectedFiles doesn't have the correct value if the user selected
  //       some files in the Browse tab and then disables it.
  if (a_showSelectedFilesNo)
    mainTitle = FXString::value("uCON64 frontend %d.%d - %d file%s selected",
      UF_MAJOR, UF_MINOR, m_nSelectedFiles, m_nSelectedFiles == 1 ? "" : "s");
  else
    mainTitle = FXString::value("uCON64 frontend %d.%d", UF_MAJOR, UF_MINOR);
  setTitle(mainTitle);
}


FXFont *
MainWindow::searchFont(char *a_name, int a_preferredSize)
{
  FXFont *font = NULL;
  FXFontDesc *fontDescs;
  FXuint nfontDescs;

  if (FXFont::listFonts(fontDescs, nfontDescs, a_name, 0,
        0, /* FONTSLANT_DONTCARE, */ 0,
        0, 0) == TRUE) // FONTPITCH_DEFAULT
    {
      FXuint n;
      char description[80];

      for (n = 0; n < nfontDescs; n++)
        {
/*
          printf("face: %s; size: %d; weight: %d; slant: %d; setwidth: %d; encoding: %d; flags: %d\n",
                 fontDescs[n].face, fontDescs[n].size, fontDescs[n].weight,
                 fontDescs[n].slant, fontDescs[n].setwidth, fontDescs[n].encoding,
                 fontDescs[n].flags);
          fflush(stdout);
*/
          if (((fontDescs[n].size > 85 && fontDescs[n].size < 100) ||
                (fontDescs[n].flags & FXFont::Scalable)) &&
              (fontDescs[n].weight == FXFont::Normal ||
               fontDescs[n].weight == FXFont::Medium))
            {
//              printf("Found font: %s (%sscalable)\n", fontDescs[n].face,
//                fontDescs[n].flags & FONTHINT_SCALABLE ? "" : "not ");
              fflush(stdout);
              sprintf(description, "%s,%d", fontDescs[n].face, a_preferredSize);
              font = new FXFont(getApp(), description);
              break;
            }
        }
      FXFREE(&fontDescs);
    }
  return font;
}


MainWindow::~MainWindow()
{
  delete m_regOutputDir;
  delete m_outputDir;
  delete m_outputText;
  delete m_tabBook;
  delete m_programMenu;
  delete m_optionsWindow;
  delete m_commandLineHandler;
}


void
MainWindow::create()
{
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}


FXString *
MainWindow::getSelectedFilenames(void)
{
  return m_fileSelector->getFilenames();
}


void
MainWindow::clearSelectedFilenames(void)
{
  m_fileSelector->clearFilenames();

  m_nSelectedFiles = 0;
  if (!m_tabBook->childAtIndex(TAB_BROWSE)->shown())
    updateTitle(true);
}


void
MainWindow::selectOutputTab(void)
{
  m_tabBook->setCurrent(TAB_OUTPUT / 2);
}


void
MainWindow::enableAbortButton(void)
{
  m_abortButton->enable();
}


void
MainWindow::disableAbortButton(void)
{
  m_abortButton->disable();
}


long
MainWindow::onResetParPort(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--xreset", false);
  m_commandLineHandler->run();
  return 1;
}


long
MainWindow::onOutputDir(FXObject *, FXSelector, void *)
{
  FXString path = FileDialog::getOpenDirectory(this, "Select output directory",
    m_outputDir->getText());
  if (path.text()[0])
    m_outputDir->setText(path);
  return 1;
}


long
MainWindow::onSelectFile(FXObject *, FXSelector, void *)
{
  FileDialog *fileDialog = new FileDialog(this, "Select one or more files");
  fileDialog->setSelectMode(SELECTFILE_MULTIPLE_ALL);
  int status = fileDialog->execute();
  if (status == 0)
    {
      delete fileDialog;
      return 1;
    }

  m_fileSelector->clearFilenames();
  FXString *fnames = fileDialog->getFilenames();
  delete fileDialog;

  int n;
  for (n = 0; fnames[n][0]; n++)
    m_fileSelector->setFilename(fnames[n]);

  m_nSelectedFiles = n;
  if (!m_tabBook->childAtIndex(TAB_BROWSE)->shown())
    updateTitle(true);

  delete [] fnames;
  return 1;
}


long
MainWindow::onAbout(FXObject *, FXSelector, void *)
{
  FXDialogBox *aboutWindow = new FXDialogBox(this, "About uCON64 frontend");
  FXString aboutText = FXString::value(
    "\n"
    "A graphical frontend for the\n"
    "ROM tool uCON64. Version %d.%d.\n"
    "\n"
    "Copyright (c) 2004, 2005 dbjh\n"
    "(dbjh@users.sourceforge.net)\n"
    "\n"
    "Swiss army knife icons are\n"
    "copyright (c) 2005 kyuusaku\n"
    "\n"
    "This software uses the FOX toolkit library\n"
    "(http://www.fox-toolkit.org), version %d.%d.%d.\n",
    UF_MAJOR, UF_MINOR, FOX_MAJOR, FOX_MINOR, FOX_LEVEL);
  FXLabel *content = new FXLabel(aboutWindow, aboutText,
    ((Frontend *) getApp())->m_veryBigIcon);
  content->setIconPosition(ICON_ABOVE_TEXT);
  new FXButton(aboutWindow, "Yeah!", NULL, aboutWindow, FXDialogBox::ID_ACCEPT,
    BUTTON_NORMAL | LAYOUT_CENTER_X, 0, 0, 0, 0, 5, 5);
  aboutWindow->setFocus();                      // only necessary on Mac OS X
  aboutWindow->execute(PLACEMENT_OWNER);
  delete aboutWindow;
  return 1;
}


long
MainWindow::onOptions(FXObject *, FXSelector, void *)
{
  m_optionsWindow->show(PLACEMENT_OWNER);
  m_optionsWindow->setFocus();
  return 1;
}


long
MainWindow::onCloseOptionsWindow(FXObject *, FXSelector a_sel, void *)
{
  if (FXSELID(a_sel) == OptionsWindow::ID_CLOSE)
    m_optionsWindow->hide();
  return 1;                                     // object against close
#if 0
  else
    {
      m_optionsWindow = NULL;
      return 0;
    }
#endif
}


void
MainWindow::writeToRegistry(void)
/*
  This function gets called by Frontend::exit(). We can't move this code to the
  destructor, because the d'tor gets called after Frontend::exit(). If we
  call the d'tor of MainWindow in Frontend::exit(), we get a segmentation fault
  at program exit, because an FXMainWindow deletes itself. MainWindow::destroy()
  is not an option (doesn't even get called). The alternative is to call
  FXApp::reg().write() in the different tab sheet d'tors, but that's very
  inefficient.
*/
{
  FXString text = m_outputDir->getText();
  m_regOutputDir->writeStringEntry(text.text());
  m_SNESFrame->writeToRegistry();
  m_GenesisFrame->writeToRegistry();
  m_GBFrame->writeToRegistry();
  m_GBAFrame->writeToRegistry();
  m_N64Frame->writeToRegistry();
  m_NESFrame->writeToRegistry();
  m_SMSFrame->writeToRegistry();
  m_PCEFrame->writeToRegistry();
  m_NGPFrame->writeToRegistry();
  m_miscFrame->writeToRegistry();
}
