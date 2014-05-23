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
#include "CommandLineHandler.h"
#include "ComboBox.h"
#include "FileDialog.h"
#include "MultiGameFileSelector.h"
#include "Frontend.h"
#include "genesis.h"


FXDEFMAP(GenesisFrame) GenesisFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_DEVICETYPE, GenesisFrame::onDeviceType),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_TRANSFERDATATYPE, GenesisFrame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_SEND, GenesisFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_RECEIVE, GenesisFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_TEST, GenesisFrame::onTest),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_TESTTYPE, GenesisFrame::onTestType),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_MULTIGAMEPATH, GenesisFrame::onMultiGamePath),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_MULTIGAME, GenesisFrame::onMultiGame),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_DATATYPE, GenesisFrame::onDataType),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_CONVERT, GenesisFrame::onConvert),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_SPLIT, GenesisFrame::onSplit),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_JOIN, GenesisFrame::onJoin),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_SHOWINFO, GenesisFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_STRIPHEADER, GenesisFrame::onStripHeader),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_FIXCHECKSUM, GenesisFrame::onFixChecksum),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_REGIONFIX, GenesisFrame::onRegionFix),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_NAME, GenesisFrame::onName),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_1991, GenesisFrame::on1991),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_OVERRIDE, GenesisFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, GenesisFrame::ID_HEADER1, GenesisFrame::ID_HEADER3, GenesisFrame::onHeader),
  FXMAPFUNC(SEL_COMMAND, GenesisFrame::ID_INTERLEAVE, GenesisFrame::onInterleave)
};

FXIMPLEMENT(GenesisFrame, FXVerticalFrame, GenesisFrameMap, ARRAYNUMBER(GenesisFrameMap))


