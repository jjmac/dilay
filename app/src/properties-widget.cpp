#include "properties-widget.hpp"
#include "view/util.hpp"
#include <QCheckBox>
#include <QAbstractButton>
#include "view/double-slider.hpp"
#include "cache.hpp"
#include "sculpt-brush.hpp"

#include <QGridLayout>

PropertiesWidget::PropertiesWidget()
    : QWidget()
    , m_properties(nullptr)
{
    setLayout(new QGridLayout(this));
}

void PropertiesWidget::setView(ViewProperties* properties)
{
    setLayout(new QGridLayout(this));
    layout()->addWidget(properties);
    m_properties.reset(properties);
}

void PropertiesWidget::updateImpl(ToolMoveMesh& tool, ViewProperties& propertiesView)
{
}

void PropertiesWidget::updateImpl(ToolDeleteMesh& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolNewMesh& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculptCarve  & tool, ViewProperties& propertiesView)
{
    ViewTwoColumnGrid properties;

    ViewDoubleSlider& intensityEdit = ViewUtil::slider (3, 0.0f, tool.intensity (), 0.05f);
    ViewUtil::connect (intensityEdit, [&tool] (float i) {
        tool.intensity(i);
    });
    properties.addStacked (QObject::tr ("Intensity"), intensityEdit);
//    tool.registerSecondarySlider (intensityEdit);

    QAbstractButton& invertEdit = ViewUtil::checkBox ("tool_sculpt_property_invert", QObject::tr ("Invert"), tool.invert ());
    ViewUtil::connectCheck (invertEdit, [&tool] (bool i) {
      tool.invert (i);
    });
    properties.add (invertEdit);

    QAbstractButton& inflateEdit = ViewUtil::checkBox ("tool_sculpt_property_inflate", QObject::tr ("Inflate"), tool.inflate ());
    ViewUtil::connectCheck (inflateEdit, [&tool] (bool i) {
      tool.inflate (i);
    });
    properties.add (inflateEdit);

}

void PropertiesWidget::updateImpl(ToolSculptDrag   & tool, ViewProperties& propertiesView)
{
    ViewTwoColumnGrid properties;

    ViewDoubleSlider& smoothnessEdit = ViewUtil::slider (2, 0.0f, tool.smoothness (), 1.0f);
    ViewUtil::connect (smoothnessEdit, [&tool] (float f) {
      tool.smoothness (f);
    });
    properties.addStacked (QObject::tr ("Smoothness"), smoothnessEdit);
//    this->self->registerSecondarySlider (smoothnessEdit);

    QCheckBox& primPlaneEdit = ViewUtil::checkBox
      ( QObject::tr ("Along primary plane")
      , tool.constraint () == MovementConstraint::PrimaryPlane
      );
    ViewUtil::connect (primPlaneEdit, [&tool] (bool p) {
      tool.constraint ( p ? MovementConstraint::PrimaryPlane
                                    : MovementConstraint::CameraPlane );
    });
    properties.add (primPlaneEdit);

    QCheckBox& discardEdit = ViewUtil::checkBox ( QObject::tr ("Discard backfaces")
                                                , tool.discardBackfaces () );
    ViewUtil::connect (discardEdit, [&tool] (bool d) {
      tool.discardBackfaces (d);
    });
    properties.add (discardEdit);

}

void PropertiesWidget::updateImpl(ToolSculptGrab   & tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculptSmooth & tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculptFlatten& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculptCrease & tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculptPinch  & tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculptReduce & tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolNewSketch& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolModifySketch& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolDeleteSketch& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolRebalanceSketch& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolConvertSketch & tool, ViewProperties& propertiesView)
{
    ViewTwoColumnGrid& properties = propertiesView.body ();

    ViewDoubleSlider& resolutionEdit = ViewUtil::slider (2, tool.getMinResolution()
                                                          , tool.getResolution()
                                                          , tool.getMaxResolution());
    ViewUtil::connect (resolutionEdit, [&tool] (float r) {
      tool.setResolution(r);
    });
    properties.addStacked (QObject::tr ("Resolution"), resolutionEdit);

    QCheckBox& moveToCenterEdit = ViewUtil::checkBox ( QObject::tr ("Move to center")
                                                     , tool.getMoveToCenter() );
    ViewUtil::connect (moveToCenterEdit, [&tool] (bool m) {
      tool.setMoveToCenter(m);
    });
    properties.add (moveToCenterEdit);

    QCheckBox& smoothMeshEdit = ViewUtil::checkBox ( QObject::tr ("Smooth mesh")
                                                   , tool.getSmoothMesh() );
    ViewUtil::connect (smoothMeshEdit, [&tool] (bool s) {
      tool.setSmoothMesh(s);
    });
    properties.add (smoothMeshEdit);

}

void PropertiesWidget::updateImpl(ToolSketchSpheres& tool, ViewProperties& propertiesView)
{

}
