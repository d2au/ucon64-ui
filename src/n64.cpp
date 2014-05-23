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
#include "n64.h"


FXDEFMAP(N64Frame) N64FrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_DEVICETYPE, N64Frame::onDeviceType),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_TRANSFERDATATYPE, N64Frame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_SEND, N64Frame::onSend),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_RECEIVE, N64Frame::onReceive),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_CONVERT, N64Frame::onConvert),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_REPLACEBOOTCODE, N64Frame::onReplaceBootCode),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_REPLACEBOOTCODEPATH, N64Frame::onReplaceBootCodePath),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_EXTRACTBOOTCODE, N64Frame::onExtractBootCode),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_SHOWINFO, N64Frame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_FIXCHECKSUM, N64Frame::onFixChecksum),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_NAME, N64Frame::onName),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_CREATESRAMROM, N64Frame::onCreateSRAMROM),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_SRAMPATH, N64Frame::onSRAMPath),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_CREATESMSEMUROM, N64Frame::onCreateSMSEmuROM),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_SMSROMPATH, N64Frame::onSMSROMPath),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_OVERRIDE, N64Frame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, N64Frame::ID_HEADER1, N64Frame::ID_HEADER3, N64Frame::onHeader),
  FXMAPFUNC(SEL_COMMAND, N64Frame::ID_INTERLEAVE, N64Frame::onInterleave)
};

FXIMPLEMENT(N64Frame, FXVerticalFrame, N64FrameMap, ARRAYNUMBER(N64FrameMap))


