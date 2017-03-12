/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QColorDialog>
#include <QPainter>
#include "color-button.hpp"

namespace {
QColor toQt(const Color& color)
{
    return QColor( qMin (255, int (255.0f * color.r())),
                   qMin (255, int (255.0f * color.g())),
                   qMin (255, int (255.0f * color.b())),
                   qMin (255, int (255.0f * color.opacity()))
                    );
}
Color toGL(const QColor& color)
{
    return Color (color.redF (), color.greenF (), color.blueF ());
}
}


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

