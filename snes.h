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
#ifndef SNES_H
#define SNES_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "TextFieldOption.h"
#include "ComboBox.h"
#include "SNESControllerSettings.h"


class SNESFrame : public FXVerticalFrame
{
  FXDECLARE(SNESFrame)
public:
  SNESFrame(CommandLineHandler *commandLineHandler,
      FXComposite *p, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
      FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
      FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING);
  virtual ~SNESFrame();
  void writeToRegistry(void);
  long onDeviceType(FXObject *src, FXSelector selector, void *data);
  long onTransferDataType(FXObject *src, FXSelector selector, void *data);
  long onSend(FXObject *src, FXSelector selector, void *data);
  long onReceive(FXObject *src, FXSelector selector, void *data);
  long onMultiGamePath(FXObject *src, FXSelector selector, void *data);
  long onMultiGame(FXObject *src, FXSelector selector, void *data);
  long onDataType(FXObject *src, FXSelector selector, void *data);
  long onFormat(FXObject *src, FXSelector selector, void *data);
  long onConvert(FXObject *src, FXSelector selector, void *data);
  long onController(FXObject *src, FXSelector selector, void *data);
  long onSplit(FXObject *src, FXSelector selector, void *data);
  long onJoin(FXObject *src, FXSelector selector, void *data);
  long onCrack(FXObject *src, FXSelector selector, void *data);
  long onRegionFix(FXObject *src, FXSelector selector, void *data);
  long onSlowFix(FXObject *src, FXSelector selector, void *data);
  long onShowInfo(FXObject *src, FXSelector selector, void *data);
  long onDisplayHeader(FXObject *src, FXSelector selector, void *data);
  long onStripHeader(FXObject *src, FXSelector selector, void *data);
  long onFixChecksum(FXObject *src, FXSelector selector, void *data);
  long onDeMirror(FXObject *src, FXSelector selector, void *data);
  long onDeNSRT(FXObject *src, FXSelector selector, void *data);
  long onName(FXObject *src, FXSelector selector, void *data);
  long onOverride(FXObject *src, FXSelector selector, void *data);
  long onHeader(FXObject *src, FXSelector selector, void *data);
  long onBankSize(FXObject *src, FXSelector selector, void *data);
  long onInterleave(FXObject *src, FXSelector selector, void *data);
  long onBS(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_SEND = FXVerticalFrame::ID_LAST,
      ID_RECEIVE,
      ID_DEVICETYPE,
      ID_TRANSFERDATATYPE,
      ID_CRACK,
      ID_REGIONFIX,
      ID_SLOWFIX,
      ID_MULTIGAMEPATH,
      ID_MULTIGAME,
      ID_DATATYPE,
      ID_FORMAT,
      ID_CONVERT,
      ID_CONTROLLER,
      ID_SPLIT,
      ID_JOIN,
      ID_SHOWINFO,
      ID_DISPLAYHEADER,
      ID_STRIPHEADER,
      ID_FIXCHECKSUM,
      ID_DEMIRROR,
      ID_DENSRT,
      ID_NAME,
      ID_OVERRIDE,
      ID_HEADER1,
      ID_HEADER2,
      ID_HEADER3,
      ID_BANKSIZE,
      ID_INTERLEAVE,
      ID_BS,
      ID_LAST
    };

protected:
  SNESFrame() {}
  void setReceiveButtonState(FXint deviceType, FXint transferDataType);
  void setTransferOption(bool fileNeeded);
  void setIOMode(void);
  void setOverrideOptions(void);

  CommandLineHandler *m_commandLineHandler;
  ComboBox *m_deviceTypes, *m_transferDataTypes, *m_dataTypes, *m_formats;
  FXTextField *m_multiGameName, *m_multiGameSize, *m_splitSize, *m_internalName,
    *m_overrideHeaderSizeText;
  TextFieldOption *m_loaderPath;
  FXButton *m_receiveButton, *m_selectFilesButton, *m_controllerButton;
  FXCheckButton *m_IOMode[9], *m_useOverride, *m_overrideSNES,
    *m_overrideExtended, *m_overrideNotSplit;
  FXRadioButton *m_overrideHeader, *m_overrideNoHeader, *m_overrideHeaderSize,
    *m_overrideHiROM, *m_overrideLoROM, *m_overrideBSDump,
    *m_overrideRegCartDump, *m_overrideInterleaved, *m_overrideSFXInterleaved,
    *m_overrideNotInterleaved;
  SNESControllerSettings *m_controllerSettings;
  char *m_transferDataTypesText[5], *m_formatsText[6];
  int m_headerChoice;
};

#endif
