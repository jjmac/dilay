/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_DOUBLE_SLIDER
#define DILAY_DOUBLE_SLIDER

#include <QSlider>

class ViewDoubleSlider : public QSlider {
  Q_OBJECT
public:
    ViewDoubleSlider(unsigned short, unsigned short, QWidget* parent = nullptr);

    double doubleValue      () const;
    double doubleSingleStep () const;

    void setDoubleValue      (double);
    void setDoubleRange      (double, double);
    void setDoubleSingleStep (double);
    void setDoublePageStep   (double);

    int intValue      () const;
    int intSingleStep () const;

    void setIntValue      (int);
    void setIntRange      (int, int);
    void setIntSingleStep (int);
    void setIntPageStep   (int);

  signals:
    void doubleValueChanged (double);

  private:
    int value      () const;
    int singleStep () const;

    void setValue      (int);
    void setRange      (int, int);
    void setSingleStep (int);
    void setPageStep   (int);

    double toDouble (int value, bool forceLinear) const;
    int toInt (double value, bool forceLinear) const;

private:
    const unsigned int   m_factor;
    const unsigned short m_order;

};

#endif
