#ifndef DILAY_VIEW_HELPER
#define DILAY_VIEW_HELPER

#include <QColor>
#include "color.hpp"
QColor toQt(const Color& color);
Color toGL(const QColor& color);


#endif // DILAY_VIEW_HELPER