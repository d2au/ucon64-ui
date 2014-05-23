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
#include <string.h>
#include <fstream>
#include <fx.h>
#include "FileSelector.h"
#include "MultiGameFileSelector.h"
#include "Frontend.h"


class FileInfo
/*
  This class is needed in case some smart-ass user tries to delete a file (from
  the filesystem) that was added to the selected files list and *then* removes
  it from that list. We wouldn't know what the total size is unless we store the
  file size beforehand.
*/
{
public:
  FileInfo(FXString a_name, int a_size) : name(a_name), size(a_size) {}

  FXString name;
  int size;

protected:
  FileInfo() {}
};


FXDEFMAP(MultiGameFileSelector) MultiGameFileSelectorMap[] =
{
  FXMAPFUNC(SEL_COMMAND, MultiGameFileSelector::ID_ADDFILE, MultiGameFileSelector::onAddFile),
  FXMAPFUNC(SEL_COMMAND, MultiGameFileSelector::ID_REMOVEFILE, MultiGameFileSelector::onRemoveFile),
  FXMAPFUNC(SEL_COMMAND, MultiGameFileSelector::ID_CREATE, MultiGameFileSelector::onCreate),
//  FXMAPFUNC(SEL_CLOSE, 0, MultiGameFileSelector::onClose) // MultiGameFileSelector::ID_CLOSE does not work (!)
};

//FXIMPLEMENT(MultiGameFileSelector, FXTopWindow, MultiGameFileSelectorMap, ARRAYNUMBER(MultiGameFileSelectorMap))
FXIMPLEMENT(MultiGameFileSelector, FXDialogBox, MultiGameFileSelectorMap, ARRAYNUMBER(MultiGameFileSelectorMap))


