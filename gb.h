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
#ifndef GB_H
#define GB_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "TextFieldOption.h"
#include "ComboBox.h"


class GBFrame : public FXVerticalFrame
{
  FXDECLARE(GBFrame)
public:
  GBFrame(CommandLineHandler *commandLineHandler,
      FXComposite *p, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
      FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
      FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING);
  virtual ~GBFrame();
  void writeToRegistry(void);
  long onDeviceType(FXObject *src, FXSelector selector, void *data);
  long onTransferDataType(FXObject *src, FXSelector selector, void *data);
  long onSend(FXObject *src, FXSelector selector, void *data);
  long onReceive(FXObject *src, FXSelector selector, void *data);
  long onDataType(FXObject *src, FXSelector selector, void *data);
  long onConvert(FXObject *src, FXSelector selector, void *data);
  long onShowInfo(FXObject *src, FXSelector selector, void *data);
  long onStripHeader(FXObject *src, FXSelector selector, void *data);
  long onFixChecksum(FXObject *src, FXSelector selector, void *data);
  long onFixLogo(FXObject *src, FXSelector selector, void *data);
  long onName(FXObject *src, FXSelector selector, void *data);
  long onCreateFCEmuROM(FXObject *src, FXSelector selector, void *data);
  long onNESROMPath(FXObject *src, FXSelector selector, void *data);
  long onOverride(FXObject *src, FXSelector selector, void *data);
  long onHeader(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_SEND = FXVerticalFrame::ID_LAST,
      ID_RECEIVE,
      ID_DEVICETYPE,
      ID_TRANSFERDATATYPE,
      ID_CONVERT,
      ID_SHOWINFO,
      ID_STRIPHEADER,
      ID_FIXCHECKSUM,
      ID_FIXLOGO,
      ID_NAME,
      ID_CREATEFCEMUROM,
      ID_NESROMPATH,
      ID_OVERRIDE,
      ID_HEADER1,
      ID_HEADER2,
      ID_HEADER3,
      ID_LAST
    };

protected:
  GBFrame() {}
  void setSendButtonState(FXint deviceType, FXint transferDataType);
  void setTransferOption(bool fileNeeded);
  void setOverrideOptions(void);

  CommandLineHandler *m_commandLineHandler;
  ComboBox *m_deviceTypes, *m_transferDataTypes, *m_formats;
  FXTextField *m_bank, *m_internalName, *m_NESROMPath, *m_overrideHeaderSizeText;
  FXButton *m_sendButton;
  FXCheckButton *m_useEPP, *m_useOverride, *m_overrideGameBoy;
  FXRadioButton *m_overrideHeader, *m_overrideNoHeader, *m_overrideHeaderSize;
  FXLabel *m_bankLabel;
  int m_headerChoice;
};

#endif
