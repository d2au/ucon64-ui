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
#include <time.h>
#include <fx.h>
#include <stdint.h>
#include "CommandLineHandler.h"
#include "ComboBox.h"
#include "FileDialog.h"
#include "Frontend.h"
#include "misc.h"


FXDEFMAP(MiscFrame) MiscFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_PATCHTYPE, MiscFrame::onPatchType),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_PATCHPATH, MiscFrame::onPatchPath),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_APPLYPATCH, MiscFrame::onApplyPatch),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_MODPATH, MiscFrame::onModPath),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CREATEPATCH, MiscFrame::onCreatePatch),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CHANGEDESCRIPTION, MiscFrame::onChangeDescription),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CHANGEDESCRIPTION2, MiscFrame::onChangeDescription2),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CHANGEDESCRIPTION2PATH, MiscFrame::onChangeDescription2Path),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_POKE, MiscFrame::onPoke),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_PATTERNPATH, MiscFrame::onPatternPath),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_APPLYPATTERN, MiscFrame::onApplyPattern),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_GAMEGENIE, MiscFrame::onGameGenie),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_SEARCHSTRING, MiscFrame::onSearchString),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_SEARCHTYPE, MiscFrame::onSearchType),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_USEDATCONSOLE, MiscFrame::onUseDATConsoleType),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_DATCONSOLE, MiscFrame::onDATConsoleType),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_DATINFO, MiscFrame::onDATInfo),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CREATEDAT, MiscFrame::onCreateDAT),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_SEARCHCRC32, MiscFrame::onSearchCRC32),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_RENAME, MiscFrame::onRename),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CALCCHECKSUM, MiscFrame::onCalcChecksum),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_OVERRIDEHEADERSIZE, MiscFrame::onOverrideHeaderSize),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_CHECKPAD, MiscFrame::onCheckPad),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_PADNEXTMB, MiscFrame::onPadNextMB),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_PADTOSIZE, MiscFrame::onPadToSize),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_STRIPEND, MiscFrame::onStripEnd),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_STRIPSTART, MiscFrame::onStripStart),
  FXMAPFUNC(SEL_COMMAND, MiscFrame::ID_INSERTSTART, MiscFrame::onInsertStart)
};

FXIMPLEMENT(MiscFrame, FXVerticalFrame, MiscFrameMap, ARRAYNUMBER(MiscFrameMap))


