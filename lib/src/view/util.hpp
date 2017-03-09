/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef VIEW_UTIL
#define VIEW_UTIL

#include <functional>
#include <glm/fwd.hpp>

class ViewDoubleSlider;
class QAbstractSpinBox;
class QButtonGroup;
class QCheckBox;
class QAbstractButton;
class QDoubleSpinBox;
class QFrame;
class QLineEdit;
class QPoint;
class QPushButton;
class QRadioButton;
class QSlider;
class QSpinBox;
class QString;
class QToolButton;
class QGroupButton;

namespace ViewUtil {
  QSpinBox&         spinBox        (int, int, int, int = 1);
  QDoubleSpinBox&   spinBox        (float, float, float, float);
  QPushButton&      pushButton     (const QString&, const QString&, bool = false);
  QToolButton&      toolButton     (const QString&);
  QRadioButton&     radioButton    (const QString&, bool = false);
  QCheckBox&        checkBox       (const QString&, bool = false);
  QCheckBox&        checkBox       (const QString& name, const QString&, bool = false);
  QSlider&          slider         (int, int, int);
  ViewDoubleSlider& slider         (unsigned short, float, float, float, unsigned short = 1);
  QGroupButton&     contraint      (int = 6);
  QFrame&           horizontalLine ();
  QWidget&          emptyWidget    ();
  QLineEdit&        lineEdit       (float, unsigned short = 2);
  QLineEdit&        lineEdit       (float, float, float, unsigned short = 2);
  QLineEdit&        lineEdit       (int);
  QLineEdit&        lineEdit       (int, int, int);
  glm::uvec2        toUVec2        (const QPoint&);
  glm::ivec2        toIVec2        (const QPoint&);
  void              connect        (const QSpinBox&, const std::function <void (int)>&);
  void              connect        (const QDoubleSpinBox&, const std::function <void (double)>&);
  void              connect        (const QPushButton&, const std::function <void ()>&);
  void              connect        (const QToolButton&, const std::function <void ()>&);
  void              connect        (const QButtonGroup&, const std::function <void (int)>&);
  void              connect        (const QCheckBox&, const std::function <void (bool)>&);
  void              connect        (const QRadioButton&, const std::function <void (bool)>&);
  void              connect        (const QSlider&, const std::function <void (int)>&);
  void              connect        (const ViewDoubleSlider&, const std::function <void (float)>&);
  void              connectFloat   (const QLineEdit&, const std::function <void (float)>&);
  void              connectInt     (const QLineEdit&, const std::function <void (int)>&);

  void              connectCheck   (const QAbstractButton&, const std::function <void (bool)>&);
  void              connectRelease   (const QAbstractButton&, const std::function <void ()>&);
  QWidget&          stretcher      (bool, bool);
  void              deselect       (QAbstractSpinBox&);
  bool              question       (QWidget&, const QString&);
  void              error          (QWidget&, const QString&);
  void              about          (QWidget&, const QString&);
  void              info           (QWidget&, const QString&);
};

#endif
