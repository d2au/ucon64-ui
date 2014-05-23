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
#include <time.h>
#ifndef __unix__
#include <sys/timeb.h>
#else
#include <sys/time.h>
#endif
#include <fx.h>
#include "ProgressDialog.h"


FXDEFMAP(ProgressDialog) ProgressDialogMap[] =
{
  FXMAPFUNC(SEL_COMMAND, ProgressDialog::ID_CANCEL, ProgressDialog::onCancel)
};

FXIMPLEMENT(ProgressDialog, FXDialogBox, ProgressDialogMap, ARRAYNUMBER(ProgressDialogMap))


ProgressDialog::ProgressDialog(FXWindow* a_owner, const FXString& a_caption,
    FXuint a_opts, FXint a_x, FXint a_y, FXint a_w, FXint a_h)
  : FXDialogBox(a_owner, a_caption, a_opts, a_x, a_y, a_w, a_h)
{
  FXVerticalFrame *windowArea = new FXVerticalFrame(this, 0,
    0, 0, 0, 0, 0, 0, 0, 0);

  FXHorizontalFrame *progressArea = new FXHorizontalFrame(windowArea, 0,
    0, 0, 0, 0, 0, 0, 0, 0);
  m_progressBar = new FXProgressBar(progressArea, NULL, ID_DIALOG,
    PROGRESSBAR_PERCENTAGE | LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT, 0, 0, 200,
    20, 0, 0, 0, 0);
  m_progressBar->setProgress(0);

  m_timeInfo = new FXLabel(progressArea, "ETA=?", NULL,
    LABEL_NORMAL | LAYOUT_FIX_WIDTH, 0, 0, 65, 0);
  m_timeInfo->setJustify(JUSTIFY_LEFT);

  new FXButton(windowArea, "&Cancel", NULL, this, ID_CANCEL,
    BUTTON_NORMAL | LAYOUT_BOTTOM | LAYOUT_CENTER_X);

  m_cancelled = false;
  m_startTimeSet = false;
}


ProgressDialog::~ProgressDialog()
{
}


void
ProgressDialog::setProgress(FXuint a_value)
// This function assumes the range of a_value is [0, 100]
{
#if 1
  static time_t t0;
  if (!m_startTimeSet)
    t0 = time(NULL);
  int nSecondsPassed = time(NULL) - t0;
#else
#ifndef __unix__
  static struct timeb t0, t1;
  if (!m_startTimeSet)
    ftime(&t0);
  ftime(&t1);
  int nSecondsPassed = ((t1.time * 1000 + t1.millitm) -
                        (t0.time * 1000 + t0.millitm) + 500) / 1000;
#else
  static struct timeval t0, t1;
  if (!m_startTimeSet)
    gettimeofday(&t0, NULL);
  gettimeofday(&t1, NULL);
  int nSecondsPassed = ((t1.tv_sec * 1000000 + t1.tv_usec) -
                        (t0.tv_sec * 1000000 + t0.tv_usec) + 500000) / 1000000;
#endif
#endif
  m_startTimeSet = true;

  m_progressBar->setProgress(a_value);
  if (nSecondsPassed == 0)
    return;
  if (a_value == 0)
    return;

  unsigned int todo = 100 - a_value;
  unsigned int nSecondsLeft =
    (unsigned int) (todo / ((float) a_value / nSecondsPassed) + 0.5f);
  if (nSecondsLeft > 99 * 60 + 59)              // just in case :-)
    nSecondsLeft = 99 * 60 + 59;

  FXString timeString = FXString::value("ETA=%02d:%02d",
    nSecondsLeft / 60, nSecondsLeft % 60);
  m_timeInfo->setText(timeString);
}


void
ProgressDialog::reset(void)
{
  m_progressBar->setProgress(0);
  m_timeInfo->setText("ETA=?");
  m_cancelled = false;
  m_startTimeSet = false;
}


long
ProgressDialog::onCancel(FXObject *, FXSelector, void *)
{
  m_cancelled = true;
  hide();
  return 1;
}