GenesisFrame::GenesisFrame(CommandLineHandler *a_commandLineHandler,
    FXComposite *a_p, FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h,
    FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXVerticalFrame(a_p, a_opts, a_x, a_y, a_w, a_h, a_pl, a_pr, a_pt, a_pb,
        a_hs, a_vs)
{
  m_commandLineHandler = a_commandLineHandler;

  Frontend *app = (Frontend *) getApp();
  FXFont *smallFont = app->getSmallFont();
  FXFont *boldFont = app->getBoldFont();

  FXHorizontalFrame *windowArea = new FXHorizontalFrame(this, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXVerticalFrame *leftArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

#ifndef DISABLE_COPIER
  FXGroupBox *transferArea = new FXGroupBox(leftArea, "Data transfer",
    FRAME_GROOVE | LAYOUT_FILL);
  transferArea->setFont(boldFont);
  FXHorizontalFrame *transferAreaTop = new FXHorizontalFrame(transferArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXMatrix *transferAreaTopLeft = new FXMatrix(transferAreaTop, 2,
    MATRIX_BY_COLUMNS, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(transferAreaTopLeft, "Device:");
  m_deviceTypes = new ComboBox(transferAreaTopLeft, 1, this, ID_DEVICETYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_deviceTypes->appendItem("Super Magic Drive");
  m_deviceTypes->appendItem("MD-PRO");
  m_deviceTypes->appendItem("M. Pavone's cable");
  m_deviceTypes->appendItem("Cyan's ROM copier");
  m_deviceTypes->setNumVisible(4);
  new FXLabel(transferAreaTopLeft, "Data type:");
  m_transferDataTypes = new ComboBox(transferAreaTopLeft, 1, this, ID_TRANSFERDATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_transferDataTypes->appendItem("ROM");
  m_transferDataTypes->appendItem("SRAM");
  m_transferDataTypes->setNumVisible(2);

  FXMatrix *transferAreaTopRight = new FXMatrix(transferAreaTop, 2,
    MATRIX_BY_COLUMNS | LAYOUT_RIGHT, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_speedLabel = new FXLabel(transferAreaTopRight, "Speed:");
  m_speed = new ComboBox(transferAreaTopRight, 2, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC);// | LAYOUT_FILL_X);
  m_speed->appendItem("1");
  m_speed->appendItem("2");
  m_speed->appendItem("3");
  m_speed->appendItem("4");
  m_speed->setCurrentItem(2);
  m_speed->setNumVisible(4);
  m_bankLabel = new FXLabel(transferAreaTopRight, "Bank:");
  m_bank = new FXTextField(transferAreaTopRight, 2, NULL, 0, FRAME_SUNKEN);

  FXHorizontalFrame *transferAreaBottom = new FXHorizontalFrame(transferArea,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *transferButtonArea = new FXHorizontalFrame(transferAreaBottom,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_sendButton = new FXButton(transferButtonArea, "Send", NULL, this, ID_SEND);
  FXButton *receiveButton = new FXButton(transferButtonArea, "Receive", NULL, this, ID_RECEIVE);
#ifdef  __unix__
  receiveButton->setTipText("Use quotes only if the file name should contain them");
#else
  (void) receiveButton;
#endif
  m_testButton = new FXButton(transferButtonArea, "Run test", NULL, this, ID_TEST);
  m_test = new ComboBox(transferAreaBottom, 6, this, ID_TESTTYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC);// | LAYOUT_FILL_X);
  m_test->appendItem("burn-in");
  m_test->appendItem("testbench");
  m_test->setNumVisible(2);
#endif // DISABLE_COPIER

  FXGroupBox *multiGameArea = new FXGroupBox(leftArea, "Multi-game",
    FRAME_GROOVE | LAYOUT_FILL);
  multiGameArea->setFont(boldFont);
  FXHorizontalFrame *loaderArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_loaderPath = TextFieldOption::add(loaderArea, "Loader:", 1,
    true, this, ID_MULTIGAMEPATH, false, "Genesis Settings", "Loader", "");
  FXHorizontalFrame *multiGameNameArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(multiGameNameArea, "Multi-game to create:");
  m_multiGameName = new FXTextField(multiGameNameArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  m_multiGameName->setTipText("Specify a file name without a path");
  FXHorizontalFrame *sizeRegionArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(sizeRegionArea, "Size:");
  m_multiGameSize = new FXTextField(sizeRegionArea, 2, NULL, 0, FRAME_SUNKEN);
  m_multiGameSize->setText("32");
  new FXLabel(sizeRegionArea, "Mbit");
  FXHorizontalFrame *regionArea = new FXHorizontalFrame(sizeRegionArea,
    LAYOUT_RIGHT, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(regionArea, "Region:");
  m_region = new ComboBox(regionArea, 1, NULL, 0, FRAME_SUNKEN | COMBOBOX_STATIC);
  m_region->appendItem("unspecified");
  m_region->appendItem("force NTSC/Japan");
  m_region->appendItem("force NTSC/U.S.A.");
  m_region->appendItem("force PAL");
  m_region->appendItem("auto-detect");
  m_region->setNumVisible(5);
  FXHorizontalFrame *createArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_selectFilesButton = new FXButton(createArea, "Select files", NULL, this,
    ID_MULTIGAME, BUTTON_NORMAL | LAYOUT_CENTER_X);
  m_selectFilesButton->setTipText(
    "Select files or confirm selection for already selected files");
  new FXButton(createArea, "Create file", NULL, this, ID_MULTIGAME,
    BUTTON_NORMAL | LAYOUT_CENTER_X);

  FXGroupBox *nameArea = new FXGroupBox(leftArea, "Internal name",
    FRAME_GROOVE | LAYOUT_FILL);
  nameArea->setFont(boldFont);
  FXMatrix *changeNameArea = new FXMatrix(nameArea, 2, MATRIX_BY_COLUMNS | LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_changeForeignName = new FXButton(changeNameArea, "Change foreign name to",
    NULL, this, ID_NAME, BUTTON_NORMAL | LAYOUT_FILL_X);
  m_foreignName = new FXTextField(changeNameArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
  m_changeJapaneseName = new FXButton(changeNameArea, "Change Japanese name to",
    NULL, this, ID_NAME, BUTTON_NORMAL | LAYOUT_FILL_X);
  m_JapaneseName = new FXTextField(changeNameArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);
  FXButton *fix1991Button = new FXButton(nameArea, "1991 fix", NULL, this, ID_1991);
  fix1991Button->setTipText("foreign, Japanese names => \"(C)SEGA\", \"(C) SEGA\"");


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *conversionAreaContainer = new FXGroupBox(rightArea, "Conversion",
    FRAME_GROOVE | LAYOUT_FILL);
  conversionAreaContainer->setFont(boldFont);
  FXVerticalFrame *conversionAreaContainer2 = new FXVerticalFrame(
    conversionAreaContainer, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *conversionArea = new FXHorizontalFrame(conversionAreaContainer2,
    LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_dataTypes = new ComboBox(conversionArea, 4, this, ID_DATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC);
  m_dataTypes->appendItem("ROM");
  m_dataTypes->appendItem("SRAM");
  m_dataTypes->setNumVisible(2);
  new FXLabel(conversionArea, "to");
  m_formats = new ComboBox(conversionArea, 3, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FIX_WIDTH, 0, 0, 50, 0);
  m_formatsText[0] = "SMD";
  m_formatsText[1] = "BIN";
  m_formatsText[2] = "MGD";
  m_formats->appendItem(m_formatsText[0]);
  m_formats->appendItem(m_formatsText[1]);
  m_formats->appendItem(m_formatsText[2]);
  m_formats->setNumVisible(3);
  new FXButton(conversionArea, "Convert", NULL, this, ID_CONVERT,
    BUTTON_NORMAL | LAYOUT_RIGHT);

  FXHorizontalFrame *splitArea = new FXHorizontalFrame(conversionAreaContainer2,
    LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *leftSplitArea = new FXHorizontalFrame(splitArea,
    LAYOUT_LEFT, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(leftSplitArea, "Split", NULL, this, ID_SPLIT, BUTTON_NORMAL,
    0, 0, 0, 0, 5, 5);
  new FXLabel(leftSplitArea, "Part size:");
  m_splitSize = new FXTextField(leftSplitArea, 2, NULL, 0, FRAME_SUNKEN);
  new FXLabel(leftSplitArea, "Mbit", NULL, 0, 0, 0, 0, 0, DEFAULT_PAD, 20); // spacer
  new FXButton(splitArea, "Join", NULL, this, ID_JOIN, BUTTON_NORMAL | LAYOUT_RIGHT,
    0, 0, 0, 0, 5, 5);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS |
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Strip header", NULL, this, ID_STRIPHEADER);
  new FXButton(miscButtonArea, "Fix checksum", NULL, this, ID_FIXCHECKSUM);
  new FXButton(miscButtonArea, "Region fix", NULL, this, ID_REGIONFIX);

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideGenesis = new FXCheckButton(overrideButtonsArea,
    "File is a Genesis/Mega Drive file");
  m_overrideGenesis->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FIX_WIDTH | SEPARATOR_GROOVE,
    0, 0, 50);
  FXGroupBox *headerSizeArea = new FXGroupBox(overrideButtonsArea, "", GROUPBOX_NORMAL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXHorizontalFrame *headerSizeArea2 = new FXHorizontalFrame(headerSizeArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_overrideHeaderSize = new FXRadioButton(headerSizeArea2, "File has header, size:", this, ID_HEADER1);
  m_overrideHeaderSize->setFont(smallFont);
  m_overrideHeaderSizeText = new FXTextField(headerSizeArea2, 4, NULL, 0,
    FRAME_SUNKEN | TEXTFIELD_INTEGER);
  m_overrideHeaderSizeText->setFont(smallFont);
  m_overrideHeaderSizeText->setText("512");
  m_overrideHeader = new FXRadioButton(headerSizeArea, "File has header", this, ID_HEADER2);
  m_overrideHeader->setFont(smallFont);
  m_overrideNoHeader = new FXRadioButton(headerSizeArea, "File has no header", this, ID_HEADER3);
  m_overrideNoHeader->setFont(smallFont);
  m_headerChoice = 0;

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FIX_WIDTH | SEPARATOR_GROOVE,
    0, 0, 50);
  m_overrideNotSplit = new FXCheckButton(overrideButtonsArea, "File is not split");
  m_overrideNotSplit->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FIX_WIDTH | SEPARATOR_GROOVE,
    0, 0, 50);
  FXGroupBox *interleaveArea = new FXGroupBox(overrideButtonsArea, "", GROUPBOX_NORMAL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_overrideInterleaved = new FXRadioButton(interleaveArea, "File is interleaved (SMD)", this, ID_INTERLEAVE);
  m_overrideInterleaved->setFont(smallFont);
  m_overrideInterleaved2 = new FXRadioButton(interleaveArea, "File is interleaved (MGD)", this, ID_INTERLEAVE);
  m_overrideInterleaved2->setFont(smallFont);
  m_overrideNotInterleaved = new FXRadioButton(interleaveArea, "File is not interleaved", this, ID_INTERLEAVE);
  m_overrideNotInterleaved->setFont(smallFont);

  handle(m_useOverride, FXSEL(SEL_COMMAND, ID_OVERRIDE), (void *) 0);
}


GenesisFrame::~GenesisFrame()
{
  delete m_loaderPath;
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
GenesisFrame::writeToRegistry(void)
{
  m_loaderPath->writeToRegistry();
}


long
GenesisFrame::onDeviceType(FXObject *, FXSelector, void *)
{
  m_transferDataTypes->clearItems();
  FXint deviceType = m_deviceTypes->getCurrentItem();
  switch (deviceType)
    {
    case 0:                                     // SMD
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->appendItem("SRAM");
      m_transferDataTypes->setNumVisible(2);
      m_sendButton->enable();
      break;
    case 1:                                     // MD-PRO
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->appendItem("SRAM");
      m_transferDataTypes->setNumVisible(2);
      m_sendButton->enable();
      break;
    case 2:                                     // M. Pavone's cable
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->setNumVisible(1);
      m_sendButton->disable();
      break;
    case 3:                                     // Cyan's ROM copier
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->setNumVisible(1);
      m_sendButton->disable();
      break;
    }

  if (deviceType == 1 && m_transferDataTypes->getCurrentItem() == 1)
    {
      m_bankLabel->enable();
      m_bank->enable();
    }
  else
    {
      m_bankLabel->disable();
      m_bank->disable();
    }

  if (deviceType == 3)
    {
      m_testButton->enable();
      m_test->enable();
      if (m_test->getCurrentItem() == 0)
        {
          m_speedLabel->enable();
          m_speed->enable();
        }
      else
        {
          m_speedLabel->disable();
          m_speed->disable();
        }
    }
  else
    {
      m_speedLabel->disable();
      m_speed->disable();
      m_testButton->disable();
      m_test->disable();
    }

  return 1;
}


long
GenesisFrame::onTransferDataType(FXObject *, FXSelector, void *)
{
  if (m_deviceTypes->getCurrentItem() == 1 &&
      m_transferDataTypes->getCurrentItem() == 1)
    {
      m_bankLabel->enable();
      m_bank->enable();
    }
  else
    {
      m_bankLabel->disable();
      m_bank->disable();
    }
  return 1;
}


long
GenesisFrame::onTestType(FXObject *, FXSelector, void *)
{
  if (m_deviceTypes->getCurrentItem() == 3 && m_test->getCurrentItem() == 0)
    {
      m_speedLabel->enable();
      m_speed->enable();
    }
  else
    {
      m_speedLabel->disable();
      m_speed->disable();
    }
  return 1;
}


void
GenesisFrame::setTransferOption(bool a_fileNeeded)
{
  char option[80], tail[80];
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      strcpy(option, "--xsmd");
      if (m_transferDataTypes->getCurrentItem() == 1)
        strcat(option, "s");
      m_commandLineHandler->setOption(option, a_fileNeeded);
      break;
    case 1:
      strcpy(option, "--xmd");
      if (m_transferDataTypes->getCurrentItem() == 1)
        {
          FXString text = m_bank->getText();
          char *bank = (char *) text.text();
          if (bank[0])
            {
              sprintf(tail, "b=%s", bank);
              strcat(option, tail);
            }
          else
            strcat(option, "s");
        }
      m_commandLineHandler->setOption(option, a_fileNeeded);
      break;
    case 2:
      m_commandLineHandler->setOption("--xmcd", a_fileNeeded);
      break;
    case 3:
      m_commandLineHandler->setOption("--xcmc", a_fileNeeded);
      sprintf(option, "--xcmcm=%d", m_speed->getCurrentItem() + 1);
      m_commandLineHandler->addOption(option);
      break;
    }
}


void
GenesisFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideGenesis->getCheck())
        m_commandLineHandler->addOption("--gen");
      if (m_overrideHeaderSize->getCheck())
        {
          char option[80];
          FXString text = m_overrideHeaderSizeText->getText();
          sprintf(option, "--hdn=%s", text.text());
          m_commandLineHandler->addOption(option);
        }
      if (m_overrideHeader->getCheck())
        m_commandLineHandler->addOption("--hd");
      if (m_overrideNoHeader->getCheck())
        m_commandLineHandler->addOption("--nhd");
      if (m_overrideNotSplit->getCheck())
        m_commandLineHandler->addOption("--ns");
      if (m_overrideInterleaved->getCheck())
        m_commandLineHandler->addOption("--int");
      if (m_overrideInterleaved2->getCheck())
        m_commandLineHandler->addOption("--int2");
      if (m_overrideNotInterleaved->getCheck())
        m_commandLineHandler->addOption("--nint");
    }
}


long
GenesisFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onReceive(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = FileDialog::getSaveFilename(this, "Specify file name to write to",
    "dummy");
  char *filename = (char *) text.text();
  if (!filename[0])
    return 1;
  else if (FXFile::exists(filename))
    {
      FXMessageBox::warning(this, MBOX_OK, "File already exists",
        "Please specify a different file name");
      return 1;
    }

  setTransferOption(false);
  setOverrideOptions();
  char *ptr;
#ifdef  __unix__
  ptr = filename;
#else
  {
    char option[FILENAME_MAX + 3];
    sprintf(option, "\"%s\"", filename);
    ptr = option;
  }
#endif
  m_commandLineHandler->addOption(ptr);
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onTest(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char option[80];
  sprintf(option, "--xcmct=%d", m_test->getCurrentItem() + 1);
  m_commandLineHandler->setOption(option, false);
  if (m_test->getCurrentItem() == 0)
    {
      sprintf(option, "--xcmcm=%d", m_speed->getCurrentItem() + 1);
      m_commandLineHandler->addOption(option);
    }
//  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onMultiGamePath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_loaderPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select multi-game loader",
    startPath);
  if (path.text()[0])
    m_loaderPath->setText(path);
  return 1;
}


long
GenesisFrame::onMultiGame(FXObject *a_src, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString loaderText = m_loaderPath->getText();
  char *loader = (char *) loaderText.text();
  if (!loader[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No loader specified",
        "Please specify a loader");
      return 1;
    }
  FXString multiGameText = m_multiGameName->getText();
  char *multiGame = (char *) multiGameText.text();
  if (!multiGame[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No multi-game name specified",
        "Please specify a multi-game name");
      return 1;
    }
  FXString multiGameSizeText = m_multiGameSize->getText();
  char *multiGameSize = (char *) multiGameSizeText.text();
  if (!multiGameSize[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No multi-game size specified",
        "Please specify a multi-game size");
      return 1;
    }

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  char option[FILENAME_MAX + 8], *ptr;
  sprintf(option, "--multi=%s", multiGameSize);
  m_commandLineHandler->addOption(option);
#ifdef  __unix__
  ptr = loader;
#else
  sprintf(option, "\"%s\"", loader);
  ptr = option;
#endif
  m_commandLineHandler->addOption(ptr);

  int status;
  if (a_src == m_selectFilesButton)
    {
      Frontend *fe = (Frontend *) getApp();
      MultiGameFileSelector *fileSelector = new MultiGameFileSelector(this,
        2 * 128 * 1024, "Select files for multi-game file", fe->m_bigIcon,
        fe->m_smallIcon);
      fileSelector->setFocus();                 // only necessary on Mac OS X
      status = fileSelector->execute();
      delete fileSelector;
      if (status == 0)
        return 1;
    }

  status = m_commandLineHandler->addFiles();
  if (status == -1 || status == 0)
    return 1;
#ifdef  __unix__
  ptr = multiGame;
#else
  sprintf(option, "\"%s\"", multiGame);
  ptr = option;
#endif
  m_commandLineHandler->addOption(ptr);

  int n = m_region->getCurrentItem();
  if (n > 0)
    {
      if (n < 4)
        {
          sprintf(option, "--region=%d", n - 1);
          ptr = option;
        }
      else
        ptr = "--region=x";
      m_commandLineHandler->addOption(ptr);
    }
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onDataType(FXObject *, FXSelector, void *)
{
  m_formats->clearItems();
  switch (m_dataTypes->getCurrentItem())
    {
    case 0:                                     // ROM
      m_formats->appendItem(m_formatsText[0]);
      m_formats->appendItem(m_formatsText[1]);
      m_formats->appendItem(m_formatsText[2]);
      m_formats->setNumVisible(3);
      break;
    case 1:                                     // SRAM
      m_formats->appendItem(m_formatsText[0]);
      m_formats->setNumVisible(1);
      break;
    }
  return 1;
}


long
GenesisFrame::onConvert(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *option = NULL;
  if (m_dataTypes->getCurrentItem() == 0)
    switch (m_formats->getCurrentItem())
      {
      case 0:
        option = "--smd";
        break;
      case 1:
        option = "--bin";
        break;
      case 2:
        option = "--mgd";
        break;
      }
  else // if (m_dataTypes->getCurrentItem() == 1)
    option = "--smds";
  m_commandLineHandler->setOption(option, true);
  /*
    --smds works for Genesis/Sega Mega Drive *and* Sega Master System. uCON64
    does not detect for which system an emulator SRAM file is meant, so we need
    to specify the console type.
  */
  if (m_dataTypes->getCurrentItem() == 1)
    m_commandLineHandler->addOption("--gen");
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onSplit(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-s", true);
  FXString text = m_splitSize->getText();
  char *size = (char *) text.text();
  if (size[0])
    {
      char option[80];
      sprintf(option, "--ssize=%s", size);
      m_commandLineHandler->addOption(option);
    }
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onJoin(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-j", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onStripHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--stp", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onFixChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--chk", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onRegionFix(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-f", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onName(FXObject *a_src, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *name, option[80];
  FXString text;
  if (a_src == m_changeForeignName)
    text = m_foreignName->getText();
  else
    text = m_JapaneseName->getText();
  name = (char *) text.text();
  if (!name[0]) // don't use trim() -- name may start/end with space
    {
      FXMessageBox::warning(this, MBOX_OK, "No name specified",
        "Please specify a name");
      return 1;
    }
  if (strlen(name) >= 80)
    name[80 - 1] = 0;
  // it would be 100% correct if we used two hyphens for -n2
#ifdef  __unix__
  sprintf(option, "-n%s=%s", a_src == m_changeJapaneseName ? "2" : "", name);
#else
  sprintf(option, "-n%s=\"%s\"", a_src == m_changeJapaneseName ? "2" : "", name);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::on1991(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--1991", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GenesisFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((int) a_data)
    {
      m_overrideGenesis->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
      m_overrideNotSplit->enable();
      m_overrideInterleaved->enable();
      m_overrideInterleaved2->enable();
      m_overrideNotInterleaved->enable();
    }
  else
    {
      m_overrideGenesis->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
      m_overrideNotSplit->disable();
      m_overrideInterleaved->disable();
      m_overrideInterleaved2->disable();
      m_overrideNotInterleaved->disable();
    }
  return 1;
}


long
GenesisFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
{
  m_headerChoice = FXSELID(a_sel);
  m_overrideHeader->setCheck(a_src == m_overrideHeader);
  m_overrideNoHeader->setCheck(a_src == m_overrideNoHeader);
  if (a_src == m_overrideHeaderSize)
    {
      m_overrideHeaderSize->setCheck();
      m_overrideHeaderSizeText->enable();
    }
  else
    {
      m_overrideHeaderSize->setCheck(FALSE);
      m_overrideHeaderSizeText->disable();
    }
  return 1;
}


long
GenesisFrame::onInterleave(FXObject *a_src, FXSelector, void *)
{
  m_overrideInterleaved->setCheck(a_src == m_overrideInterleaved);
  m_overrideInterleaved2->setCheck(a_src == m_overrideInterleaved2);
  m_overrideNotInterleaved->setCheck(a_src == m_overrideNotInterleaved);
  return 1;
}

