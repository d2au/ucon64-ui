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
#include "sms.h"


FXDEFMAP(SMSFrame) SMSFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_TRANSFERDATATYPE, SMSFrame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_SEND, SMSFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_RECEIVE, SMSFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_MULTIGAMEPATH, SMSFrame::onMultiGamePath),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_MULTIGAME, SMSFrame::onMultiGame),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_DATATYPE, SMSFrame::onDataType),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_CONVERT, SMSFrame::onConvert),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_SHOWINFO, SMSFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_STRIPHEADER, SMSFrame::onStripHeader),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_FIXCHECKSUM, SMSFrame::onFixChecksum),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_OVERRIDE, SMSFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, SMSFrame::ID_HEADER1, SMSFrame::ID_HEADER3, SMSFrame::onHeader),
  FXMAPFUNC(SEL_COMMAND, SMSFrame::ID_INTERLEAVE, SMSFrame::onInterleave)
};

FXIMPLEMENT(SMSFrame, FXVerticalFrame, SMSFrameMap, ARRAYNUMBER(SMSFrameMap))


SMSFrame::SMSFrame(CommandLineHandler *a_commandLineHandler,
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
  FXMatrix *deviceDataTypeArea = new FXMatrix(transferAreaTop, 2,
    MATRIX_BY_COLUMNS | LAYOUT_BOTTOM, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0););

  new FXLabel(deviceDataTypeArea, "Device:");
  m_deviceTypes = new ComboBox(deviceDataTypeArea, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_deviceTypes->appendItem("SMS-PRO/GG-PRO");
  m_deviceTypes->setNumVisible(1);

  new FXLabel(deviceDataTypeArea, "Data type:");
  m_transferDataTypes = new ComboBox(deviceDataTypeArea, 1, this, ID_TRANSFERDATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_transferDataTypes->appendItem("ROM");
  m_transferDataTypes->appendItem("SRAM");
  m_transferDataTypes->setNumVisible(2);

  FXVerticalFrame *transferAreaTopRight = new FXVerticalFrame(transferAreaTop, LAYOUT_FILL_X,
    0, 0, 0, 0, DEFAULT_PAD, 0, DEFAULT_PAD, 0);//, 0, 0);
  new FXLabel(transferAreaTopRight, " ");        // spacer (empty string doesn't work here)
  FXHorizontalFrame *transferAreaTopRight2 = new FXHorizontalFrame(transferAreaTopRight,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_bankLabel = new FXLabel(transferAreaTopRight2, "Bank:");
  m_bank = new FXTextField(transferAreaTopRight2, 2, NULL, 0, FRAME_SUNKEN);

  FXHorizontalFrame *transferAreaBottom = new FXHorizontalFrame(transferArea,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *transferButtonArea = new FXHorizontalFrame(transferAreaBottom,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(transferButtonArea, "Send", NULL, this, ID_SEND);
  FXButton *receiveButton = new FXButton(transferButtonArea, "Receive", NULL, this, ID_RECEIVE);
#ifdef  __unix__
  receiveButton->setTipText("Use quotes only if the file name should contain them");
#else
  (void) receiveButton;
#endif
#endif // DISABLE_COPIER

  FXGroupBox *multiGameArea = new FXGroupBox(leftArea, "Multi-game",
    FRAME_GROOVE | LAYOUT_FILL);
  multiGameArea->setFont(boldFont);
  FXHorizontalFrame *loaderArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_loaderPath = TextFieldOption::add(loaderArea, "Loader:", 1,
    true, this, ID_MULTIGAMEPATH, false, "SMS Settings", "Loader", "");
  FXHorizontalFrame *multiGameNameArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(multiGameNameArea, "Multi-game to create:");
  m_multiGameName = new FXTextField(multiGameNameArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  m_multiGameName->setTipText("Specify a file name without a path");
  FXHorizontalFrame *createArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(createArea, "Size:");
  m_multiGameSize = new FXTextField(createArea, 4, NULL, 0, FRAME_SUNKEN);
  m_multiGameSize->setText("32");
  new FXLabel(createArea, "Mbit");
  m_selectFilesButton = new FXButton(createArea, "Select files", NULL, this,
    ID_MULTIGAME, BUTTON_NORMAL | LAYOUT_CENTER_X);
  m_selectFilesButton->setTipText(
    "Select files or confirm selection for already selected files");
  new FXButton(createArea, "Create file", NULL, this, ID_MULTIGAME,
    BUTTON_NORMAL | LAYOUT_CENTER_X);


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *conversionAreaContainer = new FXGroupBox(rightArea, "Conversion",
    FRAME_GROOVE | LAYOUT_FILL, 0, 0, 0, 0, 20, 20);
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
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FIX_WIDTH, 0, 0, 90, 0);
  m_formatsText[0] = "SMD";
  m_formatsText[1] = "MGD (SMS)";
  m_formatsText[2] = "MGD (GG)";
  m_formats->appendItem(m_formatsText[0]);
  m_formats->appendItem(m_formatsText[1]);
  m_formats->appendItem(m_formatsText[2]);
  m_formats->setNumVisible(3);
  new FXButton(conversionArea, "Convert", NULL, this, ID_CONVERT);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS |
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Strip header", NULL, this, ID_STRIPHEADER);
  FXButton *fixChecksum = new FXButton(miscButtonArea, "Fix checksum", NULL, this,
    ID_FIXCHECKSUM);
  fixChecksum->setTipText("Works only for SMS (not GG) files");

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideSMS = new FXCheckButton(overrideButtonsArea, "File is a Sega Master System/Game Gear file");
  m_overrideSMS->setFont(smallFont);

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
  FXGroupBox *interleaveArea = new FXGroupBox(overrideButtonsArea, "", GROUPBOX_NORMAL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_overrideInterleaved = new FXRadioButton(interleaveArea, "File is interleaved", this, ID_INTERLEAVE);
  m_overrideInterleaved->setFont(smallFont);
  m_overrideNotInterleaved = new FXRadioButton(interleaveArea, "File is not interleaved", this, ID_INTERLEAVE);
  m_overrideNotInterleaved->setFont(smallFont);

  handle(m_useOverride, FXSEL(SEL_COMMAND, ID_OVERRIDE), (void *) 0);
}


SMSFrame::~SMSFrame()
{
  delete m_loaderPath;
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
SMSFrame::writeToRegistry(void)
{
  m_loaderPath->writeToRegistry();
}


long
SMSFrame::onTransferDataType(FXObject *, FXSelector, void *)
{
  if (m_transferDataTypes->getCurrentItem() == 1)
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


void
SMSFrame::setTransferOption(bool a_fileNeeded)
{
  char option[80], tail[80];
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      strcpy(option, "--xgg");
      if (m_transferDataTypes->getCurrentItem() == 1)
        {
          FXString text = m_bank->getText().trim();
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
    }
}


void
SMSFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideSMS->getCheck())
        m_commandLineHandler->addOption("--sms");
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
      if (m_overrideInterleaved->getCheck())
        m_commandLineHandler->addOption("--int");
      if (m_overrideNotInterleaved->getCheck())
        m_commandLineHandler->addOption("--nint");
    }
}


long
SMSFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SMSFrame::onReceive(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = FileDialog::getSaveFilename(this, "Specify file name to write to",
    "dummy");
  char *filename = (char *) text.text();
  if (!filename[0])
    return 1;
  else
    {
      if (FXStat::exists(filename))
        {
          FXMessageBox::warning(this, MBOX_OK, "File already exists",
            "Please specify a different file name");
          return 1;
        }
    }

  setTransferOption(false);
//  setOverrideOptions();
  char *ptr;
#ifdef  __unix__
  ptr = filename;
#else
  char option[FILENAME_MAX + 3];
  sprintf(option, "\"%s\"", filename);
  ptr = option;
#endif
  m_commandLineHandler->addOption(ptr);
  m_commandLineHandler->run();
  return 1;
}


long
SMSFrame::onMultiGamePath(FXObject *, FXSelector, void *)
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
SMSFrame::onMultiGame(FXObject *a_src, FXSelector, void *)
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
        16 * 1024, "Select files for multi-game file", fe->m_bigIcon,
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
SMSFrame::onDataType(FXObject *, FXSelector, void *)
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
SMSFrame::onConvert(FXObject *, FXSelector, void *)
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
        option = "--mgd";
        break;
      case 2:
        option = "--mgdgg";
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
    m_commandLineHandler->addOption("--sms");
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SMSFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SMSFrame::onStripHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--stp", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SMSFrame::onFixChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--chk", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
SMSFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((int) a_data)
    {
      m_overrideSMS->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
      m_overrideInterleaved->enable();
      m_overrideNotInterleaved->enable();
    }
  else
    {
      m_overrideSMS->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
      m_overrideInterleaved->disable();
      m_overrideNotInterleaved->disable();
    }
  return 1;
}


long
SMSFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
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
SMSFrame::onInterleave(FXObject *a_src, FXSelector, void *)
{
  m_overrideInterleaved->setCheck(a_src == m_overrideInterleaved);
  m_overrideNotInterleaved->setCheck(a_src == m_overrideNotInterleaved);
  return 1;
}
