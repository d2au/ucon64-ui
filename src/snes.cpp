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
#include "SNESControllerSettings.h"
#include "snes.h"


FXDEFMAP(SNESFrame) SNESFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_DEVICETYPE, SNESFrame::onDeviceType),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_TRANSFERDATATYPE, SNESFrame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_SEND, SNESFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_RECEIVE, SNESFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_MULTIGAMEPATH, SNESFrame::onMultiGamePath),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_MULTIGAME, SNESFrame::onMultiGame),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_DATATYPE, SNESFrame::onDataType),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_FORMAT, SNESFrame::onFormat),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_CONVERT, SNESFrame::onConvert),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_CONTROLLER, SNESFrame::onController),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_SPLIT, SNESFrame::onSplit),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_JOIN, SNESFrame::onJoin),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_CRACK, SNESFrame::onCrack),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_REGIONFIX, SNESFrame::onRegionFix),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_SLOWFIX, SNESFrame::onSlowFix),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_SHOWINFO, SNESFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_DISPLAYHEADER, SNESFrame::onDisplayHeader),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_STRIPHEADER, SNESFrame::onStripHeader),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_FIXCHECKSUM, SNESFrame::onFixChecksum),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_DEMIRROR, SNESFrame::onDeMirror),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_DENSRT, SNESFrame::onDeNSRT),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_NAME, SNESFrame::onName),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_OVERRIDE, SNESFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, SNESFrame::ID_HEADER1, SNESFrame::ID_HEADER3, SNESFrame::onHeader),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_BANKSIZE, SNESFrame::onBankSize),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_INTERLEAVE, SNESFrame::onInterleave),
  FXMAPFUNC(SEL_COMMAND, SNESFrame::ID_BS, SNESFrame::onBS)
};

FXIMPLEMENT(SNESFrame, FXVerticalFrame, SNESFrameMap, ARRAYNUMBER(SNESFrameMap))


