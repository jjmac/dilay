/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QObject>
#include "tool-tip.hpp"
#include <algorithm>


ViewToolTip::ViewToolTip()
{}

QString ViewToolTip::toString () const {
    QString            result;
    const unsigned int numSpaces = 5;
    Tips               sorted (this->tips);

    std::stable_sort (sorted.begin (), sorted.end (), [] (const Tip& a, const Tip& b)->bool {
        return static_cast <int> (std::get <0> (a)) < static_cast <int> (std::get <0> (b));
    });
    for (auto& t : sorted) {
      result.append (tipToString (t)).append (QString (" ").repeated (numSpaces));;
    }
    result.remove (result.size () - numSpaces, numSpaces);
    return result;
}

void ViewToolTip::add (MouseEvent event, Modifier modifier, const QString& tip) {
    this->tips.push_back (std::make_tuple (event, modifier, tip));
}

void ViewToolTip::add (MouseEvent event, const QString& tip) {
    this->add (event, ViewToolTip::Modifier::None, tip);
}

void ViewToolTip::reset () {
    this->tips.clear ();
}

QString ViewToolTip::tipToString (const Tip& tip) const {
  QString string ("[");

  switch (std::get <1> (tip)) {
    case ViewToolTip::Modifier::None:
      break;

    case ViewToolTip::Modifier::Ctrl:
      string.append (QObject::tr ("Ctrl"));
      string.append ("+");
      break;

    case ViewToolTip::Modifier::Shift:
      string.append (QObject::tr ("Shift"));
      string.append ("+");
      break;

    case ViewToolTip::Modifier::Alt:
      string.append (QObject::tr ("Alt"));
      string.append ("+");
      break;
  }
  switch (std::get <0> (tip)) {
    case ViewToolTip::MouseEvent::Left:
      string.append (QObject::tr ("Left"));
      break;

    case ViewToolTip::MouseEvent::Middle:
      string.append (QObject::tr ("Middle"));
      break;

    case ViewToolTip::MouseEvent::Wheel:
      string.append (QObject::tr ("Wheel"));
      break;

    case ViewToolTip::MouseEvent::Right:
      string.append (QObject::tr ("Right"));
      break;
  }

  string.append ("] ");
  string.append (std::get <2> (tip));
  return string;
}
