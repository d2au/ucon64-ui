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
#include "Frontend.h"
#include "ngp.h"


FXDEFMAP(NGPFrame) NGPFrameMap[] =
{
  FXMAPFUNC(SEL_COMMAND, NGPFrame::ID_SEND, NGPFrame::onSend),
  FXMAPFUNC(SEL_COMMAND, NGPFrame::ID_RECEIVE, NGPFrame::onReceive),
  FXMAPFUNC(SEL_COMMAND, NGPFrame::ID_CARTINFO, NGPFrame::onCartridgeInfo),
  FXMAPFUNC(SEL_COMMAND, NGPFrame::ID_SHOWINFO, NGPFrame::onShowInfo),
  FXMAPFUNC(SEL_COMMAND, NGPFrame::ID_OVERRIDE, NGPFrame::onOverride),
  FXMAPFUNCS(SEL_COMMAND, NGPFrame::ID_HEADER1, NGPFrame::ID_HEADER3, NGPFrame::onHeader)
};

FXIMPLEMENT(NGPFrame, FXVerticalFrame, NGPFrameMap, ARRAYNUMBER(NGPFrameMap))


NGPFrame::NGPFrame(CommandLineHandler *a_commandLineHandler,
    FXComposite *a_p, FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h,
    FXint a_pl, FXint a_pr, FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXVerticalFrame(a_p, a_opts, a_x, a_y, a_w, a_h, a_pl, a_pr, a_pt, a_pb,
        a_hs, a_vs)
{
  m_commandLineHandler = a_commandLineHandler;

  Frontend *app = (Frontend *) getApp();
  FXFont *smallFont = app->getSmallFont();
  FXFont *boldFont = app->getBoldFont();

  FXVerticalFrame *windowArea = new FXVerticalFrame(this, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

#ifndef DISABLE_COPIER
  FXGroupBox *transferArea = new FXGroupBox(windowArea, "Data transfer",
    FRAME_GROOVE | LAYOUT_FILL);
  transferArea->setFont(boldFont);
  FXHorizontalFrame *transferAreaTop = new FXHorizontalFrame(transferArea, LAYOUT_FILL_X,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXMatrix *transferAreaTopLeft = new FXMatrix(transferAreaTop, 2,
    MATRIX_BY_COLUMNS | LAYOUT_BOTTOM, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0););
  new FXLabel(transferAreaTopLeft, "Device:");
  m_deviceTypes = new ComboBox(transferAreaTopLeft, 1, NULL, 0,
    FRAME_SUNKEN | COMBOBOX_STATIC | LAYOUT_FILL_X);
  m_deviceTypes->appendItem("Pocket Linker");
  m_deviceTypes->setNumVisible(1);

  FXVerticalFrame *transferAreaTopRight = new FXVerticalFrame(transferAreaTop, LAYOUT_FILL_X,
    0, 0, 0, 0, DEFAULT_PAD, 0, DEFAULT_PAD, 0);//, 0, 0);
  m_useEPP = new FXCheckButton(transferAreaTopRight, "Use EPP");
  m_useEPP->setTipText("This *tries* to enable EPP mode");

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
  new FXButton(transferAreaBottom, "Show cartridge info", NULL, this, ID_CARTINFO);
#endif // DISABLE_COPIER

  FXGroupBox *miscArea = new FXGroupBox(windowArea, "Miscellaneous",
    FRAME_GROOVE | LAYOUT_FILL);
  miscArea->setFont(boldFont);
  new FXButton(miscArea, "Show info", NULL, this, ID_SHOWINFO,
    BUTTON_NORMAL | LAYOUT_CENTER_X, 0, 0, 0, 0, 10, 10);

  FXGroupBox *overrideArea = new FXGroupBox(windowArea, "Override auto-detection",
    FRAME_GROOVE | LAYOUT_FILL);
  overrideArea->setFont(boldFont);

  FXVerticalFrame *overrideButtonsArea = new FXVerticalFrame(overrideArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_useOverride = new FXCheckButton(overrideButtonsArea, "Use override options", this, ID_OVERRIDE);
  m_useOverride->setFont(smallFont);

  new FXHorizontalSeparator(overrideButtonsArea, LAYOUT_FILL_X | SEPARATOR_GROOVE);
  m_overrideNGP = new FXCheckButton(overrideButtonsArea, "File is a Neo Geo Pocket file");
  m_overrideNGP->setFont(smallFont);

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
  m_overrideHeaderSizeText->disable();
  m_overrideHeader = new FXRadioButton(headerSizeArea, "File has header", this, ID_HEADER2);
  m_overrideHeader->setFont(smallFont);
  m_overrideNoHeader = new FXRadioButton(headerSizeArea, "File has no header", this, ID_HEADER3);
  m_overrideNoHeader->setFont(smallFont);
  m_headerChoice = 0;

  handle(m_useOverride, FXSEL(SEL_COMMAND, ID_OVERRIDE), (void *) 0);
}


NGPFrame::~NGPFrame()
{
#ifndef DISABLE_COPIER
  delete m_deviceTypes;
#endif
}


void
NGPFrame::writeToRegistry(void)
{
}


void
NGPFrame::setTransferOption(bool a_fileNeeded)
{
  switch (m_deviceTypes->getCurrentItem())
    {
    case 0:
      m_commandLineHandler->setOption("--xpl", a_fileNeeded);
      if (m_useEPP->getCheck())
        m_commandLineHandler->addOption("--xplm");
      break;
    }
}


void
NGPFrame::setOverrideOptions(void)
{
  if (m_useOverride->getCheck())
    {
      if (m_overrideNGP->getCheck())
        m_commandLineHandler->addOption("--ngp");
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
NGPFrame::onSend(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  setTransferOption(true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NGPFrame::onReceive(FXObject *, FXSelector, void *)
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
NGPFrame::onCartridgeInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("--xpli", false);
  if (m_useEPP->getCheck())
    m_commandLineHandler->addOption("--xplm");
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NGPFrame::onShowInfo(FXObject *, FXSelector, void *)
{
  if (m_commandLineHandler->busy())
    return 1;

  m_commandLineHandler->setOption("", true);
  setOverrideOptions();
  m_commandLineHandler->run();
  return 1;
}


long
NGPFrame::onOverride(FXObject *, FXSelector, void *a_data)
{
  if ((int) a_data)
    {
      m_overrideNGP->enable();
      m_overrideHeaderSize->enable();
      if (m_headerChoice == ID_HEADER1)
        m_overrideHeaderSizeText->enable();
      m_overrideHeader->enable();
      m_overrideNoHeader->enable();
    }
  else
    {
      m_overrideNGP->disable();
      m_overrideHeaderSize->disable();
      m_overrideHeaderSizeText->disable();
      m_overrideHeader->disable();
      m_overrideNoHeader->disable();
    }
  return 1;
}


long
NGPFrame::onHeader(FXObject *a_src, FXSelector a_sel, void *)
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