SNESFrame::SNESFrame(CommandLineHandler *a_commandLineHandler,
    FXComposite *a_p, FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h,
    FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXVerticalFrame(a_p, a_opts, a_x, a_y, a_w, a_h, a_pl, a_pr, a_pt, a_pb,
        a_hs, a_vs)
{
  m_commandLineHandler = a_commandLineHandler;

  Frontend *app = (Frontend *) getApp();
  FXFont *smallFont = app->getSmallFont();
  FXFont *tinyFont = smallFont; //app->getTinyFont();
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
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);

  FXMatrix *transferAreaTopLeft = new FXMatrix(transferAreaTop, 2,
    MATRIX_BY_COLUMNS | LAYOUT_BOTTOM, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0););
  new FXLabel(transferAreaTopLeft, "Device:");
  m_deviceTypes = new ComboBox(transferAreaTopLeft, 1, this, ID_DEVICETYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_deviceTypes->appendItem("Super Wild Card");
  m_deviceTypes->appendItem("Game Doctor SF3");
  m_deviceTypes->appendItem("Game Doctor SF6");
  m_deviceTypes->appendItem("Pro Fighter");
  m_deviceTypes->appendItem("Super Flash");
  m_deviceTypes->setNumVisible(5);
  new FXLabel(transferAreaTopLeft, "Data type:");
  m_transferDataTypes = new ComboBox(transferAreaTopLeft, 1, this, ID_TRANSFERDATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_transferDataTypesText[0] = "ROM";
  m_transferDataTypesText[1] = "ROM (RTS mode)";
  m_transferDataTypesText[2] = "SRAM (copier)";
  m_transferDataTypesText[3] = "SRAM (cartridge)";
  m_transferDataTypesText[4] = "Real-time save";
  m_transferDataTypes->appendItem(m_transferDataTypesText[0]);
  m_transferDataTypes->appendItem(m_transferDataTypesText[1]);
  m_transferDataTypes->appendItem(m_transferDataTypesText[2]);
  m_transferDataTypes->appendItem(m_transferDataTypesText[3]);
  m_transferDataTypes->appendItem(m_transferDataTypesText[4]);
  m_transferDataTypes->setNumVisible(5);

  FXGroupBox *transferAreaTopRight = new FXGroupBox(transferAreaTop, "I/O mode",
    FRAME_GROOVE | LAYOUT_RIGHT, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  transferAreaTopRight->setFont(boldFont);
  FXMatrix *IOModeArea = new FXMatrix(transferAreaTopRight, 3, MATRIX_BY_ROWS,// | LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  int n, m = 1;
  char label[80], *IOModeTipText[9] =
    {
      "MODE=0x001 force 32 Mbit dump",
      "MODE=0x002 use alternative method for determining ROM size",
      "MODE=0x004 Super FX",
      "MODE=0x008 S-DD1",
      "MODE=0x010 SA-1",
      "MODE=0x020 SPC7110",
      "MODE=0x040 DX2 trick (might work with other SWC models)",
      "MODE=0x080 Mega Man X 2",
      "MODE=0x100 dump BIOS"
    };
  for (n = 0; n < 9; n++)
    {
      sprintf(label, "%03x", m);
      m_IOMode[n] = new FXCheckButton(IOModeArea, label);
      m_IOMode[n]->setFont(tinyFont);
      m_IOMode[n]->setTipText(IOModeTipText[n]);
      m <<= 1;
    }

  FXHorizontalFrame *transferAreaBottom = new FXHorizontalFrame(transferArea,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(transferAreaBottom, "Send", NULL, this, ID_SEND);
  m_receiveButton = new FXButton(transferAreaBottom, "Receive", NULL, this, ID_RECEIVE);
#ifdef  __unix__
#if 1
  m_receiveButton->setTipText("Use quotes only if the file name should contain them");
#else
  FXLabel *quoteNote = new FXLabel(transferArea, "Note: Use quotes only if the file name should contain them");
  quoteNote->setFont(smallFont);
#endif
#endif
#else
  (void) tinyFont;
#endif // DISABLE_COPIER

  FXGroupBox *multiGameArea = new FXGroupBox(leftArea, "Multi-game",
    FRAME_GROOVE | LAYOUT_FILL);
  multiGameArea->setFont(boldFont);
  FXHorizontalFrame *loaderArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_loaderPath = TextFieldOption::add(loaderArea, "Loader:", 1,
    true, this, ID_MULTIGAMEPATH, false, "SNES Settings", "Loader", "");
  FXHorizontalFrame *multiGameNameArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(multiGameNameArea, "Multi-game to create:");
  m_multiGameName = new FXTextField(multiGameNameArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  m_multiGameName->setTipText("Specify a file name without a path");
  FXHorizontalFrame *createArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(createArea, "Size:");
  m_multiGameSize = new FXTextField(createArea, 3, NULL, 0, FRAME_SUNKEN);
  m_multiGameSize->setText("64");
  new FXLabel(createArea, "Mbit");
  m_selectFilesButton = new FXButton(createArea, "Select files", NULL, this,
    ID_MULTIGAME, BUTTON_NORMAL | LAYOUT_CENTER_X);
  m_selectFilesButton->setTipText(
    "Select files or confirm selection for already selected files");
  new FXButton(createArea, "Create file", NULL, this, ID_MULTIGAME,
    BUTTON_NORMAL | LAYOUT_CENTER_X);

  FXGroupBox *crackArea = new FXGroupBox(leftArea, "Copy protection",
    FRAME_GROOVE | LAYOUT_FILL);
  crackArea->setFont(boldFont);
  FXHorizontalFrame *crackButtonsArea = new FXHorizontalFrame(crackArea,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(crackButtonsArea, "Crack", NULL, this, ID_CRACK);
  new FXButton(crackButtonsArea, "Region fix", NULL, this, ID_REGIONFIX);
  new FXButton(crackButtonsArea, "SlowROM fix", NULL, this, ID_SLOWFIX);

  FXGroupBox *conversionAreaContainer = new FXGroupBox(leftArea, "Conversion",
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
  m_formats = new ComboBox(conversionArea, 10, this, ID_FORMAT,
    FRAME_SUNKEN | COMBOBOX_STATIC);
  m_formatsText[0] = "SWC";
  m_formatsText[1] = "SMC";
  m_formatsText[2] = "FIG";
  m_formatsText[3] = "GD3";
  m_formatsText[4] = "UFO";
  m_formatsText[5] = "MGD/MGH";
  m_formats->appendItem(m_formatsText[0]);
  m_formats->appendItem(m_formatsText[1]);
  m_formats->appendItem(m_formatsText[2]);
  m_formats->appendItem(m_formatsText[3]);
  m_formats->appendItem(m_formatsText[4]);
  m_formats->appendItem(m_formatsText[5]);
  m_formats->setNumVisible(6);
  new FXButton(conversionArea, "Convert", NULL, this, ID_CONVERT,
    BUTTON_NORMAL | LAYOUT_RIGHT);

  FXHorizontalFrame *controllerArea = new FXHorizontalFrame(conversionAreaContainer2,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_controllerButton = new FXButton(controllerArea, "Controller settings", NULL,
    this, ID_CONTROLLER);
  m_controllerSettings = new SNESControllerSettings(this, "Controller settings");

  // Splitting a file or joining files can be seen as a conversion. Anyway,
  //  it's related (UFO/GD HiROM).
  FXHorizontalFrame *splitArea = new FXHorizontalFrame(conversionAreaContainer2,
    LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *leftSplitArea = new FXHorizontalFrame(splitArea, LAYOUT_LEFT,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(leftSplitArea, "Split", NULL, this, ID_SPLIT, BUTTON_NORMAL,
    0, 0, 0, 0, 5, 5);
  new FXLabel(leftSplitArea, "Part size:");
  m_splitSize = new FXTextField(leftSplitArea, 2, NULL, 0, FRAME_SUNKEN);
  new FXLabel(leftSplitArea, "Mbit", NULL, 0, 0, 0, 0, 0, DEFAULT_PAD, 30); // spacer
  new FXButton(splitArea, "Join", NULL, this, ID_JOIN, BUTTON_NORMAL | LAYOUT_RIGHT,
    0, 0, 0, 0, 5, 5);


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS | PACK_UNIFORM_WIDTH,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Show header", NULL, this, ID_DISPLAYHEADER);
  new FXButton(miscButtonArea, "Strip header", NULL, this, ID_STRIPHEADER);
  new FXButton(miscButtonArea, "Fix checksum", NULL, this, ID_FIXCHECKSUM);
  new FXButton(miscButtonArea, "De-mirror", NULL, this, ID_DEMIRROR);
  new FXButton(miscButtonArea, "De-NSRT", NULL, this, ID_DENSRT);

  FXHorizontalFrame *changeNameArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(changeNameArea, "Change name to", NULL, this, ID_NAME);
  m_internalName = new FXTextField(changeNameArea, 21, NULL, 0, FRAME_SUNKEN);

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideSNES = new FXCheckButton(overrideButtonsArea, "File is a SNES file");
  m_overrideSNES->setFont(smallFont);

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
  FXGroupBox *bankSizeArea = new FXGroupBox(overrideButtonsArea, "", GROUPBOX_NORMAL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_overrideHiROM = new FXRadioButton(bankSizeArea, "File is HiROM", this, ID_BANKSIZE);
  m_overrideHiROM->setFont(smallFont);
  m_overrideLoROM = new FXRadioButton(bankSizeArea, "File is LoROM", this, ID_BANKSIZE);
  m_overrideLoROM->setFont(smallFont);
  m_overrideExtended = new FXCheckButton(overrideButtonsArea, "File is Extended ROM");
  m_overrideExtended->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FIX_WIDTH | SEPARATOR_GROOVE,
    0, 0, 50);
  FXGroupBox *interleaveArea = new FXGroupBox(overrideButtonsArea, "", GROUPBOX_NORMAL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_overrideInterleaved = new FXRadioButton(interleaveArea, "File is interleaved", this, ID_INTERLEAVE);
  m_overrideInterleaved->setFont(smallFont);
  m_overrideSFXInterleaved = new FXRadioButton(interleaveArea, "File is SFX interleaved", this, ID_INTERLEAVE);
  m_overrideSFXInterleaved->setFont(smallFont);
  m_overrideNotInterleaved = new FXRadioButton(interleaveArea, "File is not interleaved", this, ID_INTERLEAVE);
  m_overrideNotInterleaved->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FIX_WIDTH | SEPARATOR_GROOVE,
    0, 0, 50);
  FXGroupBox *BSArea = new FXGroupBox(overrideButtonsArea, "", GROUPBOX_NORMAL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_overrideBSDump = new FXRadioButton(BSArea, "File is BS dump", this, ID_BS);
  m_overrideBSDump->setFont(smallFont);
  m_overrideRegCartDump = new FXRadioButton(BSArea, "File is regular cartridge dump", this, ID_BS);
  m_overrideRegCartDump->setFont(smallFont);

  handle(m_useOverride, FXSEL(SEL_COMMAND, ID_OVERRIDE), (void *) 0);
}


SNESFrame::~SNESFrame()
{
  delete m_loaderPath;
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
SNESFrame::writeToRegistry(void)
{
  m_loaderPath->writeToRegistry();
}


void
SNESFrame::setReceiveButtonState(FXint a_deviceType, FXint a_transferDataType)
{
  if (a_deviceType == 1)                        // GDSF3
    m_receiveButton->disable();
  else if (a_deviceType == 2)                   // GDSF6
    {
      if (a_transferDataType == 0)
        m_receiveButton->disable();
      else
        m_receiveButton->enable();
    }
  else
    m_receiveButton->enable();
}


long
SNESFrame::onDeviceType(FXObject *, FXSelector, void *)
{
  m_transferDataTypes->clearItems();
  FXint deviceType = m_deviceTypes->getCurrentItem();
  switch (deviceType)
    {
    case 0:                                     // SWC
      m_transferDataTypes->appendItem(m_transferDataTypesText[0]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[1]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[2]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[3]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[4]);
      m_transferDataTypes->setNumVisible(5);
      break;
    case 1:                                     // GDSF3
    case 2:                                     // GDSF6
      m_transferDataTypes->appendItem(m_transferDataTypesText[0]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[2]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[4]);
      m_transferDataTypes->setNumVisible(3);
      break;
    case 3:                                     // FIG
      m_transferDataTypes->appendItem(m_transferDataTypesText[0]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[2]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[3]);
      m_transferDataTypes->setNumVisible(3);
      break;
    case 4:                                     // SF
      m_transferDataTypes->appendItem(m_transferDataTypesText[0]);
      m_transferDataTypes->appendItem(m_transferDataTypesText[2]);
      m_transferDataTypes->setNumVisible(2);
      break;
    }

  int n;
  // uCON64 2.0.0 knows only about _SWC_ I/O modes
  if (deviceType == 0)
    for (n = 0; n < 9; n++)
      m_IOMode[n]->enable();
  else
    for (n = 0; n < 9; n++)
      m_IOMode[n]->disable();

  setReceiveButtonState(deviceType, m_transferDataTypes->getCurrentItem());

  return 1;
}


long
SNESFrame::onTransferDataType(FXObject *, FXSelector, void *)
{
  setReceiveButtonState(m_deviceTypes->getCurrentItem(),
    m_transferDataTypes->getCurrentItem());
  return 1;
}


void
SNESFrame::setTransferOption(bool a_fileNeeded)
{
  char option[80];
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      strcpy(option, "--xswc");
      switch (m_transferDataTypes->getCurrentItem())
        {
        case 1:
          strcat(option, "2");
          break;
        case 2:
          strcat(option, "s");
          break;
        case 3:
          strcat(option, "c");
          break;
        case 4:
          strcat(option, "r");
          break;
        }
      break;
    case 1:
      strcpy(option, "--xgd3");
      switch (m_transferDataTypes->getCurrentItem())
        {
        case 1:
          strcat(option, "s");
          break;
        case 2:
          strcat(option, "r");
          break;
        }
      break;
    case 2:
      strcpy(option, "--xgd6");
      switch (m_transferDataTypes->getCurrentItem())
        {
        case 1:
          strcat(option, "s");
          break;
        case 2:
          strcat(option, "r");
          break;
        }
      break;
    case 3:
      strcpy(option, "--xfig");
      switch (m_transferDataTypes->getCurrentItem())
        {
        case 1:
          strcat(option, "s");
          break;
        case 2:
          strcat(option, "c");
          break;
        }
      break;
    case 4:
      strcpy(option, "--xsf");
      switch (m_transferDataTypes->getCurrentItem())
        {
        case 1:
          strcat(option, "s");
          break;
        }
      break;
    }
  m_commandLineHandler->setOption(option, a_fileNeeded);
}


void
SNESFrame::setIOMode(void)
{
  if (m_deviceTypes->getCurrentItem() == 0)
    {
      // uCON64 2.0.0 knows only about _SWC_ I/O modes
      int n, m = 0;

      for (n = 0; n < 9; n++)
        if (m_IOMode[n]->getCheck())
          m |= 1 << n;
      if (m)
        {
          char option[80];
          sprintf(option, "--xswc-io=%#03x", m);
          m_commandLineHandler->addOption(option);
        }
    }
}


void
SNESFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideSNES->getCheck())
        m_commandLineHandler->addOption("--snes");
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
      if (m_overrideHiROM->getCheck())
        m_commandLineHandler->addOption("--hi");
      if (m_overrideLoROM->getCheck())
        m_commandLineHandler->addOption("--nhi");
      if (m_overrideExtended->getCheck())
        m_commandLineHandler->addOption("--erom");
      if (m_overrideInterleaved->getCheck())
        m_commandLineHandler->addOption("--int");
      if (m_overrideSFXInterleaved->getCheck())
        m_commandLineHandler->addOption("--int2");
      if (m_overrideNotInterleaved->getCheck())
        m_commandLineHandler->addOption("--nint");
      if (m_overrideBSDump->getCheck())
        m_commandLineHandler->addOption("--bs");
      if (m_overrideRegCartDump->getCheck())
        m_commandLineHandler->addOption("--nbs");
    }
}


long
SNESFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(true);
  setIOMode();
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onReceive(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  /*
    IMPORTANT: We have to keep a reference to the FXString object or else the
               returned pointer may become invalid. Usually it's safe to obtain
               the pointer in one step, but in this case it's not. The reason
               seems to be that the memory of a temporary FXString object can
               be used by realloc() in CommandLineHandler::addOption().
  */
  FXString text = FileDialog::getSaveFilename(this, "Specify file name to write to",
    "dummy");
  char *filename = (char *) text.text();
  if (!filename[0])
    {
//      FXMessageBox::warning(this, MBOX_OK, "No file name specified", "Please specify a file name");
      return 1;
    }
  else
    {
      // TODO?: Strip quotes from string
      if (FXStat::exists(filename))
        {
          FXMessageBox::warning(this, MBOX_OK, "File already exists",
            "Please specify a different file name");
          return 1;
        }
    }

  setTransferOption(false);
  setIOMode();
  setOverrideOptions();
  char *ptr;
#ifdef  __unix__
  ptr = filename;
#else // quote filename under Windows (necessary if it contains one or more spaces)
  char option[FILENAME_MAX + 3];
  sprintf(option, "\"%s\"", filename);
  ptr = option;
#endif
  m_commandLineHandler->addOption(ptr);
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onMultiGamePath(FXObject *, FXSelector, void *)
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
SNESFrame::onMultiGame(FXObject *a_src, FXSelector, void *)
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
        16 * 128 * 1024, "Select files for multi-game file", fe->m_bigIcon,
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
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onCrack(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-k", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onRegionFix(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-f", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onSlowFix(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-l", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onDataType(FXObject *, FXSelector, void *)
{
  m_formats->clearItems();
  switch (m_dataTypes->getCurrentItem())
    {
    case 0:                                     // ROM
      m_formats->appendItem(m_formatsText[0]);
      m_formats->appendItem(m_formatsText[1]);
      m_formats->appendItem(m_formatsText[2]);
      m_formats->appendItem(m_formatsText[3]);
      m_formats->appendItem(m_formatsText[4]);
      m_formats->appendItem(m_formatsText[5]);
      m_formats->setNumVisible(6);
      m_controllerButton->enable();
      break;
    case 1:                                     // SRAM
      m_formats->appendItem(m_formatsText[0]);
      m_formats->appendItem(m_formatsText[2]);
      m_formats->appendItem(m_formatsText[3]);
      m_formats->appendItem(m_formatsText[4]);
      m_formats->setNumVisible(4);
      m_controllerButton->disable();
      break;
    }
  return 1;
}


long
SNESFrame::onFormat(FXObject *, FXSelector, void *)
{
  if (m_dataTypes->getCurrentItem() == 0)
    {
      if (m_formats->getCurrentItem() == 5)
        m_controllerButton->disable();
      else
        m_controllerButton->enable();
    }
  return 1;
}


long
SNESFrame::onConvert(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *option = NULL;
  int dataType = m_dataTypes->getCurrentItem(),
      format = m_formats->getCurrentItem();
  if (dataType == 0)
    switch (format)
      {
      case 0:
        option = "--swc";
        break;
      case 1:
        option = "--smc";
        break;
      case 2:
        option = "--fig";
        break;
      case 3:
        option = "--gd3";
        break;
      case 4:
        option = "--ufo";
        break;
      case 5:
        option = "--mgd";
        break;
      }
  else // if (dataType == 1)
    switch (format)
      {
      case 0:
        option = "--swcs";
        break;
      case 1:
        option = "--figs";
        break;
      case 2:
        option = "--gd3s";
        break;
      case 3:
        option = "--ufos";
        break;
      }
  m_commandLineHandler->setOption(option, true);
  if (dataType == 0 && format != 5 && m_controllerSettings->useSettings())
    {
      char option2[80];
      // uCON64 will set both controller fields even if only one controller
      //  switch is used, so we can just as well use both controller switches
      sprintf(option2, "--ctrl=%d", m_controllerSettings->getController1Type());
      m_commandLineHandler->addOption(option2);
      sprintf(option2, "--ctrl2=%d", m_controllerSettings->getController2Type());
      m_commandLineHandler->addOption(option2);
    }
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onController(FXObject *, FXSelector, void *)
{
  m_controllerSettings->setFocus();
  m_controllerSettings->execute();
  return 1;
}


long
SNESFrame::onSplit(FXObject *, FXSelector, void *)
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
SNESFrame::onJoin(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("-j", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onDisplayHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--dbuh", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onStripHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--stp", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onFixChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--chk", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onDeMirror(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--dmirr", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onDeNSRT(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--dnsrt", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onName(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_internalName->getText();
  char *name = (char *) text.text(), option[80];
  if (!name[0]) // don't use trim() -- name may start/end with space
    {
      FXMessageBox::warning(this, MBOX_OK, "No name specified",
        "Please specify a name");
      return 1;
    }
  if (strlen(name) >= 80)
    name[80 - 1] = 0;
#ifdef  __unix__
  sprintf(option, "-n=%s", name);
#else
  sprintf(option, "-n=\"%s\"", name);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SNESFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((int) a_data)
    {
      m_overrideSNES->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
      m_overrideNotSplit->enable();
      m_overrideHiROM->enable();
      m_overrideLoROM->enable();
      m_overrideExtended->enable();
      m_overrideInterleaved->enable();
      m_overrideSFXInterleaved->enable();
      m_overrideNotInterleaved->enable();
      m_overrideBSDump->enable();
      m_overrideRegCartDump->enable();
    }
  else
    {
      m_overrideSNES->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
      m_overrideNotSplit->disable();
      m_overrideHiROM->disable();
      m_overrideLoROM->disable();
      m_overrideExtended->disable();
      m_overrideInterleaved->disable();
      m_overrideSFXInterleaved->disable();
      m_overrideNotInterleaved->disable();
      m_overrideBSDump->disable();
      m_overrideRegCartDump->disable();
    }
  return 1;
}


long
SNESFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
{
  m_headerChoice = FXSELID(a_sel);
  // We don't need a handler for SEL_UPDATE:
  m_overrideHeader->setCheck(a_src == m_overrideHeader);
  m_overrideNoHeader->setCheck(a_src == m_overrideNoHeader);
  // It's not necessary to to set FXRadioButtons, but it is necessary to clear them
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
SNESFrame::onBankSize(FXObject *a_src, FXSelector, void *)
{
  m_overrideHiROM->setCheck(a_src == m_overrideHiROM);
  m_overrideLoROM->setCheck(a_src == m_overrideLoROM);
  return 1;
}


long
SNESFrame::onInterleave(FXObject *a_src, FXSelector, void *)
{
  m_overrideInterleaved->setCheck(a_src == m_overrideInterleaved);
  m_overrideSFXInterleaved->setCheck(a_src == m_overrideSFXInterleaved);
  m_overrideNotInterleaved->setCheck(a_src == m_overrideNotInterleaved);
  return 1;
}


long
SNESFrame::onBS(FXObject *a_src, FXSelector, void *)
{
  m_overrideBSDump->setCheck(a_src == m_overrideBSDump);
  m_overrideRegCartDump->setCheck(a_src == m_overrideRegCartDump);
  return 1;
}
