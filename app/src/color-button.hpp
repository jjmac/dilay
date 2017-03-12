/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_COLOR_BUTTON
#define DILAY_COLOR_BUTTON

#include <QPushButton>
#include "dilay/color.hpp"

class Color;

class ViewColorButton : public QPushButton
{
  Q_OBJECT

  public:
    ViewColorButton(const Color&);

  signals:
    void colorChanged (const Color&);

  protected:
    void paintEvent (QPaintEvent*);

  private:
    Color            m_color;
};

#endif
