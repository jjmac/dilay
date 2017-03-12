/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_TOOL_TIP
#define DILAY_VIEW_TOOL_TIP

#include <vector>


class QString;

class ViewToolTip {
  public:
    ViewToolTip();

    enum class MouseEvent { Left, Middle, Wheel, Right };
    enum class Modifier   { None, Ctrl, Shift, Alt };

    QString toString () const;

    void add   (MouseEvent, Modifier, const QString&);
    void add   (MouseEvent, const QString&);
    void reset ();

private:
    typedef std::tuple <ViewToolTip::MouseEvent, ViewToolTip::Modifier, QString> Tip;
    QString tipToString (const Tip& tip) const ;

private:
    typedef std::vector <Tip> Tips;
    Tips tips;

};

#endif
