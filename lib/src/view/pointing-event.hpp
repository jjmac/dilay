/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_POINTING_EVENT
#define DILAY_VIEW_POINTING_EVENT

#include <glm/glm.hpp>

enum class KeyboardModifiers {
    NoModifier           = 0x00000000,
    ShiftModifier        = 0x02000000,
    ControlModifier      = 0x04000000,
    AltModifier          = 0x08000000,
    MetaModifier         = 0x10000000,
    KeypadModifier       = 0x20000000,
    GroupSwitchModifier  = 0x40000000,
    // Do not extend the mask to include 0x01000000
    KeyboardModifierMask = 0xfe000000
};

class ViewPointingEvent {
  public:
    ViewPointingEvent(KeyboardModifiers     _modifiers,
                      bool                  _pressEvent,
                      bool                  _moveEvent,
                      bool                  _releaseEvent,
                      bool                  _primaryButton,
                      bool                  _secondaryButton,
                      glm::ivec2            _ivec2,
                      float                 _intensity)
        : _modifiers(_modifiers)
        , _pressEvent(_pressEvent)
        , _moveEvent(_moveEvent)
        , _releaseEvent(_releaseEvent)
        , _primaryButton(_primaryButton)
        , _secondaryButton(_secondaryButton)
        , _ivec2(_ivec2)
        , _intensity(_intensity)
    {}

    bool                  valid           () const { return this->pressEvent () || this->moveEvent () || this->releaseEvent (); }
    KeyboardModifiers     modifiers       () const { return this->_modifiers; }
    bool                  pressEvent      () const { return this->_pressEvent; }
    bool                  moveEvent       () const { return this->_moveEvent; }
    bool                  releaseEvent    () const { return this->_releaseEvent; }
    bool                  primaryButton   () const { return this->_primaryButton; }
    bool                  secondaryButton () const { return this->_secondaryButton; }
    const glm::ivec2&     ivec2           () const { return this->_ivec2; }
    float                 intensity       () const { return this->_intensity; }

  private:
    const  KeyboardModifiers    _modifiers;
    const bool                  _pressEvent;
    const bool                  _moveEvent;
    const bool                  _releaseEvent;
    const bool                  _primaryButton;
    const bool                  _secondaryButton;
    const glm::ivec2            _ivec2;
    const float                 _intensity;
};

class ViewWheelEvent {
public:
  ViewWheelEvent(KeyboardModifiers     _modifiers,
				 bool _vertical,
				 int _delta)
	  : _modifiers(_modifiers)
	  , _vertical(_vertical)
	  , _delta(_delta)
	  {}

  KeyboardModifiers     modifiers       () const { return this->_modifiers; }
  bool                  isVertical      () const { return this->_vertical; }
  int                   delta           () const { return this->_delta; }

private:
	const KeyboardModifiers	_modifiers;
	const bool				_vertical;
	const int				_delta;
};

#endif