MultiGameFileSelector::MultiGameFileSelector(FXWindow *a_owner, int a_blockSize,
    const FXString &a_name, FXIcon *a_icon, FXIcon *a_icon2, FXuint a_opts,
    FXint a_x, FXint a_y, FXint a_w, FXint a_h, FXint a_pl, FXint a_pr,
    FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
//  : FXTopWindow(a_owner, a_name, a_icon, a_icon2, a_opts, a_x, a_y, a_w, a_h,
//        a_pl, a_pr, a_pt, a_pb, a_hs, a_vs)
  : FXDialogBox(a_owner, a_name, a_opts, a_x, a_y, a_w, a_h,
        a_pl, a_pr, a_pt, a_pb, a_hs, a_vs)
{
#if 1
  setIcon(a_icon);
  setMiniIcon(a_icon2);
#endif
  setWidth(600);
  setHeight(450);

  FXHorizontalFrame *windowArea = new FXHorizontalFrame(this,
    LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXSpring *leftArea = new FXSpring(windowArea, LAYOUT_FILL, 50, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXVerticalFrame *fileSelectorArea = new FXVerticalFrame(leftArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  m_fileSelector = new FileSelector(fileSelectorArea, NULL, 0,
    LAYOUT_FILL | SELECTFILE_NOENTRYBLOCK | SELECTFILE_SMALLDIRBOX);
  m_fileSelector->setFileBoxStyle(ICONLIST_MINI_ICONS); // ICONLIST_DETAILED, ICONLIST_BIG_ICONS
  m_fileSelector->setSelectMode(SELECTFILE_MULTIPLE);

  FXSpring *rightArea = new FXSpring(windowArea, LAYOUT_FILL, 50, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXHorizontalFrame *buttonFilesArea = new FXHorizontalFrame(rightArea, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

  FXVerticalFrame *buttonAreaContainer = new FXVerticalFrame(buttonFilesArea,
    LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 2 * DEFAULT_PAD, 0, 0);//, 0, 0);
  FXVerticalFrame *buttonArea = new FXVerticalFrame(buttonAreaContainer,
    LAYOUT_CENTER_X | LAYOUT_CENTER_Y | PACK_UNIFORM_WIDTH, 0, 0, 0, 0, 0, 0, 0, 0);//, 0, 0);
  FXButton *addButton = new FXButton(buttonArea, "Add", NULL, this, ID_ADDFILE);
  addButton->setTipText("Add to selected files list");
  m_removeButton = new FXButton(buttonArea, "Remove", NULL, this, ID_REMOVEFILE);
  m_removeButton->setTipText("Remove from selected files list");

  FXVerticalFrame *selectedFilesAreaContainer = new FXVerticalFrame(buttonFilesArea,
    LAYOUT_FILL, 0, 0, 0, 0, 0, 2 * DEFAULT_PAD, 2 * DEFAULT_PAD, 2 * DEFAULT_PAD);//, 0, 0);
  FXVerticalFrame *selectedFilesArea = new FXVerticalFrame(selectedFilesAreaContainer,
    FRAME_NORMAL | LAYOUT_FILL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  FXLabel *header = new FXLabel(selectedFilesArea, "Selected files", NULL, FRAME_RAISED | LAYOUT_FILL_X);
  header->setJustify(JUSTIFY_LEFT);
  m_selectedFiles = new FXList(selectedFilesArea, NULL, 0, LAYOUT_FILL);
  m_totalSizeLabel = new FXLabel(selectedFilesAreaContainer, "");
  m_totalSizeLabel->setTipText("Estimated space that is needed on a flash card");
  new FXButton(selectedFilesAreaContainer, "Create file", NULL, this, ID_CREATE,
    BUTTON_NORMAL | LAYOUT_CENTER_X);

  m_blockSize = a_blockSize;
  m_totalSize = 0;
  FXString *filenames = ((Frontend *) getApp())->getMainWindow()->getSelectedFilenames();
  if (filenames)
    {
      int n = 0;
      while (filenames[n][0])
        {
          int size = FXFile::size(filenames[n]);
          if (size % 32768 == 512)
            size -= 512;                        // quick hack to handle backup unit headers
          m_selectedFiles->appendItem(new FXListItem(FXFile::name(filenames[n]),
            NULL, new FileInfo(filenames[n], size)));
          m_totalSize += (size + m_blockSize - 1) & ~(m_blockSize - 1);
          n++;
        }
      delete [] filenames;
    }
  m_totalSizeLabel->setText(FXStringFormat("Total size: %d Mbit",
    (m_totalSize + 128 * 1024 - 1) / (128 * 1024))); // round up
  if (m_totalSize == 0)
    m_removeButton->disable();
}


MultiGameFileSelector::~MultiGameFileSelector()
{
  delete m_fileSelector;
  delete m_removeButton;
  while (m_selectedFiles->getNumItems())
    {
      FXListItem *item = m_selectedFiles->getItem(0);
      FileInfo *fileInfo = (FileInfo *) item->getData();
      m_selectedFiles->removeItem(0);
      delete fileInfo;
    }
  delete m_selectedFiles;
  delete m_totalSizeLabel;
}


long
MultiGameFileSelector::onAddFile(FXObject *, FXSelector, void *)
{
  FXString *filenames = m_fileSelector->getFilenames();
  if (filenames)
    {
      int n = 0;
      while (filenames[n][0])
        {
          int size = FXFile::size(filenames[n]);
          if (size % 32768 == 512)
            size -= 512;                        // quick hack to handle backup unit headers
          m_selectedFiles->appendItem(new FXListItem(FXFile::name(filenames[n]),
            NULL, new FileInfo(filenames[n], size)));
          m_totalSize += (size + m_blockSize - 1) & ~(m_blockSize - 1);
          n++;
        }
      delete [] filenames;
    }
  else
    FXMessageBox::information(this, MBOX_OK, "No file selected",
      "Please select a file on the left");

  m_totalSizeLabel->setText(FXStringFormat("Total size: %d Mbit",
    (m_totalSize + 128 * 1024 - 1) / (128 * 1024)));
  if (m_totalSize)
    m_removeButton->enable();
  else
    m_removeButton->disable();
  return 1;
}


long
MultiGameFileSelector::onRemoveFile(FXObject *, FXSelector, void *)
{
  int n = 0;
  bool fileSelected = false;
  while (n < m_selectedFiles->getNumItems())
    {
      if (m_selectedFiles->isItemSelected(n))
        {
          fileSelected = true;
          FXListItem *item = m_selectedFiles->getItem(n);
          FileInfo *fileInfo = (FileInfo *) item->getData();
          m_selectedFiles->removeItem(n);
          m_totalSize -= (fileInfo->size + m_blockSize - 1) & ~(m_blockSize - 1);
          delete fileInfo;
        }
      else
        n++;
    }
  if (!fileSelected)
    FXMessageBox::information(this, MBOX_OK, "No file selected",
      "Please select a file on the right");
  m_totalSizeLabel->setText(FXStringFormat("Total size: %d Mbit",
    (m_totalSize + 128 * 1024 - 1) / (128 * 1024)));
  if (m_totalSize)
    m_removeButton->enable();
  else
    m_removeButton->disable();
  return 1;
}


#if 0
int
MultiGameFileSelector::execute(FXuint a_placement)
{
  create();
  show(a_placement);
  getApp()->refresh();
  return getApp()->runModalFor(this);
}
#endif


long
MultiGameFileSelector::onCreate(FXObject *, FXSelector, void *)
{
  int nItems = m_selectedFiles->getNumItems(), n;
  FXString *filenames = new FXString[nItems + 1];
  for (n = 0; n < nItems; n++)
    {
      FXListItem *item = m_selectedFiles->getItem(n);
      filenames[n] = *(FXString *) item->getData();
    }
  filenames[n] = FXString::null;
  ((Frontend *) getApp())->getMainWindow()->getCommandLineHandler()->
    setSelectedFilenames(filenames);

  getApp()->stopModal(this, TRUE);
  hide();
  return 1;
}


#if 0
long
MultiGameFileSelector::onClose(FXObject *, FXSelector, void *)
{
  getApp()->stopModal(this, FALSE);
  hide();
  return 0;
}
#endif
