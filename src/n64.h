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
#ifndef N64_H
#define N64_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "TextFieldOption.h"
#include "ComboBox.h"


class N64Frame : public FXVerticalFrame
{
  FXDECLARE(N64Frame)
public:
  N64Frame(CommandLineHandler *commandLineHandler,
      FXComposite *p, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
      FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
      FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING);
  virtual ~N64Frame();
  void writeToRegistry(void);
  long onDeviceType(FXObject *src, FXSelector selector, void *data);
  long onTransferDataType(FXObject *src, FXSelector selector, void *data);
  long onSend(FXObject *src, FXSelector selector, void *data);
  long onReceive(FXObject *src, FXSelector selector, void *data);
  long onDataType(FXObject *src, FXSelector selector, void *data);
  long onConvert(FXObject *src, FXSelector selector, void *data);
  long onReplaceBootCode(FXObject *src, FXSelector selector, void *data);
  long onReplaceBootCodePath(FXObject *src, FXSelector selector, void *data);
  long onExtractBootCode(FXObject *src, FXSelector selector, void *data);
  long onShowInfo(FXObject *src, FXSelector selector, void *data);
  long onFixChecksum(FXObject *src, FXSelector selector, void *data);
  long onName(FXObject *src, FXSelector selector, void *data);
  long onCreateSRAMROM(FXObject *src, FXSelector selector, void *data);
  long onSRAMPath(FXObject *src, FXSelector selector, void *data);
  long onCreateSMSEmuROM(FXObject *src, FXSelector selector, void *data);
  long onSMSROMPath(FXObject *src, FXSelector selector, void *data);
  long onOverride(FXObject *src, FXSelector selector, void *data);
  long onHeader(FXObject *src, FXSelector selector, void *data);
  long onInterleave(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_SEND = FXVerticalFrame::ID_LAST,
      ID_RECEIVE,
      ID_DEVICETYPE,
      ID_TRANSFERDATATYPE,
      ID_CD64PROTOCOL,
      ID_CONVERT,
      ID_REPLACEBOOTCODE,
      ID_REPLACEBOOTCODEPATH,
      ID_EXTRACTBOOTCODE,
      ID_SHOWINFO,
      ID_FIXCHECKSUM,
      ID_NAME,
      ID_CREATESRAMROM,
      ID_SRAMPATH,
      ID_CREATESMSEMUROM,
      ID_SMSROMPATH,
      ID_OVERRIDE,
      ID_HEADER1,
      ID_HEADER2,
      ID_HEADER3,
      ID_INTERLEAVE,
      ID_LAST
    };

protected:
  N64Frame() {}
  void setSendButtonState(FXint deviceType, FXint transferDataType);
  bool setTransferOptions(bool receive, bool fileNeeded);
  void setOverrideOptions(void);

  CommandLineHandler *m_commandLineHandler;
  ComboBox *m_deviceTypes, *m_transferDataTypes, *m_CD64Protocol, *m_formats;
  FXTextField *m_index, *m_receiveSize, *m_replaceBootCodePath, *m_extractBootCodeName,
    *m_internalName, *m_SRAMPath, *m_SMSROMPath, *m_overrideHeaderSizeText;
  FXButton *m_receiveButton;
  FXCheckButton *m_useOverride, *m_overrideN64;
  FXRadioButton *m_overrideHeader, *m_overrideNoHeader, *m_overrideHeaderSize,
    *m_overrideInterleaved, *m_overrideNotInterleaved;
  FXLabel *m_indexLabel, *m_CD64ProtocolLabel, *m_receiveSizeLabel;
  int m_headerChoice;
};

#endif
