/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <glm/glm.hpp>
#include "double-slider.hpp"

ViewDoubleSlider::ViewDoubleSlider (unsigned short numDecimals, unsigned short o, QWidget* parent)
    : QSlider   (parent)
    , m_factor (glm::pow (10, numDecimals))
    , m_order  (o)
{
    assert (m_order > 0);

    connect (this, &QSlider::valueChanged, [this] (int i) {
      emit doubleValueChanged (this->toDouble (i, false));
    });
}
  
double ViewDoubleSlider::doubleValue () const {
    return this->toDouble (this->value (), false);
}

double ViewDoubleSlider::doubleSingleStep () const {
    return this->toDouble (this->singleStep (), true);
}

void ViewDoubleSlider::setDoubleValue (double v) {
    this->setValue (this->toInt (v, false));
}

void ViewDoubleSlider::setDoubleRange (double min, double max) {
    this->setRange (this->toInt (min, true), this->toInt (max, true));
}

void ViewDoubleSlider::setDoubleSingleStep (double v) {
    this->setSingleStep (this->toInt (v, true));
}

void ViewDoubleSlider::setDoublePageStep (double v) {
    this->setPageStep (this->toInt (v, true));
}

int ViewDoubleSlider::intValue () const {
    return QSlider::value ();
}

int ViewDoubleSlider::intSingleStep () const {
    return QSlider::singleStep ();
}

void ViewDoubleSlider::setIntValue (int v) {
    this->QSlider::setValue (v);
}

void ViewDoubleSlider::setIntRange (int min, int max) {
    this->QSlider::setRange (min, max);
}

void ViewDoubleSlider::setIntSingleStep (int v) {
    this->QSlider::setSingleStep (v);
}

void ViewDoubleSlider::setIntPageStep (int v) {
    this->QSlider::setPageStep (v);
}

int ViewDoubleSlider::value () const {
    return this->intValue ();
}

int ViewDoubleSlider::singleStep () const {
    return this->intSingleStep ();
}

void ViewDoubleSlider::setValue (int v) {
    this->setIntValue (v);
}

void ViewDoubleSlider::setRange (int min, int max) {
    this->setIntRange (min, max);
}

void ViewDoubleSlider::setSingleStep (int v) {
    this->setIntSingleStep (v);
}

void ViewDoubleSlider::setPageStep (int v) {
    this->setIntPageStep (v);
}

double ViewDoubleSlider::toDouble (int value, bool forceLinear) const {
    const unsigned short o = forceLinear ? 1 : m_order;

    if (o == 1) {
      return double (value) / m_factor;
    }
    else {
      const double min    = double (this->minimum ()) / m_factor;
      const double max    = double (this->maximum ()) / m_factor;
      const double d      = double (value) / m_factor;
      const double norm   = (d - min) / (max - min);
      const double result = min + (glm::pow (norm, float (o)) * (max - min));

      return result;
    }
}

int ViewDoubleSlider::toInt (double value, bool forceLinear) const {
    const unsigned short o = forceLinear ? 1 : m_order;

    if (o == 1) {
      return int (std::round (value * m_factor));
    }
    else {
      const double min   = double (this->minimum ()) / m_factor;
      const double max   = double (this->maximum ()) / m_factor;
      const double norm  = (value - min) / (max - min);
      const double slope = norm <= .0001f ? 0.0f : glm::pow (norm, 1.0f / float (o));

      return int (std::round (m_factor * (min + (slope * (max - min)))));
    }
}