MiscFrame::MiscFrame(CommandLineHandler *a_commandLineHandler,
    FXComposite *a_p, FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h,
    FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXVerticalFrame(a_p, a_opts, a_x, a_y, a_w, a_h, a_pl, a_pr, a_pt, a_pb,
        a_hs, a_vs)
{
  m_commandLineHandler = a_commandLineHandler;

  FXFont *boldFont = ((Frontend *) getApp())->getBoldFont();

  FXHorizontalFrame *windowArea = new FXHorizontalFrame(this, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXVerticalFrame *leftArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *patchingAreaContainer = new FXGroupBox(leftArea, "Patching",
    FRAME_GROOVE | LAYOUT_FILL);
  patchingAreaContainer->setFont(boldFont);

  FXMatrix *patchingArea = new FXMatrix(patchingAreaContainer, 4,
    MATRIX_BY_COLUMNS | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);

  new FXLabel(patchingArea, "Patch type:");
  m_patchTypes = new ComboBox(patchingArea, 1, this, ID_PATCHTYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_COLUMN);
  m_patchTypes->appendItem("IPS");
  m_patchTypes->appendItem("BSL");
  m_patchTypes->appendItem("APS");
  m_patchTypes->appendItem("PPF");
  m_patchTypes->setNumVisible(4);
  new FXLabel(patchingArea, "");
  new FXLabel(patchingArea, "");

  new FXLabel(patchingArea, "Patch file:");
  m_patchPath = new FXTextField(patchingArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
  new FXButton(patchingArea, "Change", NULL, this, ID_PATCHPATH);
  new FXButton(patchingArea, "Apply", NULL, this, ID_APPLYPATCH,
    BUTTON_NORMAL | LAYOUT_FILL_X);

  m_modifiedFilePathLabel = new FXLabel(patchingArea, "Modified file:");
  m_modifiedFilePath = new FXTextField(patchingArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
  m_modifiedFilePathButton = new FXButton(patchingArea, "Change", NULL, this, ID_MODPATH);
  m_createPatchButton = new FXButton(patchingArea, "Create patch", NULL, this, ID_CREATEPATCH,
    BUTTON_NORMAL | LAYOUT_FILL_X);

  FXMatrix *patchDescriptionArea = new FXMatrix(patchingAreaContainer, 2,
    MATRIX_BY_COLUMNS | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);

  m_patchDescriptionButton = new FXButton(patchDescriptionArea, "Change description to",
    NULL, this, ID_CHANGEDESCRIPTION, BUTTON_NORMAL | LAYOUT_FILL_X);
  m_patchDescription = new FXTextField(patchDescriptionArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);

  m_patchDescription2Button = new FXButton(patchDescriptionArea, "Change FILE_ID.DIZ to",
    NULL, this, ID_CHANGEDESCRIPTION2, BUTTON_NORMAL | LAYOUT_FILL_X);
  FXHorizontalFrame *patchFILE_IDArea = new FXHorizontalFrame(patchDescriptionArea,
    LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_PAD, 0);
  m_patchDescription2Path = new FXTextField(patchFILE_IDArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  m_patchDescription2PathButton = new FXButton(patchFILE_IDArea, "Change", NULL,
    this, ID_CHANGEDESCRIPTION2PATH);

  new FXHorizontalSeparator(patchingAreaContainer, SEPARATOR_GROOVE | LAYOUT_FILL_X,
    0, 0, 0, 0, 1, 1, 4 * DEFAULT_PAD, 4 * DEFAULT_PAD);
  FXHorizontalFrame *pokeArea = new FXHorizontalFrame(patchingAreaContainer, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(pokeArea, "Offset:");
  m_pokeOffset = new FXTextField(pokeArea, 8, NULL, 0, FRAME_SUNKEN);
  m_pokeOffset->setTipText("Specify a hexadecimal value");
  new FXLabel(pokeArea, "Value:");
  m_pokeValue = new FXTextField(pokeArea, 2, NULL, 0, FRAME_SUNKEN);
  m_pokeValue->setTipText("Specify a hexadecimal value");
  m_pokeButton = new FXButton(pokeArea, "Apply", NULL, this, ID_POKE,
    BUTTON_NORMAL | LAYOUT_RIGHT);

  FXHorizontalFrame *patternArea = new FXHorizontalFrame(patchingAreaContainer, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(patternArea, "Pattern file:");
  m_patternFilePath = new FXTextField(patternArea, 1, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X);
  new FXButton(patternArea, "Change", NULL, this, ID_PATTERNPATH);
  new FXButton(patternArea, "Apply", NULL, this, ID_APPLYPATTERN);

  FXGroupBox *GameGenieAreaContainer = new FXGroupBox(patchingAreaContainer, "Game Genie",
    FRAME_GROOVE | LAYOUT_FILL, 0, 0, 0, 0, DEFAULT_PAD, DEFAULT_PAD, DEFAULT_PAD, 0);//, 0, 0);
  GameGenieAreaContainer->setFont(boldFont);
  FXHorizontalFrame *GameGenieAreaTop = new FXHorizontalFrame(GameGenieAreaContainer, 0,
    0, 0, 0, 0, DEFAULT_PAD, 0, 0, 0);//, 0, 0);
  new FXLabel(GameGenieAreaTop, "Code:");
  m_GameGenieCode = new FXTextField(GameGenieAreaTop, 11, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  new FXLabel(GameGenieAreaTop, "Console:");
  m_GameGenieConsoles = new ComboBox(GameGenieAreaTop, 1, NULL, 0,//this, ID_PATCHTYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC);
  m_GameGenieConsoles->appendItem("Game Gear");
  m_GameGenieConsoles->appendItem("Genesis");
  m_GameGenieConsoles->appendItem("Game Boy");
  m_GameGenieConsoles->appendItem("NES");
  m_GameGenieConsoles->appendItem("SNES");
  m_GameGenieConsoles->setCurrentItem(4);
  m_GameGenieConsoles->setNumVisible(5);

  FXHorizontalFrame *GameGenieAreaBottom = new FXHorizontalFrame(GameGenieAreaContainer, 0,
    0, 0, 0, 0, DEFAULT_PAD, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *GameGenieButtonArea = new FXHorizontalFrame(GameGenieAreaBottom,
    PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_GameGenieEncodeButton = new FXButton(GameGenieButtonArea, "Encode", NULL,
    this, ID_GAMEGENIE);
  m_GameGenieDecodeButton = new FXButton(GameGenieButtonArea, "Decode", NULL,
    this, ID_GAMEGENIE);
  m_GameGenieApplyButton = new FXButton(GameGenieButtonArea, "Apply", NULL,
    this, ID_GAMEGENIE);
  m_useGameGenieConsoles = new FXCheckButton(GameGenieAreaBottom, "Use console for apply");

  FXGroupBox *searchingArea = new FXGroupBox(leftArea, "Searching",
    FRAME_GROOVE | LAYOUT_FILL);

  FXHorizontalFrame *searchingAreaTop = new FXHorizontalFrame(searchingArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  searchingArea->setFont(boldFont);
  new FXButton(searchingAreaTop, "Search for string", NULL, this, ID_SEARCHSTRING);
  m_searchString = new FXTextField(searchingAreaTop, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);

  FXHorizontalFrame *searchingAreaBottom = new FXHorizontalFrame(searchingArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(searchingAreaBottom, "Search type:");
  m_searchTypes[0] = new FXRadioButton(searchingAreaBottom, "normal", this, ID_SEARCHTYPE);
  m_searchTypes[1] = new FXRadioButton(searchingAreaBottom, "ignore case", this, ID_SEARCHTYPE);
  m_searchTypes[2] = new FXRadioButton(searchingAreaBottom, "relative", this, ID_SEARCHTYPE);
  m_searchTypes[0]->setCheck();


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *DATArea = new FXGroupBox(rightArea, "DATabase",
    FRAME_GROOVE | LAYOUT_FILL);
  DATArea->setFont(boldFont);

  FXHorizontalFrame *DATConsoleTypeArea = new FXHorizontalFrame(DATArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_useDATConsoles = new FXCheckButton(DATConsoleTypeArea, "Force console type is",
    this, ID_USEDATCONSOLE);
  m_DATConsoles = new ComboBox(DATConsoleTypeArea, 1, this, ID_DATCONSOLE,
    FRAME_SUNKEN | COMBOBOX_STATIC);
  m_DATConsoles->appendItem("\"Atari hw.\"");
  m_DATConsoles->appendItem("GBA");
  m_DATConsoles->appendItem("Game Boy");
  m_DATConsoles->appendItem("Genesis");
  m_DATConsoles->appendItem("Jaguar");
  m_DATConsoles->appendItem("Lynx");
  m_DATConsoles->appendItem("Neo Geo");
  m_DATConsoles->appendItem("Neo Geo Pocket");
  m_DATConsoles->appendItem("NES");
  m_DATConsoles->appendItem("Nintendo 64");
  m_DATConsoles->appendItem("PC-Engine");
  m_DATConsoles->appendItem("SMS/GG");
  m_DATConsoles->appendItem("SNES");
  m_DATConsoles->appendItem("WonderSwan");
  m_DATConsoles->setNumVisible(5);

  FXMatrix *DATAreaBottom = new FXMatrix(DATArea, 2,
    MATRIX_BY_COLUMNS | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);

  new FXButton(DATAreaBottom, "Show DATabase info", NULL, this, ID_DATINFO,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  new FXLabel(DATAreaBottom, "", NULL, LAYOUT_FILL_COLUMN);

  new FXButton(DATAreaBottom, "Create DAT file", NULL, this, ID_CREATEDAT,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  m_DATFilename = new FXTextField(DATAreaBottom, 19, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
  m_DATFilename->setTipText("Specify a file name without a path");
  setDATFilename();

  new FXButton(DATAreaBottom, "Search CRC32 value", NULL, this, ID_SEARCHCRC32,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  m_searchCRC32 = new FXTextField(DATAreaBottom, 10, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_COLUMN);

  new FXButton(DATAreaBottom, "Rename to", NULL, this, ID_RENAME,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  m_renameTypes = new ComboBox(DATAreaBottom, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_COLUMN);
  m_renameTypesText[0] = "DATabase name";
  m_renameTypesText[1] = "internal name";
  m_renameTypesText[2] = "lowercase";
  m_renameTypesText[3] = "uppercase";
  m_renameTypes->appendItem(m_renameTypesText[0]);
  m_renameTypes->appendItem(m_renameTypesText[1]);
  m_renameTypes->appendItem(m_renameTypesText[2]);
  m_renameTypes->appendItem(m_renameTypesText[3]);
  m_renameTypes->setNumVisible(4);

  FXGroupBox *checksumArea = new FXGroupBox(rightArea, "Checksums",
    FRAME_GROOVE | LAYOUT_FILL);
  checksumArea->setFont(boldFont);

  FXHorizontalFrame *checksumAreaTop = new FXHorizontalFrame(checksumArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(checksumAreaTop, "Checksum algorithm:");
  m_checksumTypes = new ComboBox(checksumAreaTop, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC);
  m_checksumTypes->appendItem("CRC32");
  m_checksumTypes->appendItem("MD5");
  m_checksumTypes->appendItem("SHA1");
  m_checksumTypes->setNumVisible(3);
  new FXButton(checksumAreaTop, "Calculate", NULL, this, ID_CALCCHECKSUM);

  FXHorizontalFrame *checksumAreaBottom = new FXHorizontalFrame(checksumArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_overrideHeaderSize = new FXCheckButton(checksumAreaBottom,
    "Override header size:", this, ID_OVERRIDEHEADERSIZE);
  m_headerSize = new FXTextField(checksumAreaBottom, 4, NULL, 0,
    FRAME_SUNKEN | TEXTFIELD_INTEGER);
  m_headerSize->disable();

  FXGroupBox *paddingAreaContainer = new FXGroupBox(rightArea, "Padding",
    FRAME_GROOVE | LAYOUT_FILL);
  paddingAreaContainer->setFont(boldFont);

  FXHorizontalFrame *paddingAreaTop = new FXHorizontalFrame(paddingAreaContainer,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(paddingAreaTop, "Check if padded", NULL, this, ID_CHECKPAD);
  new FXButton(paddingAreaTop, "Pad to next Mb", NULL, this, ID_PADNEXTMB,
    BUTTON_NORMAL | LAYOUT_RIGHT);

  FXMatrix *paddingArea = new FXMatrix(paddingAreaContainer, 3,
    MATRIX_BY_COLUMNS, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);

  FXButton *padToSize = new FXButton(paddingArea, "Pad to size", NULL,
    this, ID_PADTOSIZE, BUTTON_NORMAL | LAYOUT_FILL_X);
  padToSize->setTipText("Size is the file size disregarding the header");
  m_padNewSize = new FXTextField(paddingArea, 8, NULL, 0, FRAME_SUNKEN);
  m_padNewSize->setTipText("Size is the file size disregarding the header");
  new FXLabel(paddingArea, "bytes");

  new FXButton(paddingArea, "Strip from end", NULL, this, ID_STRIPEND,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  m_stripEnd = new FXTextField(paddingArea, 8, NULL, 0, FRAME_SUNKEN);
  new FXLabel(paddingArea, "bytes");

  new FXButton(paddingArea, "Strip from start", NULL, this, ID_STRIPSTART,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  m_stripStart = new FXTextField(paddingArea, 8, NULL, 0, FRAME_SUNKEN);
  m_stripStart->setText("512");
  new FXLabel(paddingArea, "bytes");

  new FXButton(paddingArea, "Insert before start", NULL, this, ID_INSERTSTART,
    BUTTON_NORMAL | LAYOUT_FILL_X);
  m_insertStart = new FXTextField(paddingArea, 8, NULL, 0, FRAME_SUNKEN);
  m_insertStart->setText("512");
  new FXLabel(paddingArea, "bytes");
}


MiscFrame::~MiscFrame()
{
}


void
MiscFrame::writeToRegistry(void)
{
}


long
MiscFrame::onPatchType(FXObject *, FXSelector, void *)
{
  FXint patchType = m_patchTypes->getCurrentItem();
  switch (patchType)
    {
    case 0:                                     // IPS
    case 1:                                     // BSL
      m_patchDescription->disable();
      m_patchDescriptionButton->disable();
      m_patchDescription2Path->disable();
      m_patchDescription2Button->disable();
      m_patchDescription2PathButton->disable();
      break;
    case 2:                                     // APS
      m_patchDescription->enable();
      m_patchDescriptionButton->enable();
      m_patchDescription2Path->disable();
      m_patchDescription2Button->disable();
      m_patchDescription2PathButton->disable();
      break;
    case 3:                                     // PPF
      m_patchDescription->enable();
      m_patchDescriptionButton->enable();
      m_patchDescription2Path->enable();
      m_patchDescription2Button->enable();
      m_patchDescription2PathButton->enable();
      break;
    }

  if (patchType == 1)
    {
      m_modifiedFilePathLabel->disable();
      m_modifiedFilePath->disable();
      m_modifiedFilePathButton->disable();
      m_createPatchButton->disable();
    }
  else
    {
      m_modifiedFilePathLabel->enable();
      m_modifiedFilePath->enable();
      m_modifiedFilePathButton->enable();
      m_createPatchButton->enable();
    }

  return 1;
}


long
MiscFrame::onPatchPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_patchPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select patch", startPath);
  if (path.text()[0])
    m_patchPath->setText(path);
  return 1;
}


long
MiscFrame::onApplyPatch(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_patchPath->getText();
  char *filename = (char *) text.text();
  if (!filename[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No patch specified",
        "Please specify a patch");
      return 1;
    }

  char option[FILENAME_MAX + 8];
#ifdef  __unix__
  sprintf(option, "--patch=%s", filename);
#else
  sprintf(option, "--patch=\"%s\"", filename);
#endif
  m_commandLineHandler->setOption(option, true);

  switch (m_patchTypes->getCurrentItem())
    {
    case 0:                                     // IPS
      m_commandLineHandler->addOption("-i");
      break;
    case 1:                                     // BSL
      m_commandLineHandler->addOption("-b");
      break;
    case 2:                                     // APS
      m_commandLineHandler->addOption("-a");
      break;
    case 3:                                     // PPF
      m_commandLineHandler->addOption("--ppf");
      break;
    }

  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onModPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_modifiedFilePath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select modified file",
    startPath);
  if (path.text()[0])
    m_modifiedFilePath->setText(path);
  return 1;
}


long
MiscFrame::onCreatePatch(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_modifiedFilePath->getText();
  char *modFilename = (char *) text.text();
  if (!modFilename[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No modified file specified",
        "Please specify modified file");
      return 1;
    }

  m_commandLineHandler->setOption("", true);
  FXString *filenames = m_commandLineHandler->getSelectedFilenames();
  if (filenames == NULL || filenames[0][0] == 0)
    return 1;

  char *ptr = NULL, option[FILENAME_MAX + 10];
  switch (m_patchTypes->getCurrentItem())
    {
    case 0:                                     // IPS
      ptr = "--mki";
      break;
    case 2:                                     // APS
      ptr = "--mka";
      break;
    case 3:                                     // PPF
      ptr = "--mkppf";
      break;
    }
#ifdef  __unix__
  sprintf(option, "%s=%s", ptr, filenames[0].text());
#else
  sprintf(option, "%s=\"%s\"", ptr, filenames[0].text());
#endif
  m_commandLineHandler->addOption(option);
#ifdef  __unix__
  ptr = modFilename;
#else
  sprintf(option, "\"%s\"", modFilename);
  ptr = option;
#endif
  m_commandLineHandler->addOption(ptr);
  m_commandLineHandler->addFiles(1);            // skip first file
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onChangeDescription(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_patchDescription->getText();
  char *description = (char *) text.text();
  if (!description[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No description specified",
        "Please specify a description");
      return 1;
    }

  char *ptr = NULL, option[80];
  switch (m_patchTypes->getCurrentItem())
    {
    case 2:                                     // APS
      ptr = "--na";
      break;
    case 3:                                     // PPF
      ptr = "--nppf";
      break;
    }
  if (strlen(description) >= 80)
    description[80 - 1] = 0;
#ifdef  __unix__
  sprintf(option, "%s=%s", ptr, description);
#else
  sprintf(option, "%s=\"%s\"", ptr, description);
#endif
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onChangeDescription2(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_patchDescription2Path->getText();
  char *FILE_ID_DIZPath = (char *) text.text();
  if (!FILE_ID_DIZPath[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No FILE_ID.DIZ file specified",
        "Please specify a FILE_ID.DIZ file");
      return 1;
    }

  char option[FILENAME_MAX + 10];
#ifdef  __unix__
  sprintf(option, "--idppf=%s", FILE_ID_DIZPath);
#else
  sprintf(option, "--idppf=\"%s\"", FILE_ID_DIZPath);
#endif
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onChangeDescription2Path(FXObject *, FXSelector, void *)
{
  FXString startPath = m_patchDescription2Path->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select FILE_ID.DIZ file",
    startPath);
  if (path.text()[0])
    m_patchDescription2Path->setText(path);
  return 1;
}


long
MiscFrame::onPoke(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString offsetText = m_pokeOffset->getText();
  char *offset = (char *) offsetText.text();
  if (!offset[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No offset specified",
        "Please specify an offset");
      return 1;
    }
  FXString valueText = m_pokeValue->getText();
  char *value = (char *) valueText.text();
  if (!value[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No value specified",
        "Please specify a value");
      return 1;
    }

  char option[80];
  sprintf(option, "--poke=%s:%s", offset, value);
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onPatternPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_patternFilePath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select pattern file",
    startPath);
  if (path.text()[0])
    m_patternFilePath->setText(path);
  return 1;
}


long
MiscFrame::onApplyPattern(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_patternFilePath->getText();
  char *pattern = (char *) text.text();
  if (!pattern[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No pattern file specified",
        "Please specify a pattern file");
      return 1;
    }

  char option[FILENAME_MAX];
#ifdef  __unix__
  sprintf(option, "--pattern=%s", pattern);
#else
  sprintf(option, "--pattern=\"%s\"", pattern);
#endif
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onGameGenie(FXObject *a_src, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_GameGenieCode->getText();
  char *code = (char *) text.text();
  if (!code[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No code specified",
        "Please specify a code");
      return 1;
    }

  char *ptr, option[80];
  bool fileNeeded;
  if (a_src == m_GameGenieEncodeButton)
    {
      ptr = "--gge";
      fileNeeded = false;
    }
  else if (a_src == m_GameGenieDecodeButton)
    {
      ptr = "--ggd";
      fileNeeded = false;
    }
  else // if (a_src == m_GameGenieApplyButton)
    {
      ptr = "--gg";
      fileNeeded = true;
    }
  sprintf(option, "%s=%s", ptr, code);
  m_commandLineHandler->setOption(option, fileNeeded);

  switch (m_GameGenieConsoles->getCurrentItem())
    {
    case 0:
      ptr = "--sms";
      break;
    case 1:
      ptr = "--gen";
      break;
    case 2:
      ptr = "--gb";
      break;
    case 3:
      ptr = "--nes";
      break;
    case 4:
      ptr = "--snes";
      break;
    }
  if (a_src != m_GameGenieApplyButton || m_useGameGenieConsoles->getCheck())
    m_commandLineHandler->addOption(ptr);

  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onSearchString(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_searchString->getText();
  char *searchString = (char *) text.text();
  if (!searchString[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No search string specified",
        "Please specify a search string");
      return 1;
    }

  char option[800], *findOption;

  if (m_searchTypes[0]->getCheck())
    findOption = "--find";
  else if (m_searchTypes[1]->getCheck())
    findOption = "--findi";
  else // if (m_searchTypes[2]->getCheck())
    findOption = "--findr";

  if (strlen(searchString) >= 800)
    searchString[800 - 1] = 0;
#ifdef  __unix__
  sprintf(option, "%s=%s", findOption, searchString);
#else
  sprintf(option, "%s=\"%s\"", findOption, searchString);
#endif
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onSearchType(FXObject *a_src, FXSelector, void *)
{
  m_searchTypes[0]->setCheck(a_src == m_searchTypes[0]);
  m_searchTypes[1]->setCheck(a_src == m_searchTypes[1]);
  m_searchTypes[2]->setCheck(a_src == m_searchTypes[2]);
  return 1;
}


void
MiscFrame::updateRenameTypes(void)
{
  FXint console = m_DATConsoles->getCurrentItem();
  m_renameTypes->clearItems();
  if (console == 1 || console == 2 || console == 3 ||   // GBA, Game Boy, Genesis
      console == 5 || console == 8 || console == 9 ||   // Lynx, NES, Nintendo 64
      console == 12)                                    // SNES
    {
      m_renameTypes->appendItem(m_renameTypesText[0]);
      m_renameTypes->appendItem(m_renameTypesText[1]);
      m_renameTypes->appendItem(m_renameTypesText[2]);
      m_renameTypes->appendItem(m_renameTypesText[3]);
      m_renameTypes->setNumVisible(4);
    }
  else                                    // other consoles don't use internal names
    {
      m_renameTypes->appendItem(m_renameTypesText[0]);
      m_renameTypes->appendItem(m_renameTypesText[2]);
      m_renameTypes->appendItem(m_renameTypesText[3]);
      m_renameTypes->setNumVisible(3);
    }
}


void
MiscFrame::setDATFilename(void)
{
  char suggestedFilename[FILENAME_MAX], *consolePrefix = "";
  if (m_useDATConsoles->getCheck())
    {
      FXint console = m_DATConsoles->getCurrentItem();
      switch (console)
        {
        case 0:
          consolePrefix = "2600";
          break;
        case 1:
          consolePrefix = "gba";
          break;
        case 2:
          consolePrefix = "gbx";
          break;
        case 3:
          consolePrefix = "gen";
          break;
        case 4:
          consolePrefix = "jag";
          break;
        case 5:
          consolePrefix = "lynx";
          break;
        case 6:
          consolePrefix = "neo-geo";
          break;
        case 7:
//          consolePrefix = "goodngpx";
          consolePrefix = "ngp";                // uCON64 2.0.0 recognizes prefix ngp
          break;
        case 8:
          consolePrefix = "nes";
          break;
        case 9:
          consolePrefix = "n64";
          break;
        case 10:
          consolePrefix = "pce";
          break;
        case 11:
          consolePrefix = "sms";
          break;
        case 12:
          consolePrefix = "snes";
          break;
        case 13:
          consolePrefix = "swan";
          break;
        }
    }
  else
    consolePrefix = "unknown";
  time_t time_t_val = time(NULL);
  struct tm *t = localtime(&time_t_val);
  sprintf(suggestedFilename, "%s-%d%d%d.dat", consolePrefix, t->tm_year + 1900,
    t->tm_mon + 1, t->tm_mday);
  m_DATFilename->setText(suggestedFilename);
}


long
MiscFrame::onUseDATConsoleType(FXObject *, FXSelector, void *a_data)
{
  if ((intptr_t) a_data)                             // 1 if checked
    {
      updateRenameTypes();
      setDATFilename();
    }
  return 1;
}


long
MiscFrame::onDATConsoleType(FXObject *, FXSelector, void *)
{
  if (!m_useDATConsoles->getCheck())
    return 1;

  updateRenameTypes();
  setDATFilename();

  return 1;
}


void
MiscFrame::setDATConsoleOption(void)
{
  if (!m_useDATConsoles->getCheck())
    return;

  switch (m_DATConsoles->getCurrentItem())
    {
    case 0:
      m_commandLineHandler->addOption("--ata");
      break;
    case 1:
      m_commandLineHandler->addOption("--gba");
      break;
    case 2:
      m_commandLineHandler->addOption("--gb");
      break;
    case 3:
      m_commandLineHandler->addOption("--gen");
      break;
    case 4:
      m_commandLineHandler->addOption("--jag");
      break;
    case 5:
      m_commandLineHandler->addOption("--lynx");
      break;
    case 6:
      m_commandLineHandler->addOption("--ng");
      break;
    case 7:
      m_commandLineHandler->addOption("--ngp");
      break;
    case 8:
      m_commandLineHandler->addOption("--nes");
      break;
    case 9:
      m_commandLineHandler->addOption("--n64");
      break;
    case 10:
      m_commandLineHandler->addOption("--pce");
      break;
    case 11:
      m_commandLineHandler->addOption("--sms");
      break;
    case 12:
      m_commandLineHandler->addOption("--snes");
      break;
    case 13:
      m_commandLineHandler->addOption("--swan");
      break;
    }
}


long
MiscFrame::onDATInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--db", false);
  setDATConsoleOption();
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onCreateDAT(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_DATFilename->getText();
  char *DATFilename = (char *) text.text();
  if (!DATFilename[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No DAT file name specified",
        "Please specify a DAT file name");
      return 1;
    }

  char option[FILENAME_MAX + 80];
  sprintf(option, "--mkdat=%s", DATFilename);
  m_commandLineHandler->setOption(option, true);
  setDATConsoleOption();
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onSearchCRC32(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_searchCRC32->getText();
  char *CRC32Value = (char *) text.text();
  if (!CRC32Value[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No CRC32 value specified",
        "Please specify a CRC32 value");
      return 1;
    }
  char option[80];
  sprintf(option, "--dbs=%s", CRC32Value);
  m_commandLineHandler->setOption(option, false);
  setDATConsoleOption();
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onRename(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *ptr = NULL;
  if (m_renameTypes->getNumItems() == 3) // consoles that don't use an internal name
    switch (m_renameTypes->getCurrentItem())
      {
        case 0:
          ptr = "--rename";
          break;
        case 1:
          ptr = "--rl";
          break;
        case 2:
          ptr = "--ru";
          break;
      }
  else
    switch (m_renameTypes->getCurrentItem())
      {
        case 0:
          ptr = "--rename";
          break;
        case 1:
          ptr = "--rrom";
          break;
        case 2:
          ptr = "--rl";
          break;
        case 3:
          ptr = "--ru";
          break;
      }

  m_commandLineHandler->setOption(ptr, true);
  setDATConsoleOption();
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onCalcChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *option = NULL;
  switch (m_checksumTypes->getCurrentItem())
    {
    case 0:
      option = "--crc";
      break;
    case 1:
      option = "--md5";
      break;
    case 2:
      option = "--sha1";
      break;
    }
  m_commandLineHandler->setOption(option, true);
  if (m_overrideHeaderSize->getCheck())
    {
      char option2[80];
      FXString text = m_headerSize->getText();
      sprintf(option2, "--hdn=%s", text.text());
      m_commandLineHandler->addOption(option2);
    }
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onOverrideHeaderSize(FXObject *, FXSelector, void *)
{
  if (m_overrideHeaderSize->getCheck())
    m_headerSize->enable();
  else
    m_headerSize->disable();
  return 1;
}


long
MiscFrame::onCheckPad(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--ispad", true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onPadNextMB(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--pad", true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onPadToSize(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_padNewSize->getText();
  char *size = (char *) text.text();
  if (!size[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No size specified",
        "Please specify a size");
      return 1;
    }
  char option[80];
  sprintf(option, "--padn=%s", size);
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onStripEnd(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_stripEnd->getText();
  char *nBytes = (char *) text.text();
  if (!nBytes[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No value specified",
        "Please specify a number of bytes to strip from end of file");
      return 1;
    }
  char option[80];
  sprintf(option, "--strip=%s", nBytes);
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onStripStart(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_stripStart->getText();
  char *nBytes = (char *) text.text();
  if (!nBytes[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No value specified",
        "Please specify a number of bytes to strip from start of file");
      return 1;
    }
  char option[80];
  sprintf(option, "--stpn=%s", nBytes);
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}


long
MiscFrame::onInsertStart(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_insertStart->getText();
  char *nBytes = (char *) text.text();
  if (!nBytes[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No value specified",
        "Please specify a number of bytes to insert at start of file");
      return 1;
    }
  char option[80];
  sprintf(option, "--insn=%s", nBytes);
  m_commandLineHandler->setOption(option, true);
  m_commandLineHandler->run();
  return 1;
}
