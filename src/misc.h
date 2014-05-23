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
#ifndef MISC_H
#define MISC_H

#include <fx.h>
#include "CommandLineHandler.h"
#include "TextFieldOption.h"
#include "ComboBox.h"


class MiscFrame : public FXVerticalFrame
{
  FXDECLARE(MiscFrame)
public:
  MiscFrame(CommandLineHandler *commandLineHandler,
      FXComposite *p, FXuint opts = 0,
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0,
      FXint pl = DEFAULT_SPACING, FXint pr = DEFAULT_SPACING,
      FXint pt = DEFAULT_SPACING, FXint pb = DEFAULT_SPACING,
      FXint hs = DEFAULT_SPACING, FXint vs = DEFAULT_SPACING);
  virtual ~MiscFrame();
  void writeToRegistry(void);
  long onPatchType(FXObject *src, FXSelector selector, void *data);
  long onPatchPath(FXObject *src, FXSelector selector, void *data);
  long onApplyPatch(FXObject *src, FXSelector selector, void *data);
  long onModPath(FXObject *src, FXSelector selector, void *data);
  long onCreatePatch(FXObject *src, FXSelector selector, void *data);
  long onChangeDescription(FXObject *src, FXSelector selector, void *data);
  long onChangeDescription2(FXObject *src, FXSelector selector, void *data);
  long onChangeDescription2Path(FXObject *src, FXSelector selector, void *data);
  long onPoke(FXObject *src, FXSelector selector, void *data);
  long onPatternPath(FXObject *src, FXSelector selector, void *data);
  long onApplyPattern(FXObject *src, FXSelector selector, void *data);
  long onGameGenie(FXObject *src, FXSelector selector, void *data);
  long onSearchString(FXObject *src, FXSelector selector, void *data);
  long onSearchType(FXObject *src, FXSelector selector, void *data);
  long onUseDATConsoleType(FXObject *src, FXSelector selector, void *data);
  long onDATConsoleType(FXObject *src, FXSelector selector, void *data);
  long onDATInfo(FXObject *src, FXSelector selector, void *data);
  long onCreateDAT(FXObject *src, FXSelector selector, void *data);
  long onSearchCRC32(FXObject *src, FXSelector selector, void *data);
  long onRename(FXObject *src, FXSelector selector, void *data);
  long onCalcChecksum(FXObject *src, FXSelector selector, void *data);
  long onOverrideHeaderSize(FXObject *src, FXSelector selector, void *data);
  long onCheckPad(FXObject *src, FXSelector selector, void *data);
  long onPadNextMB(FXObject *src, FXSelector selector, void *data);
  long onPadToSize(FXObject *src, FXSelector selector, void *data);
  long onStripEnd(FXObject *src, FXSelector selector, void *data);
  long onStripStart(FXObject *src, FXSelector selector, void *data);
  long onInsertStart(FXObject *src, FXSelector selector, void *data);

  enum
    {
      ID_PATCHTYPE = FXVerticalFrame::ID_LAST,
      ID_PATCHPATH,
      ID_APPLYPATCH,
      ID_MODPATH,
      ID_CREATEPATCH,
      ID_CHANGEDESCRIPTION,
      ID_CHANGEDESCRIPTION2,
      ID_CHANGEDESCRIPTION2PATH,
      ID_POKE,
      ID_PATTERNPATH,
      ID_APPLYPATTERN,
      ID_GAMEGENIE,
      ID_SEARCHSTRING,
      ID_SEARCHTYPE,
      ID_USEDATCONSOLE,
      ID_DATCONSOLE,
      ID_DATINFO,
      ID_CREATEDAT,
      ID_SEARCHCRC32,
      ID_RENAME,
      ID_CALCCHECKSUM,
      ID_OVERRIDEHEADERSIZE,
      ID_CHECKPAD,
      ID_PADNEXTMB,
      ID_PADTOSIZE,
      ID_STRIPEND,
      ID_STRIPSTART,
      ID_INSERTSTART,
      ID_LAST
    };

protected:
  MiscFrame() {}
  void updateRenameTypes(void);
  void setDATFilename(void);
  void setDATConsoleOption(void);

  CommandLineHandler *m_commandLineHandler;
  ComboBox *m_patchTypes, *m_GameGenieConsoles, *m_DATConsoles, *m_renameTypes,
    *m_checksumTypes;
  FXTextField *m_patchTargetPath, *m_patchPath, *m_patchDescription,
    *m_patchDescription2Path, *m_modifiedFilePath, *m_pokeOffset, *m_pokeValue,
    *m_patternFilePath, *m_GameGenieCode, *m_searchString, *m_DATFilename,
    *m_searchCRC32, *m_headerSize, *m_padNewSize, *m_stripEnd, *m_stripStart,
    *m_insertStart;
  FXButton *m_modifiedFilePathButton, *m_createPatchButton,
    *m_patchDescriptionButton, *m_patchDescription2Button,
    *m_patchDescription2PathButton, *m_pokeButton, *m_GameGenieEncodeButton,
    *m_GameGenieDecodeButton, *m_GameGenieApplyButton;
  FXCheckButton *m_useGameGenieConsoles, *m_useDATConsoles, *m_overrideHeaderSize;
  FXRadioButton *m_searchTypes[3];
  FXLabel *m_modifiedFilePathLabel;
  char *m_renameTypesText[4];
};

#endif