N64Frame::N64Frame(CommandLineHandler *a_commandLineHandler,
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
  m_deviceTypes->appendItem("Doctor V64");
  m_deviceTypes->appendItem("Doctor V64 Junior");
  m_deviceTypes->appendItem("CD64");
  m_deviceTypes->appendItem("DexDrive");
  m_deviceTypes->setNumVisible(4);
  new FXLabel(transferAreaTopLeft, "Data type:");
  m_transferDataTypes = new ComboBox(transferAreaTopLeft, 1, this, ID_TRANSFERDATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_transferDataTypes->appendItem("ROM");
  m_transferDataTypes->setNumVisible(1);
  m_CD64ProtocolLabel = new FXLabel(transferAreaTopLeft, "Protocol:");
  m_CD64Protocol = new ComboBox(transferAreaTopLeft, 1, this, ID_CD64PROTOCOL,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_CD64Protocol->appendItem("CD64 BIOS");
  m_CD64Protocol->appendItem("Ghemor");
  m_CD64Protocol->appendItem("UltraLink");
  m_CD64Protocol->setNumVisible(3);

  FXVerticalFrame *transferAreaTopRight = new FXVerticalFrame(transferAreaTop, LAYOUT_FILL_X,
    0, 0, 0, 0, DEFAULT_PAD, 0, DEFAULT_PAD, 0);//, 0, 0);
  new FXLabel(transferAreaTopRight, " ");        // spacer (empty string doesn't work here)
  FXHorizontalFrame *transferAreaTopRight2 = new FXHorizontalFrame(transferAreaTopRight,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_indexLabel = new FXLabel(transferAreaTopRight2, "Index:");
  m_index = new FXTextField(transferAreaTopRight2, 2, NULL, 0, FRAME_SUNKEN);

  FXHorizontalFrame *transferAreaBottom = new FXHorizontalFrame(transferArea,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXHorizontalFrame *transferButtonArea = new FXHorizontalFrame(transferAreaBottom,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(transferButtonArea, "Send", NULL, this, ID_SEND);
  m_receiveButton = new FXButton(transferButtonArea, "Receive", NULL, this, ID_RECEIVE);
#ifdef  __unix__
  m_receiveButton->setTipText("Use quotes only if the file name should contain them");
#endif
  m_receiveSize = new FXTextField(transferAreaBottom, 3, NULL, 0, FRAME_SUNKEN);
  m_receiveSize->setText("64");
  m_receiveSizeLabel = new FXLabel(transferAreaBottom, "Mbit");
#endif // DISABLE_COPIER

  FXGroupBox *conversionAreaContainer = new FXGroupBox(leftArea, "Conversion",
    FRAME_GROOVE | LAYOUT_FILL);
  conversionAreaContainer->setFont(boldFont);
  FXHorizontalFrame *conversionArea = new FXHorizontalFrame(conversionAreaContainer,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(conversionArea, "ROM to");
  m_formats = new ComboBox(conversionArea, 3, NULL, 0, FRAME_SUNKEN | COMBOBOX_STATIC);
  m_formats->appendItem("V64");
  m_formats->appendItem("Z64");
  m_formats->appendItem("byte-swapped");
  m_formats->appendItem("word-swapped");
  m_formats->setNumVisible(4);
  new FXButton(conversionArea, "Convert", NULL, this, ID_CONVERT);

  FXGroupBox *bootCodeArea = new FXGroupBox(leftArea, "Boot code",
    FRAME_GROOVE | LAYOUT_FILL);
  bootCodeArea->setFont(boldFont);
  FXHorizontalFrame *replaceBootCodeArea = new FXHorizontalFrame(bootCodeArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(replaceBootCodeArea, "Replace boot code", NULL, this, ID_REPLACEBOOTCODE);
  m_replaceBootCodePath = new FXTextField(replaceBootCodeArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  new FXButton(replaceBootCodeArea, "Change", NULL, this, ID_REPLACEBOOTCODEPATH);
  FXHorizontalFrame *extractBootCodeArea = new FXHorizontalFrame(bootCodeArea,
    LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(extractBootCodeArea, "Extract boot code", NULL, this, ID_EXTRACTBOOTCODE);
  m_extractBootCodeName = new FXTextField(extractBootCodeArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  m_extractBootCodeName->setTipText("Specify a file name without a path");


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS | PACK_UNIFORM_WIDTH,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Fix checksum", NULL, this, ID_FIXCHECKSUM);

  FXHorizontalFrame *changeNameArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(changeNameArea, "Change name to", NULL, this, ID_NAME);
  m_internalName = new FXTextField(changeNameArea, 20, NULL, 0, FRAME_SUNKEN);

  FXHorizontalFrame *createSRAMROMArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(createSRAMROMArea, "Create SRAM upload ROM", NULL, this, ID_CREATESRAMROM);
  m_SRAMPath = new FXTextField(createSRAMROMArea, 14, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X);
  new FXButton(createSRAMROMArea, "Change", NULL, this, ID_SRAMPATH);

  FXHorizontalFrame *createSMSEmuROMArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(createSMSEmuROMArea, "Create SMS emu ROM", NULL, this, ID_CREATESMSEMUROM);
  m_SMSROMPath = new FXTextField(createSMSEmuROMArea, 1, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X);
  new FXButton(createSMSEmuROMArea, "Change", NULL, this, ID_SMSROMPATH);

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideN64 = new FXCheckButton(overrideButtonsArea, "File is a Nintendo 64 file");
  m_overrideN64->setFont(smallFont);

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


N64Frame::~N64Frame()
{
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
N64Frame::writeToRegistry(void)
{
}


long
N64Frame::onDeviceType(FXObject *, FXSelector, void *)
{
  m_transferDataTypes->clearItems();
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:                                     // Doctor V64
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->setNumVisible(1);
      m_indexLabel->disable();
      m_index->disable();
      m_receiveButton->enable();
      m_CD64ProtocolLabel->disable();
      m_CD64Protocol->disable();
      m_receiveSize->disable();
      m_receiveSizeLabel->disable();
      break;
    case 1:                                     // Doctor V64 Junior
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->setNumVisible(1);
      m_indexLabel->disable();
      m_index->disable();
      m_receiveButton->disable();
      m_CD64ProtocolLabel->disable();
      m_CD64Protocol->disable();
      m_receiveSize->disable();
      m_receiveSizeLabel->disable();
      break;
    case 2:                                     // CD64
      m_transferDataTypes->appendItem("ROM");
      m_transferDataTypes->appendItem("Boot emu");
      m_transferDataTypes->appendItem("SRAM");
      m_transferDataTypes->appendItem("Flash RAM");
      m_transferDataTypes->appendItem("EEPROM");
      m_transferDataTypes->appendItem("Memory pack");
      m_transferDataTypes->setNumVisible(6);
      m_indexLabel->disable();                  // FOX will show the 1st element which
      m_index->disable();                       //  is ROM => disable index widgets
      m_receiveButton->enable();
      m_CD64ProtocolLabel->enable();
      m_CD64Protocol->enable();
      m_receiveSize->enable();
      m_receiveSizeLabel->enable();
      break;
    case 3:                                     // DexDrive
      m_transferDataTypes->appendItem("SRAM");
      m_transferDataTypes->setNumVisible(1);
      m_indexLabel->enable();
      m_index->enable();
      m_receiveButton->enable();
      m_CD64ProtocolLabel->disable();
      m_CD64Protocol->disable();
      m_receiveSize->disable();
      m_receiveSizeLabel->disable();
      break;
    }
  return 1;
}


long
N64Frame::onTransferDataType(FXObject *, FXSelector, void *)
{
  FXint device = m_deviceTypes->getCurrentItem(),
        dataType = m_transferDataTypes->getCurrentItem();
  if ((device == 2 && dataType == 5) || device == 3)
    {                                           // CD64 memory pack/DexDrive
      m_indexLabel->enable();
      m_index->enable();
    }
  else
    {
      m_indexLabel->disable();
      m_index->disable();
    }

  if (device == 2)
    {
      if (dataType == 1)                        // CD64 boot emu
        m_receiveButton->disable();
      else
        m_receiveButton->enable();

      if (dataType == 0)
        {
          m_receiveSize->enable();
          m_receiveSizeLabel->enable();
        }
      else
        {
          m_receiveSize->disable();
          m_receiveSizeLabel->disable();
        }
    }

  return 1;
}


bool
N64Frame::setTransferOptions(bool a_receive, bool a_fileNeeded)
{
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      m_commandLineHandler->setOption("--xv64", a_fileNeeded);
      break;
    case 1:
      m_commandLineHandler->setOption("--xdjr", a_fileNeeded);
      break;
    case 2:
      {
        char option[80], tail[80];
        strcpy(option, "--xcd64");
        switch (m_transferDataTypes->getCurrentItem())
          {
          case 0:
            if (a_receive)
              {
                FXString text = m_receiveSize->getText();
                char *receiveSizeStr = (char *) text.text();
                int receiveSize = strtol(receiveSizeStr, NULL, 10);
                if (receiveSizeStr[0] && receiveSize != 64)
                  {
                    sprintf(tail, "c=%d", receiveSize);
                    strcat(option, tail);
                  }
              }
            break;
          case 1:
            strcat(option, "b");
            break;
          case 2:
            strcat(option, "s");
            break;
          case 3:
            strcat(option, "f");
            break;
          case 4:
            strcat(option, "e");
            break;
          case 5:
            {
              FXString text = m_index->getText();
              char *index = (char *) text.text();
              if (!index[0])
                {
                  FXMessageBox::warning(this, MBOX_OK, "No index specified",
                    "Please specify an index");
                  return false;
                }
              sprintf(tail, "m=%s", index);
              strcat(option, tail);
            }
            break;
          }
        m_commandLineHandler->setOption(option, a_fileNeeded);
        sprintf(option, "--xcd64p=%d", m_CD64Protocol->getCurrentItem());
        m_commandLineHandler->addOption(option);
      }
      break;
    case 3:
      {
        FXString text = m_index->getText();
        char *index = (char *) text.text();
        if (!index[0])
          {
            FXMessageBox::warning(this, MBOX_OK, "No index specified",
              "Please specify an index");
            return false;
          }
        char option[80];
        sprintf(option, "--xdex=%s", index);
        m_commandLineHandler->setOption(option, a_fileNeeded);
      }
      break;
    }
  return true;
}


void
N64Frame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideN64->getCheck())
        m_commandLineHandler->addOption("--n64");
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
N64Frame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  if (!setTransferOptions(false, true))
    return 1;
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onReceive(FXObject *, FXSelector, void *)
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

  if (!setTransferOptions(true, false))
    return 1;
  setOverrideOptions();
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
N64Frame::onConvert(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *option = NULL;
  switch (m_formats->getCurrentItem())
    {
    case 0:
      option = "--v64";
      break;
    case 1:
      option = "--z64";
      break;
    case 2:
      option = "--swap";
      break;
    case 3:
      option = "--swap2";
      break;
    }
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onReplaceBootCode(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_replaceBootCodePath->getText();
  char *filename = (char *) text.text();
  if (!filename[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No boot code file name specified",
        "Please specify a boot code file name");
      return 1;
    }
  else if (!FXStat::exists(filename))
    {
      FXMessageBox::warning(this, MBOX_OK,
        "Specified boot code file does not exist",
        "Please specify a different boot code file name");
      return 1;
    }

  char option[FILENAME_MAX + 8];
#ifdef  __unix__
  sprintf (option, "--bot=%s", filename);
#else
  sprintf (option, "--bot=\"%s\"", filename);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onReplaceBootCodePath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_replaceBootCodePath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select boot code file",
    startPath);
  if (path.text()[0])
    m_replaceBootCodePath->setText(path);
  return 1;
}


long
N64Frame::onExtractBootCode(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_extractBootCodeName->getText();
  char *filename = (char *) text.text();
  if (!filename[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No boot code file name specified",
        "Please specify a boot code file name");
      return 1;
    }
  // also check for the existence of the file, because the action of --bot
  //  is determinded by it
  else if (FXStat::exists(filename))
    {
      FXMessageBox::warning(this, MBOX_OK, "File already exists",
        "Please specify a different file name");
      return 1;
    }

  char option[FILENAME_MAX + 8];
#ifdef  __unix__
  sprintf (option, "--bot=%s", filename);
#else
  sprintf (option, "--bot=\"%s\"", filename);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onFixChecksum(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--chk", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onName(FXObject *, FXSelector, void *)
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
N64Frame::onCreateSRAMROM(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_SRAMPath->getText();
  char *SRAMPath = (char *) text.text(), option[FILENAME_MAX + 10];
  if (!SRAMPath[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No SRAM file specified",
        "Please specify an SRAM file suitable for LaC's SRAM upload tool");
      return 1;
    }
#ifdef  __unix__
  sprintf(option, "--lsram=%s", SRAMPath);
#else
  sprintf(option, "--lsram=\"%s\"", SRAMPath);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onSRAMPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_SRAMPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this,
    "Select an SRAM file suitable for LaC's SRAM upload tool", startPath);
  if (path.text()[0])
    m_SRAMPath->setText(path);
  return 1;
}


long
N64Frame::onCreateSMSEmuROM(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  FXString text = m_SMSROMPath->getText();
  char *SMSROMPath = (char *) text.text(), option[FILENAME_MAX + 9];
  if (!SMSROMPath[0])
    {
      FXMessageBox::warning(this, MBOX_OK, "No SMS ROM specified",
        "Please specify an SMS ROM suitable for Jos Kwanten's SMS emulator");
      return 1;
    }
#ifdef  __unix__
  sprintf(option, "--usms=%s", SMSROMPath);
#else
  sprintf(option, "--usms=\"%s\"", SMSROMPath);
#endif
  m_commandLineHandler->setOption(option, true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
N64Frame::onSMSROMPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_SMSROMPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this,
    "Select an SMS ROM suitable for Jos Kwanten's SMS emulator", startPath);
  if (path.text()[0])
    m_SMSROMPath->setText(path);
  return 1;
}


long
N64Frame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((intptr_t) a_data)
    {
      m_overrideN64->enable();
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
      m_overrideN64->disable();
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
N64Frame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
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
N64Frame::onInterleave(FXObject *a_src, FXSelector, void *)
{
  m_overrideInterleaved->setCheck(a_src == m_overrideInterleaved);
  m_overrideNotInterleaved->setCheck(a_src == m_overrideNotInterleaved);
  return 1;
}
