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
#ifndef NES_H
#define NES_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "TextFieldOption.h"
#include "ComboBox.h"


class NESFrame : public FXVerticalFrame
{
  FXDECLARE(NESFrame)
public:
  NESFrame(CommandLineHandler *commandLineHandler,
      FXComposite *p, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
      FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
      FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING);
  virtual ~NESFrame();
  void writeToRegistry(void);
  long onTransferDataType(FXObject *src, FXSelector selector, void *data);
  long onSend(FXObject *src, FXSelector selector, void *data);
  long onReceive(FXObject *src, FXSelector selector, void *data);
  long onFormat(FXObject *src, FXSelector selector, void *data);
  long onConvert(FXObject *src, FXSelector selector, void *data);
  long onBattery(FXObject *src, FXSelector selector, void *data);
  long onMirroring(FXObject *src, FXSelector selector, void *data);
  long onController(FXObject *src, FXSelector selector, void *data);
  long onTV(FXObject *src, FXSelector selector, void *data);
  long onVRAM(FXObject *src, FXSelector selector, void *data);
  long onDumperInfoPath(FXObject *src, FXSelector selector, void *data);
  long onShowInfo(FXObject *src, FXSelector selector, void *data);
  long onShowFDSList(FXObject *src, FXSelector selector, void *data);
  long onStripHeader(FXObject *src, FXSelector selector, void *data);
  long onExtractiNESHeader(FXObject *src, FXSelector selector, void *data);
  long onName(FXObject *src, FXSelector selector, void *data);
  long onOverride(FXObject *src, FXSelector selector, void *data);
  long onHeader(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_SEND = FXVerticalFrame::ID_LAST,
      ID_RECEIVE,
      ID_TRANSFERDATATYPE,
      ID_FORMAT,
      ID_CONVERT,
      ID_BATTERY,
      ID_MIRRORING,
      ID_CONTROLLER,
      ID_TV,
      ID_VRAM,
      ID_DUMPINFOPATH,
      ID_SHOWINFO,
      ID_SHOWFDSLIST,
      ID_STRIPHEADER,
      ID_INESHD,
      ID_NAME,
      ID_OVERRIDE,
      ID_HEADER1,
      ID_HEADER2,
      ID_HEADER3,
      ID_LAST
    };

protected:
  NESFrame() {}
  void setSendButtonState(FXint deviceType, FXint transferDataType);
  void setTransferOption(bool fileNeeded);
  void setOverrideOptions(void);
  bool setConversionOptions(void);

  CommandLineHandler *m_commandLineHandler;
  ComboBox *m_deviceTypes, *m_transferDataTypes, *m_formats;
  FXTextField *m_mapper, *m_comment, *m_dumperInfoPath, *m_internalName,
    *m_overrideHeaderSizeText;
  FXButton *m_receiveButton, *m_dumperInfoPathButton;
  FXCheckButton *m_controller[7], *m_televisionType[3], *m_useOverride,
    *m_overrideNES, *m_overrideNotSplit;
  FXRadioButton *m_overrideHeader, *m_overrideNoHeader, *m_overrideHeaderSize;
  FXLabel *m_mapperLabel, *m_batteryLabel, *m_battery[3], // FXCheckButton & FXRadioButtons
    *m_mirroringLabel, *m_mirroring[7], // FXCheckButton & FXRadioButtons
    *m_controllerLabel, *m_televisionTypeLabel, *m_VRAMOverrideLabel,
    *m_VRAMOverride[3] /* FXCheckButton & FXRadioButtons */, *m_commentLabel,
    *m_dumperInfoLabel;
  int m_headerChoice;
};

#endif
