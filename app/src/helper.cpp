#include "helper.hpp"

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
