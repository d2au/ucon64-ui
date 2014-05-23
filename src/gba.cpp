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
#include "gba.h"


FXDEFMAP(GBAFrame) GBAFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_DEVICETYPE, GBAFrame::onDeviceType),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_TRANSFERDATATYPE, GBAFrame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_SEND, GBAFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_RECEIVE, GBAFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_MULTIGAMEPATH, GBAFrame::onMultiGamePath),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_MULTIGAME, GBAFrame::onMultiGame),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_SRAMFIX, GBAFrame::onSRAMFix),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_CRASHFIX, GBAFrame::onCrashFix),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_SHOWINFO, GBAFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_FIXCHECKSUM, GBAFrame::onFixChecksum),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_FIXLOGO, GBAFrame::onFixLogo),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_NAME, GBAFrame::onName),
  FXMAPFUNC(SEL_COMMAND, GBAFrame::ID_OVERRIDE, GBAFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, GBAFrame::ID_HEADER1, GBAFrame::ID_HEADER3, GBAFrame::onHeader)
};

FXIMPLEMENT(GBAFrame, FXVerticalFrame, GBAFrameMap, ARRAYNUMBER(GBAFrameMap))


GBAFrame::GBAFrame(CommandLineHandler *a_commandLineHandler,
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
  m_deviceTypes = new ComboBox(deviceDataTypeArea, 1, this, ID_DEVICETYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_deviceTypes->appendItem("Flash Advance Linker");
  m_deviceTypes->appendItem("Flash 2 Advance (parport)");
#ifdef  __linux__                               // yes, Linux
  m_deviceTypes->appendItem("Flash 2 Advance (USB)");
  m_deviceTypes->setNumVisible(3);
#else
  m_deviceTypes->setNumVisible(2);
#endif

  new FXLabel(deviceDataTypeArea, "Data type:");
  m_transferDataTypes = new ComboBox(deviceDataTypeArea, 1, this, ID_TRANSFERDATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_transferDataTypes->appendItem("ROM");
  m_transferDataTypes->appendItem("SRAM");
  m_transferDataTypes->setNumVisible(2);

  FXVerticalFrame *transferAreaTopRight = new FXVerticalFrame(transferAreaTop, LAYOUT_FILL_X,
    0, 0, 0, 0, DEFAULT_PAD, 0, DEFAULT_PAD, 0);//, 0, 0);
  m_useEPP = new FXCheckButton(transferAreaTopRight, "Use EPP");
  m_useEPP->setTipText("This *tries* to enable EPP mode");
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
  m_receiveSize = new FXTextField(transferAreaBottom, 4, NULL, 0, FRAME_SUNKEN);
  m_receiveSize->setText("32");
  m_receiveSizeLabel = new FXLabel(transferAreaBottom, "Mbit");
#endif // DISABLE_COPIER

  FXGroupBox *multiGameArea = new FXGroupBox(leftArea, "Multi-game",
    FRAME_GROOVE | LAYOUT_FILL);
  multiGameArea->setFont(boldFont);
  FXHorizontalFrame *loaderArea = new FXHorizontalFrame(multiGameArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_loaderPath = TextFieldOption::add(loaderArea, "Loader:", 1,
    true, this, ID_MULTIGAMEPATH, false, "GBA Settings", "Loader", "");
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
  new FXLabel(createArea, "Mbit");
  m_selectFilesButton = new FXButton(createArea, "Select files", NULL, this,
    ID_MULTIGAME, BUTTON_NORMAL | LAYOUT_CENTER_X);
  m_selectFilesButton->setTipText(
    "Select files or confirm selection for already selected files");
  new FXButton(createArea, "Create file", NULL, this, ID_MULTIGAME,
    BUTTON_NORMAL | LAYOUT_CENTER_X);

  FXGroupBox *crackAreaContainer = new FXGroupBox(leftArea, "Copy protection",
    FRAME_GROOVE | LAYOUT_FILL);
  crackAreaContainer->setFont(boldFont);
  FXHorizontalFrame *crackArea = new FXHorizontalFrame(crackAreaContainer,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *crackButtonsArea = new FXHorizontalFrame(crackArea,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXButton *SRAMFixButton = new FXButton(crackButtonsArea, "SRAM fix", NULL,
    this, ID_SRAMFIX);
  SRAMFixButton->setTipText("Patch ROM for SRAM saving");
  FXButton *CrashFixButton = new FXButton(crackButtonsArea, "Crash fix", NULL,
    this, ID_CRASHFIX);
  CrashFixButton->setTipText("Slow down ROM access");
  new FXLabel(crackArea, "Wait time:");
  m_waitTime = new FXTextField(crackArea, 2, NULL, 0, FRAME_SUNKEN);
  m_waitTime->setText("0");


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS | PACK_UNIFORM_WIDTH,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Fix checksum", NULL, this, ID_FIXCHECKSUM);
  new FXButton(miscButtonArea, "Fix logo", NULL, this, ID_FIXLOGO);

  FXHorizontalFrame *changeNameArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(changeNameArea, "Change name to", NULL, this, ID_NAME);
  m_internalName = new FXTextField(changeNameArea, 12, NULL, 0, FRAME_SUNKEN);

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideGBA = new FXCheckButton(overrideButtonsArea, "File is a Game Boy Advance file");
  m_overrideGBA->setFont(smallFont);

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

  handle(m_useOverride, FXSEL(SEL_COMMAND, ID_OVERRIDE), (void *) 0);
}


GBAFrame::~GBAFrame()
{
  delete m_loaderPath;
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
GBAFrame::writeToRegistry(void)
{
  m_loaderPath->writeToRegistry();
}


long
GBAFrame::onDeviceType(FXObject *, FXSelector, void *)
{
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:                                     // Flash Advance Linker
      m_useEPP->enable();
      break;
    case 1:                                     // Flash 2 Advance (parport)
    case 2:                                     // Flash 2 Advance (USB)
      m_useEPP->disable();
      break;
    }
  return 1;
}


long
GBAFrame::onTransferDataType(FXObject *, FXSelector, void *)
{
  if (m_transferDataTypes->getCurrentItem() == 1)
    {
      m_bankLabel->enable();
      m_bank->enable();
      m_receiveSize->disable();
      m_receiveSizeLabel->disable();
    }
  else
    {
      m_bankLabel->disable();
      m_bank->disable();
      m_receiveSize->enable();
      m_receiveSizeLabel->enable();
    }
  return 1;
}


void
GBAFrame::setTransferOption(bool a_receive, bool a_fileNeeded)
{
  char option[80];
  FXint deviceType = m_deviceTypes->getCurrentItem();
  switch (deviceType)
    {
    case 0:
      strcpy(option, "--xfal");
      break;
    case 1:
    case 2:
      strcpy(option, "--xf2a");
      break;
    }

  char tail[80];
  switch (m_transferDataTypes->getCurrentItem())
    {
    case 0:
      if (a_receive)
        {
          FXString text = m_receiveSize->getText();
          char *receiveSizeStr = (char *) text.text();
          int receiveSize = strtol(receiveSizeStr, NULL, 10);
          if (receiveSizeStr[0] && receiveSize != 32)
            {
              sprintf(tail, "c=%d", receiveSize);
              strcat(option, tail);
            }
        }
      break;
    case 1:
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
    }

  m_commandLineHandler->setOption(option, a_fileNeeded);
  if (deviceType == 0 && m_useEPP->getCheck())
    m_commandLineHandler->addOption("--xfalm");
  else if (deviceType == 2)
    m_commandLineHandler->addOption("--port=USB"); // --port=USB has higher precedence
}                                                  //  than --port=<parport address>


void
GBAFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideGBA->getCheck())
        m_commandLineHandler->addOption("--gba");
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
    }
}


long
GBAFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(false, true);
//  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBAFrame::onReceive(FXObject *, FXSelector, void *)
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

  setTransferOption(true, false);
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
GBAFrame::onMultiGamePath(FXObject *, FXSelector, void *)
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
GBAFrame::onMultiGame(FXObject *a_src, FXSelector, void *)
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
        1, "Select files for multi-game file", fe->m_bigIcon, fe->m_smallIcon);
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
GBAFrame::onSRAMFix(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--sram", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBAFrame::onCrashFix(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_waitTime->getText();
  char *waitTime = (char *) text.text(), option[80];
  if (!waitTime[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No wait time specified",
        "Please specify a wait time");
      return 1;
    }
  if (strlen(waitTime) >= 3)
    waitTime[2] = 0;
  sprintf(option, "--crp=%s", waitTime);
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBAFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBAFrame::onFixChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--chk", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBAFrame::onFixLogo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--logo", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBAFrame::onName(FXObject *, FXSelector, void *)
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
GBAFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((int) a_data)
    {
      m_overrideGBA->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
    }
  else
    {
      m_overrideGBA->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
    }
  return 1;
}


long
GBAFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
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
