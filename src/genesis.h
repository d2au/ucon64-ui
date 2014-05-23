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
#ifndef GENESIS_H
#define GENESIS_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "TextFieldOption.h"
#include "ComboBox.h"


class GenesisFrame : public FXVerticalFrame
{
  FXDECLARE(GenesisFrame)
public:
  GenesisFrame(CommandLineHandler *commandLineHandler,
      FXComposite *p, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
      FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
      FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING);
  virtual ~GenesisFrame();
  void writeToRegistry(void);
  long onDeviceType(FXObject *src, FXSelector selector, void *data);
  long onTransferDataType(FXObject *src, FXSelector selector, void *data);
  long onSend(FXObject *src, FXSelector selector, void *data);
  long onReceive(FXObject *src, FXSelector selector, void *data);
  long onTest(FXObject *src, FXSelector selector, void *data);
  long onTestType(FXObject *src, FXSelector selector, void *data);
  long onMultiGamePath(FXObject *src, FXSelector selector, void *data);
  long onMultiGame(FXObject *src, FXSelector selector, void *data);
  long onDataType(FXObject *src, FXSelector selector, void *data);
  long onConvert(FXObject *src, FXSelector selector, void *data);
  long onSplit(FXObject *src, FXSelector selector, void *data);
  long onJoin(FXObject *src, FXSelector selector, void *data);
  long onRegionFix(FXObject *src, FXSelector selector, void *data);
  long onShowInfo(FXObject *src, FXSelector selector, void *data);
  long onStripHeader(FXObject *src, FXSelector selector, void *data);
  long onFixChecksum(FXObject *src, FXSelector selector, void *data);
  long onName(FXObject *src, FXSelector selector, void *data);
  long on1991(FXObject *src, FXSelector selector, void *data);
  long onOverride(FXObject *src, FXSelector selector, void *data);
  long onHeader(FXObject *src, FXSelector selector, void *data);
  long onInterleave(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_SEND = FXVerticalFrame::ID_LAST,
      ID_RECEIVE,
      ID_TEST,
      ID_DEVICETYPE,
      ID_TRANSFERDATATYPE,
      ID_TESTTYPE,
      ID_REGIONFIX,
      ID_MULTIGAMEPATH,
      ID_MULTIGAME,
      ID_DATATYPE,
      ID_CONVERT,
      ID_SPLIT,
      ID_JOIN,
      ID_SHOWINFO,
      ID_STRIPHEADER,
      ID_FIXCHECKSUM,
      ID_NAME,
      ID_1991,
      ID_OVERRIDE,
      ID_HEADER1,
      ID_HEADER2,
      ID_HEADER3,
      ID_INTERLEAVE,
      ID_LAST
    };

protected:
  GenesisFrame() {}
  void setSendButtonState(FXint deviceType, FXint transferDataType);
  void setTransferOption(bool fileNeeded);
  void setOverrideOptions(void);

  CommandLineHandler *m_commandLineHandler;
  ComboBox *m_deviceTypes, *m_transferDataTypes, *m_speed, *m_test, *m_region,
    *m_dataTypes, *m_formats;
  FXTextField *m_bank, *m_multiGameName, *m_multiGameSize, *m_splitSize,
    *m_foreignName, *m_JapaneseName, *m_overrideHeaderSizeText;
  TextFieldOption *m_loaderPath;
  FXButton *m_sendButton, *m_testButton, *m_selectFilesButton,
    *m_changeForeignName, *m_changeJapaneseName;
  FXCheckButton *m_useOverride, *m_overrideGenesis, *m_overrideNotSplit;
  FXRadioButton *m_overrideHeader, *m_overrideNoHeader, *m_overrideHeaderSize,
    *m_overrideInterleaved, *m_overrideInterleaved2, *m_overrideNotInterleaved;
  FXLabel *m_bankLabel, *m_speedLabel;
  char *m_formatsText[3];
  int m_headerChoice;
};

#endif
