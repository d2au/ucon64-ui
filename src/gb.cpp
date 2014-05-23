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
#include <stdint.h>
#include "CommandLineHandler.h"
#include "ComboBox.h"
#include "FileDialog.h"
#include "Frontend.h"
#include "gb.h"


FXDEFMAP(GBFrame) GBFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_DEVICETYPE, GBFrame::onDeviceType),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_TRANSFERDATATYPE, GBFrame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_SEND, GBFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_RECEIVE, GBFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_CONVERT, GBFrame::onConvert),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_SHOWINFO, GBFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_STRIPHEADER, GBFrame::onStripHeader),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_FIXCHECKSUM, GBFrame::onFixChecksum),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_FIXLOGO, GBFrame::onFixLogo),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_NAME, GBFrame::onName),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_CREATEFCEMUROM, GBFrame::onCreateFCEmuROM),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_NESROMPATH, GBFrame::onNESROMPath),
  FXMAPFUNC(SEL_COMMAND, GBFrame::ID_OVERRIDE, GBFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, GBFrame::ID_HEADER1, GBFrame::ID_HEADER3, GBFrame::onHeader)
};

FXIMPLEMENT(GBFrame, FXVerticalFrame, GBFrameMap, ARRAYNUMBER(GBFrameMap))


GBFrame::GBFrame(CommandLineHandler *a_commandLineHandler,
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
  m_deviceTypes->appendItem("Game Boy Xchanger");
  m_deviceTypes->appendItem("Mad Catz Camera Link");
  m_deviceTypes->setNumVisible(2);
  new FXLabel(transferAreaTopLeft, "Data type:");
  m_transferDataTypes = new ComboBox(transferAreaTopLeft, 1, this, ID_TRANSFERDATATYPE,
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
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_sendButton = new FXButton(transferAreaBottom, "Send", NULL, this, ID_SEND);
  FXButton *receiveButton = new FXButton(transferAreaBottom, "Receive", NULL, this, ID_RECEIVE);
#ifdef  __unix__
  receiveButton->setTipText("Use quotes only if the file name should contain them");
#else
  (void) receiveButton;
#endif
#endif // DISABLE_COPIER

  FXGroupBox *conversionAreaContainer = new FXGroupBox(leftArea, "Conversion",
    FRAME_GROOVE | LAYOUT_FILL);
  conversionAreaContainer->setFont(boldFont);
  FXHorizontalFrame *conversionArea = new FXHorizontalFrame(conversionAreaContainer,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(conversionArea, "ROM to");
  m_formats = new ComboBox(conversionArea, 3, NULL, 0, FRAME_SUNKEN | COMBOBOX_STATIC);
  m_formats->appendItem("MGD");
  m_formats->appendItem("SSC");
  m_formats->appendItem("SGB");
  m_formats->appendItem("GBX");
  m_formats->setNumVisible(4);
  new FXButton(conversionArea, "Convert", NULL, this, ID_CONVERT);


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS | PACK_UNIFORM_WIDTH,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Strip header", NULL, this, ID_STRIPHEADER);
  new FXButton(miscButtonArea, "Fix checksum", NULL, this, ID_FIXCHECKSUM);
  new FXButton(miscButtonArea, "Fix logo", NULL, this, ID_FIXLOGO);

  FXHorizontalFrame *changeNameArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(changeNameArea, "Change name to", NULL, this, ID_NAME);
  m_internalName = new FXTextField(changeNameArea, 16, NULL, 0, FRAME_SUNKEN);

  FXHorizontalFrame *createFCEmuROMArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(createFCEmuROMArea, "Create FC emu ROM", NULL, this, ID_CREATEFCEMUROM);
  m_NESROMPath = new FXTextField(createFCEmuROMArea, 14, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  new FXButton(createFCEmuROMArea, "Change", NULL, this, ID_NESROMPATH);

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideGameBoy = new FXCheckButton(overrideButtonsArea, "File is a Game Boy file");
  m_overrideGameBoy->setFont(smallFont);

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


GBFrame::~GBFrame()
{
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
GBFrame::writeToRegistry(void)
{
}


long
GBFrame::onDeviceType(FXObject *, FXSelector, void *)
{
  m_transferDataTypes->clearItems();
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:                                     // Game Boy Xchanger
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->appendItem("SRAM");
      m_transferDataTypes->setNumVisible(2);
      m_useEPP->enable();
      m_sendButton->enable();
      break;
    case 1:                                     // Mad Catz Camera Link
      m_transferDataTypes->appendItem("Image data");
      m_transferDataTypes->setNumVisible(1);
      m_useEPP->disable();
      m_sendButton->disable();
      break;
    }
  return 1;
}


long
GBFrame::onTransferDataType(FXObject *, FXSelector, void *)
{
  if (m_deviceTypes->getCurrentItem() == 0 &&
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


void
GBFrame::setTransferOption(bool a_fileNeeded)
{
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      {
        char option[80], tail[80];
        strcpy(option, "--xgbx");
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
        if (m_useEPP->getCheck())
          m_commandLineHandler->addOption("--xgbxm");
      }
      break;
    case 1:
      m_commandLineHandler->setOption("--xmccl", a_fileNeeded);
      break;
    }
}


void
GBFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideGameBoy->getCheck())
        m_commandLineHandler->addOption("--gb");
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
GBFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(true);
//  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onReceive(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = FileDialog::getSaveFilename(this, "Specify file name to write to",
    "dummy");
  char *filename = (char *) text.text();
  if (!filename[0])
    return 1;
  else if (FXStat::exists(filename))
    {
      FXMessageBox::warning(this, MBOX_OK, "File already exists",
        "Please specify a different file name");
      return 1;
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
GBFrame::onConvert(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *option = NULL;
  switch (m_formats->getCurrentItem())
    {
    case 0:
      option = "--mgd";
      break;
    case 1:
      option = "--ssc";
      break;
    case 2:
      option = "--sgb";
      break;
    case 3:
      option = "--gbx";
      break;
    }
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onStripHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--stp", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onFixChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--chk", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onFixLogo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--logo", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onName(FXObject *, FXSelector, void *)
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
GBFrame::onCreateFCEmuROM(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_NESROMPath->getText();
  char *NESROMPath = (char *) text.text(), option[FILENAME_MAX + 9];
  if (!NESROMPath[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No NES ROM specified",
        "Please specify a NES ROM suitable for KAMI's FC emulator");
      return 1;
    }
#ifdef  __unix__
  sprintf(option, "--n2gb=%s", NESROMPath);
#else
  sprintf(option, "--n2gb=\"%s\"", NESROMPath);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
GBFrame::onNESROMPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_NESROMPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this,
    "Select a NES ROM suitable for KAMI's FC emulator", startPath);
  if (path.text()[0])
    m_NESROMPath->setText(path);
  return 1;
}


long
GBFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((intptr_t) a_data)
    {
      m_overrideGameBoy->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
    }
  else
    {
      m_overrideGameBoy->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
    }
  return 1;
}


long
GBFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
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
