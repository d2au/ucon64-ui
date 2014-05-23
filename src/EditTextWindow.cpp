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
#include <string.h>
#include <fstream>
#include <fx.h>
#include "EditTextWindow.h"


using namespace std;


FXDEFMAP(EditTextWindow) EditTextWindowMap[] =
{
  FXMAPFUNC(SEL_COMMAND, EditTextWindow::ID_SAVE, EditTextWindow::onSave),
  FXMAPFUNC(SEL_CLOSE, 0, EditTextWindow::onClose) // Why doesn't EditTextWindow::ID_CLOSE work?
};

FXIMPLEMENT(EditTextWindow, FXTopWindow, EditTextWindowMap, ARRAYNUMBER(EditTextWindowMap))


EditTextWindow::EditTextWindow(FXWindow *a_owner, const FXString &a_name,
    char *a_filename, FXIcon *a_icon, FXIcon *a_icon2, FXuint a_opts,
    FXint a_x, FXint a_y, FXint a_w, FXint a_h, FXint a_pl, FXint a_pr,
    FXint a_pt, FXint a_pb, FXint a_hs, FXint a_vs)
  : FXTopWindow(a_owner, a_name, a_icon, a_icon2, a_opts, a_x, a_y, a_w, a_h,
        a_pl, a_pr, a_pt, a_pb, a_hs, a_vs)
{
  strcpy(m_filename, a_filename);

  setWidth(500);
  setHeight(400);

  FXVerticalFrame *windowArea = new FXVerticalFrame(this, LAYOUT_FILL,
    0, 0, 0, 0, 0, 0, 0, DEFAULT_PAD);//, 0, 0);
  m_contents = new FXText(windowArea, NULL, 0, LAYOUT_FILL);

  new FXButton(windowArea, "Save", NULL, this, ID_SAVE,
    BUTTON_NORMAL | LAYOUT_CENTER_X, 0, 0, 0, 0, 10, 10);

  FXStat::mode(m_filename, 0644);               // rw-r--r--
  int size = FXStat::size(m_filename);
  char *configFileContents = new char[size + 1];
  ifstream configFile(m_filename, ios::in);
  // NOT "| ios::binary", because under Windows it's a DOS text file
  if (!configFile)
    {
      FXMessageBox::error(a_owner, MBOX_OK, "Could not open configuration file",
        "Please specify a correct configuration file directory");
      return;
    }
  configFile.read(configFileContents, size);
  configFile.close();
  configFileContents[configFile.gcount()] = 0;
  // in text mode fstream::gcount() returns the number of "interpreted" bytes
  //  (\r is not counted)
  m_contents->setText(configFileContents);
  delete configFileContents;
}


EditTextWindow::~EditTextWindow()
{
  delete m_contents;
}


long
EditTextWindow::onSave(FXObject *, FXSelector, void *)
{
  FXStat::mode(m_filename, 0644);               // rw-r--r--
  ofstream configFile(m_filename, ios::out);
  if (!configFile)
    {
      FXMessageBox::error(this, MBOX_OK, "Could not write to configuration file",
        "Please specify a correct configuration file directory");
      /*
        Close immediately after the user presses OK in order to avoid confusion
        (he could think the save will work if he changes the configuration file
        directory).
      */
      close();
      return 1;
    }
  configFile.write(m_contents->getText().text(), m_contents->getLength());
  configFile.close();
  FXMessageBox::information(this, MBOX_OK, "Saved configuration file",
    "Wrote changes to configuration file");
  m_contents->setModified(FALSE);
  return 1;
}


long
EditTextWindow::onClose(FXObject *, FXSelector, void *)
{
  if (m_contents->isModified())
    if (FXMessageBox::warning(this, MBOX_YES_NO, "Changes have not been saved",
          "Are you sure you want to close the window?") == MBOX_CLICKED_NO)
      return 1;
    else
      getOwner()->setFocus();
  close();
  return 0;
}
