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
#include "nes.h"


FXDEFMAP(NESFrame) NESFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_TRANSFERDATATYPE, NESFrame::onTransferDataType),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_SEND, NESFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_RECEIVE, NESFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_FORMAT, NESFrame::onFormat),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_CONVERT, NESFrame::onConvert),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_BATTERY, NESFrame::onBattery),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_MIRRORING, NESFrame::onMirroring),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_CONTROLLER, NESFrame::onController),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_TV, NESFrame::onTV),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_VRAM, NESFrame::onVRAM),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_DUMPINFOPATH, NESFrame::onDumperInfoPath),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_SHOWINFO, NESFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_SHOWFDSLIST, NESFrame::onShowFDSList),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_STRIPHEADER, NESFrame::onStripHeader),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_INESHD, NESFrame::onExtractiNESHeader),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_NAME, NESFrame::onName),
  FXMAPFUNC(SEL_COMMAND, NESFrame::ID_OVERRIDE, NESFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, NESFrame::ID_HEADER1, NESFrame::ID_HEADER3, NESFrame::onHeader)
};

FXIMPLEMENT(NESFrame, FXVerticalFrame, NESFrameMap, ARRAYNUMBER(NESFrameMap))


NESFrame::NESFrame(CommandLineHandler *a_commandLineHandler,
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

  FXMatrix *transferAreaTop = new FXMatrix(transferArea, 2,
    MATRIX_BY_COLUMNS /*| LAYOUT_CENTER_X*/, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(transferAreaTop, "Device:");
  m_deviceTypes = new ComboBox(transferAreaTop, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_deviceTypes->appendItem("Super Magic Card");
  m_deviceTypes->setNumVisible(1);
  new FXLabel(transferAreaTop, "Data type:");
  m_transferDataTypes = new ComboBox(transferAreaTop, 1, this, ID_TRANSFERDATATYPE,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_transferDataTypes->appendItem("ROM");
  m_transferDataTypes->appendItem("Real-time save");
  m_transferDataTypes->setNumVisible(2);

  FXHorizontalFrame *transferAreaBottom = new FXHorizontalFrame(transferArea,
    LAYOUT_CENTER_X | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(transferAreaBottom, "Send", NULL, this, ID_SEND);
  m_receiveButton = new FXButton(transferAreaBottom, "Receive", NULL, this, ID_RECEIVE);
#ifdef  __unix__
  m_receiveButton->setTipText("Use quotes only if the file name should contain them");
#endif
#endif // DISABLE_COPIER

  FXGroupBox *conversionArea = new FXGroupBox(leftArea, "Conversion",
    FRAME_GROOVE | LAYOUT_FILL);
  conversionArea->setFont(boldFont);
  FXHorizontalFrame *conversionAreaTop = new FXHorizontalFrame(conversionArea,
    LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXLabel(conversionAreaTop, "ROM to");
  m_formats = new ComboBox(conversionAreaTop, 1, this, ID_FORMAT, FRAME_SUNKEN | COMBOBOX_STATIC);
  m_formats->appendItem("UNIF");
  m_formats->appendItem("iNES");
  m_formats->appendItem("Pasofami");
  m_formats->appendItem("FFE (SMC)");
  m_formats->appendItem("FDS");
  m_formats->setNumVisible(5);
  new FXButton(conversionAreaTop, "Convert", NULL, this, ID_CONVERT);

  // board name
  FXHorizontalFrame *mapperArea = new FXHorizontalFrame(conversionArea, 0,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_mapperLabel = new FXLabel(mapperArea, "Board name/mapper:");
  m_mapper = new FXTextField(mapperArea, 20, NULL, 0, FRAME_SUNKEN);

  FXMatrix *conversionAreaMiddle = new FXMatrix(conversionArea, 2,
     MATRIX_BY_COLUMNS | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);

  // battery
  m_batteryLabel = new FXLabel(conversionAreaMiddle, "Battery:");
  FXHorizontalFrame *batteryArea = new FXHorizontalFrame(conversionAreaMiddle,
    LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_battery[0] = new FXCheckButton(batteryArea, "?", this, ID_BATTERY);
  m_battery[0]->setTipText("Presence of battery is unspecified");
  ((FXCheckButton *) m_battery[0])->setCheck();
  m_battery[1] = new FXRadioButton(batteryArea, "present", this, ID_BATTERY);
  m_battery[2] = new FXRadioButton(batteryArea, "not present", this, ID_BATTERY);
  ((FXRadioButton *) m_battery[1])->setCheck();

  // mirroring type
  m_mirroringLabel = new FXLabel(conversionAreaMiddle, "Mirroring type:");
  FXHorizontalFrame *mirroringArea = new FXHorizontalFrame(conversionAreaMiddle,
    LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  int n;
  char label[80], *mirroringTipText[7] =
    {
      "Mirroring type is unspecified",
      "MTYPE=0 horizontal mirroring",
      "MTYPE=1 vertical mirroring",
      "MTYPE=2 mirror all pages from $2000 (UNIF only)",
      "MTYPE=3 mirror all pages from $2400 (UNIF only)",
      "MTYPE=4 four screens of VRAM",
      "MTYPE=5 mirroring controlled by mapper hardware (UNIF only)"
    };
  m_mirroring[0] = new FXCheckButton(mirroringArea, "?", this, ID_MIRRORING);
  m_mirroring[0]->setTipText(mirroringTipText[0]);
  ((FXCheckButton *) m_mirroring[0])->setCheck();
  for (n = 1; n < 7; n++)
    {
      sprintf(label, "%d", n - 1);
      m_mirroring[n] = new FXRadioButton(mirroringArea, label, this, ID_MIRRORING);
      m_mirroring[n]->setTipText(mirroringTipText[n]);
    }
  ((FXRadioButton *) m_mirroring[1])->setCheck();

  // television standard
  m_televisionTypeLabel = new FXLabel(conversionAreaMiddle, "TV standard:");
  FXHorizontalFrame *televisionArea = new FXHorizontalFrame(conversionAreaMiddle,
    LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_televisionType[0] = new FXCheckButton(televisionArea, "?", this, ID_TV);
  m_televisionType[0]->setTipText("Television standard is unspecified");
  m_televisionType[0]->setCheck();
  m_televisionType[1] = new FXCheckButton(televisionArea, "NTSC", this, ID_TV);
  m_televisionType[2] = new FXCheckButton(televisionArea, "PAL", this, ID_TV);

  // controller type
  m_controllerLabel = new FXLabel(conversionAreaMiddle, "Controller type:");
  FXHorizontalFrame *controllerArea = new FXHorizontalFrame(conversionAreaMiddle,
    LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  char *controllerTipText[7] =
    {
      "Controller type is unspecified",
      "TYPE=0 regular joypad",
      "TYPE=1 zapper",
      "TYPE=2 R.O.B.",
      "TYPE=3 Arkanoid controller",
      "TYPE=4 powerpad",
      "TYPE=5 four-score adapter"
    };
  m_controller[0] = new FXCheckButton(controllerArea, "?", this, ID_CONTROLLER);
  m_controller[0]->setTipText(controllerTipText[0]);
  m_controller[0]->setCheck();
  for (n = 1; n < 7; n++)
    {
      sprintf(label, "%d", n - 1);
      m_controller[n] = new FXCheckButton(controllerArea, label);
      m_controller[n]->setTipText(controllerTipText[n]);
    }

  // VRAM override
  m_VRAMOverrideLabel = new FXLabel(conversionAreaMiddle, "VRAM override:");
  FXHorizontalFrame *VRAMOverrideArea = new FXHorizontalFrame(conversionAreaMiddle,
    LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_VRAMOverride[0] = new FXCheckButton(VRAMOverrideArea, "?", this, ID_VRAM);
  m_VRAMOverride[0]->setTipText("VRAM override is unspecified");
  ((FXCheckButton *) m_VRAMOverride[0])->setCheck();
  m_VRAMOverride[1] = new FXRadioButton(VRAMOverrideArea, "yes", this, ID_VRAM);
  m_VRAMOverride[2] = new FXRadioButton(VRAMOverrideArea, "no", this, ID_VRAM);
  ((FXRadioButton *) m_VRAMOverride[1])->setCheck();

  // comment
  m_commentLabel = new FXLabel(conversionAreaMiddle, "Comment:");
  m_comment = new FXTextField(conversionAreaMiddle, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN);

  // dumper info
  m_dumperInfoLabel = new FXLabel(conversionAreaMiddle, "Dumper info:");
  FXHorizontalFrame *m_dumperInfoArea = new FXHorizontalFrame(conversionAreaMiddle,
    LAYOUT_FILL_X | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  m_dumperInfoPath = new FXTextField(m_dumperInfoArea, 1, NULL, 0,
    FRAME_SUNKEN | LAYOUT_FILL_X);
  m_dumperInfoPathButton = new FXButton(m_dumperInfoArea, "Change", NULL, this, ID_DUMPINFOPATH);


  FXVerticalFrame *rightArea = new FXVerticalFrame(windowArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXGroupBox *miscArea = new FXGroupBox(rightArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  FXMatrix *miscButtonArea = new FXMatrix(miscArea, 3, MATRIX_BY_COLUMNS | PACK_UNIFORM_WIDTH,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  new FXButton(miscButtonArea, "Show info", NULL, this, ID_SHOWINFO);
  new FXButton(miscButtonArea, "Show FDS list", NULL, this, ID_SHOWFDSLIST);
  FXButton *stripHeaderButton =
    new FXButton(miscButtonArea, "Strip header", NULL, this, ID_STRIPHEADER);
  stripHeaderButton->setTipText("Strips 512 bytes, not the detected header size");
  new FXButton(miscArea, "Extract iNES header", NULL, this, ID_INESHD);

  FXHorizontalFrame *changeNameArea = new FXHorizontalFrame(miscArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXButton *changeNameButton = new FXButton(changeNameArea, "Change name to", NULL, this, ID_NAME);
  changeNameButton->setTipText("Works only for UNIF files");
  m_internalName = new FXTextField(changeNameArea, 1, NULL, 0, FRAME_SUNKEN | LAYOUT_FILL_X);

  FXGroupBox *overrideArea = new FXGroupBox(rightArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideNES = new FXCheckButton(overrideButtonsArea, "File is a NES/Famicom file");
  m_overrideNES->setFont(smallFont);

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
  m_overrideHeaderSizeText->setText("16");      // iNES header is 16 bytes
  m_overrideHeader = new FXRadioButton(headerSizeArea, "File has header", this, ID_HEADER2);
  m_overrideHeader->setFont(smallFont);
  m_overrideNoHeader = new FXRadioButton(headerSizeArea, "File has no header", this, ID_HEADER3);
  m_overrideNoHeader->setFont(smallFont);
  m_headerChoice = 0;

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FIX_WIDTH | SEPARATOR_GROOVE,
    0, 0, 50);
  m_overrideNotSplit = new FXCheckButton(overrideButtonsArea, "File is not split");
  m_overrideNotSplit->setFont(smallFont);

  handle(m_useOverride, FXSEL(SEL_COMMAND, ID_OVERRIDE), (void *) 0);
}


NESFrame::~NESFrame()
{
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
  delete m_transferDataTypes;
#endif
}


void
NESFrame::writeToRegistry(void)
{
}


long
NESFrame::onTransferDataType(FXObject *, FXSelector, void *)
{
  if (m_deviceTypes->getCurrentItem() == 0 &&
      m_transferDataTypes->getCurrentItem() == 1)
    m_receiveButton->enable();
  else
    m_receiveButton->disable();

  return 1;
}


void
NESFrame::setTransferOption(bool a_fileNeeded)
{
  char option[80];
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      strcpy(option, "--xsmc");
      if (m_transferDataTypes->getCurrentItem() == 1)
        strcat(option, "r");
      m_commandLineHandler->setOption(option, a_fileNeeded);
      break;
    }
}


void
NESFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideNES->getCheck())
        m_commandLineHandler->addOption("--nes");
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
    }
}


long
NESFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NESFrame::onReceive(FXObject *, FXSelector, void *)
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


bool
NESFrame::setConversionOptions(void)
{
  int format = m_formats->getCurrentItem();
  char option[(FILENAME_MAX > 800 ? FILENAME_MAX : 800) + 80];

  if (format <= 2)                              // UNIF, iNES, Pasofami
    {
      FXString text = m_mapper->getText();
      char *mapper = (char *) text.text();
      if (mapper[0])
        {
          if (strlen(mapper) >= 80)
            mapper[80 - 1] = 0;
#ifdef  __unix__
          sprintf(option, "--mapr=%s", mapper);
#else
          sprintf(option, "--mapr=\"%s\"", mapper);
          // just in case a new board name contains whitespace
#endif
          m_commandLineHandler->addOption(option);
        }

      if (!((FXCheckButton *) m_battery[0])->getCheck())
        {
          if (((FXRadioButton *) m_battery[1])->getCheck())
            m_commandLineHandler->addOption("--bat");
          else
            m_commandLineHandler->addOption("--nbat");
        }

      if (!((FXCheckButton *) m_mirroring[0])->getCheck())
        /*
          I chose to do it this way instead of letting each FXRadioButton send
          its unique ID, remember that ID and act on it here. Reason: less
          code :-)
        */
        for (int n = 1; n < 7; n++)
          if (((FXRadioButton *) m_mirroring[n])->getCheck())
            {
              sprintf(option, "--mirr=%d", n - 1);
              m_commandLineHandler->addOption(option);
              break;
            }
    }

  if (format <= 1)                              // UNIF, iNES
    {
      if (!m_televisionType[0]->getCheck())
        {
          if (m_televisionType[1]->getCheck())
            m_commandLineHandler->addOption("--ntsc");
          if (m_televisionType[2]->getCheck())
            m_commandLineHandler->addOption("--pal");
        }
    }

  if (format == 0)                              // UNIF
    {
      if (!m_controller[0]->getCheck())
        for (int n = 1; n < 7; n++)
          if (m_controller[n]->getCheck())
            {
              sprintf(option, "--ctrl=%d", n - 1);
              m_commandLineHandler->addOption(option);
            }

      if (!((FXCheckButton *) m_VRAMOverride[0])->getCheck())
        {
          if (((FXRadioButton *) m_VRAMOverride[1])->getCheck())
            m_commandLineHandler->addOption("--vram");
          else
            m_commandLineHandler->addOption("--nvram");
        }

      FXString text = m_comment->getText();
      char *ptr = (char *) text.text();
      if (ptr[0])                               // specifying a comment is optional
        {
          if (strlen(ptr) >= 800)
            ptr[800 - 1] = 0;
#ifdef  __unix__
          sprintf(option, "--cmnt=%s", ptr);
#else
          sprintf(option, "--cmnt=\"%s\"", ptr);
#endif
          m_commandLineHandler->addOption(option);
        }

      text = m_dumperInfoPath->getText();
      ptr = (char *) text.text();
      if (ptr[0])                               // specifying a dumper info file
        {                                       //  is optional
          if (!FXStat::exists(ptr))
            {
              FXMessageBox::warning(this, MBOX_OK,
                "Specified dumper info file does not exist",
                "Please specify a different dumper info file name or none at all");
              return false;
            }
#ifdef  __unix__
          sprintf(option, "--dumpinfo=%s", ptr);
#else
          sprintf(option, "--dumpinfo=\"%s\"", ptr);
#endif
          m_commandLineHandler->addOption(option);
        }
    }
  return true;
}


long
NESFrame::onFormat(FXObject *, FXSelector, void *)
{
  switch (m_formats->getCurrentItem())
    {
    case 0:                                     // UNIF
      m_mapperLabel->enable();
      m_mapper->enable();

      m_batteryLabel->enable();
      m_battery[0]->enable();
      handle(m_battery[0], FXSEL(SEL_COMMAND, ID_BATTERY),
        (void *) (int) ((FXCheckButton *) m_battery[0])->getCheck());

      m_mirroringLabel->enable();
      m_mirroring[0]->enable();
      handle(m_mirroring[0], FXSEL(SEL_COMMAND, ID_MIRRORING),
        (void *) (int) ((FXCheckButton *) m_mirroring[0])->getCheck());

      m_televisionTypeLabel->enable();
      m_televisionType[0]->enable();
      handle(m_televisionType[0], FXSEL(SEL_COMMAND, ID_TV),
        (void *) (int) m_televisionType[0]->getCheck());

      m_controllerLabel->enable();
      m_controller[0]->enable();
      handle(m_controller[0], FXSEL(SEL_COMMAND, ID_CONTROLLER),
        (void *) (int) m_controller[0]->getCheck());

      m_VRAMOverrideLabel->enable();
      m_VRAMOverride[0]->enable();
      handle(m_VRAMOverride[0], FXSEL(SEL_COMMAND, ID_VRAM),
        (void *) (int) ((FXCheckButton *) m_VRAMOverride[0])->getCheck());
        
      m_commentLabel->enable();
      m_comment->enable();

      m_dumperInfoLabel->enable();
      m_dumperInfoPath->enable();
      m_dumperInfoPathButton->enable();
      break;
    case 1:                                     // iNES
      m_mapperLabel->enable();
      m_mapper->enable();

      m_batteryLabel->enable();
      m_battery[0]->enable();
      handle(m_battery[0], FXSEL(SEL_COMMAND, ID_BATTERY),
        (void *) (int) ((FXCheckButton *) m_battery[0])->getCheck());

      m_mirroringLabel->enable();
      m_mirroring[0]->enable();
      handle(m_mirroring[0], FXSEL(SEL_COMMAND, ID_MIRRORING),
        (void *) (int) ((FXCheckButton *) m_mirroring[0])->getCheck());

      m_televisionTypeLabel->enable();
      m_televisionType[0]->enable();
      handle(m_televisionType[0], FXSEL(SEL_COMMAND, ID_TV),
        (void *) (int) m_televisionType[0]->getCheck());

      m_controllerLabel->disable();
      m_controller[0]->disable();
      handle(m_controller[0], FXSEL(SEL_COMMAND, ID_CONTROLLER), (void *) 1);

      m_VRAMOverrideLabel->disable();
      m_VRAMOverride[0]->disable();
      handle(m_VRAMOverride[0], FXSEL(SEL_COMMAND, ID_VRAM), (void *) 1);

      m_commentLabel->disable();
      m_comment->disable();

      m_dumperInfoLabel->disable();
      m_dumperInfoPath->disable();
      m_dumperInfoPathButton->disable();
      break;
    case 2:                                     // Pasofami
      m_mapperLabel->enable();
      m_mapper->enable();

      m_batteryLabel->enable();
      m_battery[0]->enable();
      handle(m_battery[0], FXSEL(SEL_COMMAND, ID_BATTERY),
        (void *) (int) ((FXCheckButton *) m_battery[0])->getCheck());

      m_mirroringLabel->enable();
      m_mirroring[0]->enable();
      handle(m_mirroring[0], FXSEL(SEL_COMMAND, ID_MIRRORING),
        (void *) (int) ((FXCheckButton *) m_mirroring[0])->getCheck());

      m_televisionTypeLabel->disable();
      m_televisionType[0]->disable();
      handle(m_televisionType[0], FXSEL(SEL_COMMAND, ID_TV), (void *) 1);

      m_controllerLabel->disable();
      m_controller[0]->disable();
      handle(m_controller[0], FXSEL(SEL_COMMAND, ID_CONTROLLER), (void *) 1);

      m_VRAMOverrideLabel->disable();
      m_VRAMOverride[0]->disable();
      handle(m_VRAMOverride[0], FXSEL(SEL_COMMAND, ID_VRAM), (void *) 1);

      m_commentLabel->disable();
      m_comment->disable();

      m_dumperInfoLabel->disable();
      m_dumperInfoPath->disable();
      m_dumperInfoPathButton->disable();
      break;
    case 3:                                     // FFE (SMC)
    case 4:                                     // FDS
      m_mapperLabel->disable();
      m_mapper->disable();

      m_batteryLabel->disable();
      m_battery[0]->disable();
      handle(m_battery[0], FXSEL(SEL_COMMAND, ID_BATTERY), (void *) 1);

      m_mirroringLabel->disable();
      m_mirroring[0]->disable();
      handle(m_mirroring[0], FXSEL(SEL_COMMAND, ID_MIRRORING), (void *) 1);

      m_televisionTypeLabel->disable();
      m_televisionType[0]->disable();
      handle(m_televisionType[0], FXSEL(SEL_COMMAND, ID_TV), (void *) 1);

      m_controllerLabel->disable();
      m_controller[0]->disable();
      handle(m_controller[0], FXSEL(SEL_COMMAND, ID_CONTROLLER), (void *) 1);

      m_VRAMOverrideLabel->disable();
      m_VRAMOverride[0]->disable();
      handle(m_VRAMOverride[0], FXSEL(SEL_COMMAND, ID_VRAM), (void *) 1);

      m_commentLabel->disable();
      m_comment->disable();

      m_dumperInfoLabel->disable();
      m_dumperInfoPath->disable();
      m_dumperInfoPathButton->disable();
      break;
    }
  return 1;
}


long
NESFrame::onConvert(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  char *option = NULL;
  switch (m_formats->getCurrentItem())
    {
    case 0:
      option = "--unif";
      break;
    case 1:
      option = "--ines";
      break;
    case 2:
      option = "--pasofami";
      break;
    case 3:
      option = "--ffe";
      break;
    case 4:
      option = "--fds";
      break;
    }
  m_commandLineHandler->setOption(option, true);
  if (!setConversionOptions())
    return 1;
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NESFrame::onBattery(FXObject *a_src, FXSelector, void *a_data)
{
  int n;

  if (a_src == m_battery[0])
    {
      if ((intptr_t) a_data)
        for (n = 1; n < 3; n++)
          m_battery[n]->disable();
      else
        for (n = 1; n < 3; n++)
          m_battery[n]->enable();
    }
  else
    {
      ((FXRadioButton *) m_battery[1])->setCheck(a_src == m_battery[1]);
      ((FXRadioButton *) m_battery[2])->setCheck(a_src == m_battery[2]);
    }

  return 1;
}


long
NESFrame::onMirroring(FXObject *a_src, FXSelector, void *a_data)
{
  int n;

  if (a_src == m_mirroring[0])
    {
      if ((intptr_t) a_data)
        for (n = 1; n < 7; n++)
          m_mirroring[n]->disable();
      else
        for (n = 1; n < 7; n++)
          m_mirroring[n]->enable();
    }
  else
    {
      for (n = 1; n < 7; n++)
        ((FXRadioButton *) m_mirroring[n])->setCheck(FALSE);
      ((FXRadioButton *) a_src)->setCheck();
    }

  return 1;
}


long
NESFrame::onController(FXObject *, FXSelector, void *a_data)
{
  int n;

  if ((intptr_t) a_data)
    for (n = 1; n < 7; n++)
      m_controller[n]->disable();
  else
    for (n = 1; n < 7; n++)
      m_controller[n]->enable();

  return 1;
}


long
NESFrame::onTV(FXObject *a_src, FXSelector, void *a_data)
{
  int n;

  if (a_src == m_televisionType[0])
    {
      if ((intptr_t) a_data)
        for (n = 1; n < 3; n++)
          m_televisionType[n]->disable();
      else
        for (n = 1; n < 3; n++)
          m_televisionType[n]->enable();

      // Unlike UNIF, iNES does not support NTSC + PAL
      if (m_formats->getCurrentItem() == 1)     // iNES
        if (m_televisionType[1]->getCheck() && m_televisionType[2]->getCheck())
          m_televisionType[2]->setCheck(FALSE);
    }
  else if (a_src == m_televisionType[1])        // NTSC
    {
      if (m_formats->getCurrentItem() == 1)     // iNES
        m_televisionType[2]->setCheck(FALSE);
    }
  else if (a_src == m_televisionType[2])        // PAL
    {
      if (m_formats->getCurrentItem() == 1)     // iNES
        m_televisionType[1]->setCheck(FALSE);
    }

  return 1;
}


long
NESFrame::onVRAM(FXObject *a_src, FXSelector, void *a_data)
{
  int n;

  if (a_src == m_VRAMOverride[0])
    {
      if ((intptr_t) a_data)
        for (n = 1; n < 3; n++)
          m_VRAMOverride[n]->disable();
      else
        for (n = 1; n < 3; n++)
          m_VRAMOverride[n]->enable();
    }
  else
    {
      ((FXRadioButton *) m_VRAMOverride[1])->setCheck(a_src == m_VRAMOverride[1]);
      ((FXRadioButton *) m_VRAMOverride[2])->setCheck(a_src == m_VRAMOverride[2]);
    }

  return 1;
}


long
NESFrame::onDumperInfoPath(FXObject *, FXSelector, void *)
{
  FXString startPath = m_dumperInfoPath->getText();
  if (startPath.empty())
    startPath = ".";
  FXString path = FileDialog::getOpenFilename(this, "Select dumper info file",
    startPath);
  if (path.text()[0])
    m_dumperInfoPath->setText(path);
  return 1;
}


long
NESFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NESFrame::onShowFDSList(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--fdsl", true);
  m_commandLineHandler->addOption("-q");
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NESFrame::onStripHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--stp", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NESFrame::onExtractiNESHeader(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--ineshd", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NESFrame::onName(FXObject *, FXSelector, void *)
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
NESFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((intptr_t) a_data)
    {
      m_overrideNES->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
      m_overrideNotSplit->enable();
    }
  else
    {
      m_overrideNES->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
      m_overrideNotSplit->disable();
    }
  return 1;
}


long
NESFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
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
