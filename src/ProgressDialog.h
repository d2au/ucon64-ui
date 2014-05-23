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
#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <time.h>
#ifndef __unix__
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <fx.h>


class ProgressDialog : public FXDialogBox
{
  FXDECLARE(ProgressDialog)
public:
  ProgressDialog(FXWindow* owner, const FXString& caption,
      FXuint opts = DECOR_TITLE | DECOR_BORDER, // default for FXDialogBox
      FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
  virtual ~ProgressDialog();
  void setProgress(FXuint value);
  long onCancel(FXObject *src, FXSelector sel, void *data);
  bool cancelled(void) { return m_cancelled; }
  void reset(void);

  enum
    {
      ID_DIALOG = FXDialogBox::ID_LAST,
      ID_CANCEL,
      ID_LAST
    };
protected:
  ProgressDialog() {}

  FXProgressBar *m_progressBar;
  FXLabel *m_timeInfo;
  bool m_cancelled;
  bool m_startTimeSet;
};

#endif
