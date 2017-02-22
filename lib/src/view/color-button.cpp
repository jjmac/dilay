/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QColorDialog>
#include <QPainter>
#include "color.hpp"
#include "helper.hpp"
#include "view/color-button.hpp"
#include "view/util.hpp"

struct ViewColorButton::Impl {
  ViewColorButton* self;
  Color            color;

  Impl (ViewColorButton* s, const Color& c)
    : self  (s)
    , color (c)
  {
    ViewUtil::connect (*this->self, [this] () {
      QColorDialog dialog (toQt(this->color), this->self->parentWidget ());
      if (dialog.exec () == QDialog::Accepted) {
        this->color = toGL(dialog.selectedColor ());
        this->self->update ();
        emit this->self->colorChanged (this->color);
      }
    });
  }

  void paintEvent (QPaintEvent* event) {
    this->self->QPushButton::paintEvent (event);

    QPainter painter (this->self);
    QRect    rect    (this->self->rect ());

    const int dw = this->self->width  () / 10;
    const int dh = this->self->height () / 5;

    rect.setLeft   (rect.left   () + dw);
    rect.setRight  (rect.right  () - dw);
    rect.setTop    (rect.top    () + dh);
    rect.setBottom (rect.bottom () - dh);

    painter.fillRect (rect, toQt(this->color));
  }
};

DELEGATE1_BIG2_SELF (ViewColorButton, const Color&)
DELEGATE1 (void, ViewColorButton, paintEvent, QPaintEvent*)
