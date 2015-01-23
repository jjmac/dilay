#include <QCheckBox>
#include <QDoubleSpinBox>
#include "color.hpp"
#include "config.hpp"
#include "sculpt-brush.hpp"
#include "tool/sculpt/behavior.hpp"
#include "view/cursor.hpp"
#include "view/properties.hpp"
#include "view/tool/tip.hpp"
#include "view/util.hpp"

struct ToolSculptBehavior::Impl {
  ToolSculptBehavior* self;
  ConfigProxy&        config;
  ViewCursor          cursor;
  QDoubleSpinBox&     radiusEdit;

  Impl (ToolSculptBehavior* s, ConfigProxy& c) 
    : self       (s)
    , config     (c)
    , cursor     (1.0f, Color::red ())
    , radiusEdit (ViewUtil::spinBox (0.01f, 1.0f, 1000.0f, 1.0f))
  {}

  void setupBrush () {
    SculptBrush& brush = this->self->brush ();

    brush.radius          (this->self->config ().get <float> ("radius"           , 10.0f));
    brush.detailFactor    (this->self->config ().get <float> ("detail-factor"    ,  0.6f));
    brush.stepWidthFactor (this->self->config ().get <float> ("step-width-factor",  0.3f));
    brush.subdivide       (this->self->config ().get <bool>  ("subdivide"        ,  true));

    this->cursor.radius         (brush.radius ());
    this->cursor.color          (this->self->config ().get <Color> ("cursor-color", Color::red ()));
    this->cursor.updateGeometry ();

    this->self->runSetupBrush ();
  }

  void setupProperties (ViewProperties& properties) {
    SculptBrush& brush = this->self->brush ();

    this->radiusEdit.setValue (brush.radius ());
    ViewUtil::connect (this->radiusEdit, [this] (float r) {
      this->self->brush ().radius (r);
      this->config.cache ("radius", r);
    });
    properties.addWidget (QObject::tr ("Radius"), this->radiusEdit);

    QDoubleSpinBox& detailEdit = ViewUtil::spinBox (0.01f, brush.detailFactor (), 0.95f, 0.1f);
    ViewUtil::connect (detailEdit, [this] (float h) {
      this->self->brush ().detailFactor (h);
      this->config.cache ("detail-factor", h);
    });
    properties.addWidget (QObject::tr ("Detail"), detailEdit);

    QDoubleSpinBox& stepEdit = ViewUtil::spinBox (0.01f, brush.stepWidthFactor (), 1000.0f, 0.1f);
    ViewUtil::connect (stepEdit, [this] (float s) {
      this->self->brush ().stepWidthFactor (s);
      this->config.cache ("step-width-factor", s);
    });
    properties.addWidget (QObject::tr ("Step width"), stepEdit);

    QCheckBox& subdivEdit = ViewUtil::checkBox (QObject::tr ("Subdivide"), brush.subdivide ());
    QObject::connect (&subdivEdit, &QCheckBox::stateChanged, [this] (int s) {
      this->self->brush ().subdivide (bool (s));
      this->config.cache ("subdivide", bool (s));
    });
    properties.addWidget (subdivEdit);

    this->self->runSetupProperties (properties);
  }

  void setupToolTip (ViewToolTip& toolTip) {
    this->self->runSetupToolTip (toolTip);
    toolTip.add ( ViewToolTip::MouseEvent::Wheel, ViewToolTip::Modifier::Shift
                , QObject::tr ("Change radius") );
  }

  bool update (State& state, const glm::ivec2& pos, bool updateBrush) {
    return this->self->runUpdate (state, pos, updateBrush);
  }
};

DELEGATE1_BIG3_SELF (ToolSculptBehavior, ConfigProxy&)

GETTER_CONST (ViewCursor&    , ToolSculptBehavior, cursor)
GETTER_CONST (QDoubleSpinBox&, ToolSculptBehavior, radiusEdit)
GETTER_CONST (ConfigProxy&   , ToolSculptBehavior, config)
DELEGATE     (void           , ToolSculptBehavior, setupBrush)
DELEGATE1    (void           , ToolSculptBehavior, setupProperties, ViewProperties&)
DELEGATE1    (void           , ToolSculptBehavior, setupToolTip, ViewToolTip&)
DELEGATE3    (bool           , ToolSculptBehavior, update, State&, const glm::ivec2&, bool)