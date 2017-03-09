/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QColorDialog>
#include <QPainter>
#include "color.hpp"
#include "view/helper.hpp"
#include "color-button.hpp"
#include "util.hpp"

ViewColorButton::ViewColorButton(const Color& c)
    : m_color (c)
{
    connect (this, &QPushButton::released, [this] () {
        QColorDialog dialog (toQt(this->m_color), this->parentWidget ());
        if (dialog.exec () == QDialog::Accepted) {
            this->m_color = toGL(dialog.selectedColor ());
            this->update ();
            emit this->colorChanged (this->m_color);
        }
    });
}

void ViewColorButton::paintEvent (QPaintEvent* event)
{
    QPushButton::paintEvent (event);

    QPainter painter (this);
    QRect    rect    (rect ());

    const int dw = width  () / 10;
    const int dh = height () / 5;

    rect.setLeft   (rect.left   () + dw);
    rect.setRight  (rect.right  () - dw);
    rect.setTop    (rect.top    () + dh);
    rect.setBottom (rect.bottom () - dh);

    painter.fillRect (rect, toQt(m_color));
}

