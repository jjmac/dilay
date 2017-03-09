#include "properties-widget.hpp"
#include "view-util.hpp"
#include <QCheckBox>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QRadioButton>
#include "double-slider.hpp"
#include "cache.hpp"
#include "sculpt-brush.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

static QLabel* lapelPrt = nullptr;
static int labelI = 0;
PropertiesWidget::PropertiesWidget()
    : QWidget()
    , m_properties(nullptr)
{
    setLayout(new QGridLayout(this));


	setMinimumWidth(200);

	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(128, 128, 128));
	setAutoFillBackground(true);
	setPalette(pal);
}

void PropertiesWidget::reset()
{
	m_properties.reset();
}

void PropertiesWidget::setView(ViewProperties* properties)
{
	m_properties.reset(properties);
	layout()->addWidget(properties);
}


void PropertiesWidget::updateImpl(ToolMoveMesh& tool, ViewProperties& propertiesView)
{
	ViewTwoColumnGrid& properties = propertiesView.body ();
    QButtonGroup& constraint = ViewUtil::contraint((int)tool.constraint());
	for (auto button: constraint.buttons())
	{
		properties.add(*button);
	}
	ViewUtil::connect(constraint, [&tool] (int r) {
        tool.constraint(MovementConstraint(r));
	});
}

void PropertiesWidget::updateImpl(ToolDeleteMesh& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolNewMesh& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolSculpt& tool, ViewProperties& propertiesView)
{
	ViewTwoColumnGrid& properties = propertiesView.body ();
	ViewDoubleSlider& radiusEdit = ViewUtil::slider  (2, 0.01f
										   , tool.radius()
										   , 1.0f, 3);
	ViewUtil::connect (radiusEdit, [&tool] (float r) {
		tool.radius(r);
	});
	properties.addStacked (QObject::tr ("Radius"), radiusEdit);

	QAbstractButton& absRadiusEdit = ViewUtil::checkBox ( "tool_sculpt_property_absradius", QObject::tr ("Absolute radius")
												  , tool.absoluteRadius() );
	ViewUtil::connectCheck (absRadiusEdit, [&tool] (bool a) {
		tool.absoluteRadius(a);
	});
	properties.add (absRadiusEdit);

	QAbstractButton& subdivEdit = ViewUtil::checkBox ( "tool_sculpt_property_subdivide", QObject::tr ("Subdivide")
											   , tool.subdivide () );
	ViewUtil::connectCheck (subdivEdit, [&tool] (bool s) {
		tool.subdivide(s);
	});
	properties.add (subdivEdit);

	QPushButton& syncButton = ViewUtil::pushButton ("tool_sculpt_property_syncmirror", QObject::tr ("Sync"));
	ViewUtil::connect (syncButton, [&tool] () {
	  tool.syncMirror();
	});
	syncButton.setEnabled (tool.hasMirror ());

	QAbstractButton& mirrorEdit = ViewUtil::checkBox ( "tool_sculpt_property_mirror", QObject::tr ("Mirror")
											   , tool.hasMirror () );
	ViewUtil::connectCheck (mirrorEdit, [&tool,&syncButton] (bool m) {
	  tool.mirror (m);
	  syncButton.setEnabled (m);
	});

	properties.add (mirrorEdit, syncButton);

	properties.add (ViewUtil::horizontalLine ());
}

void PropertiesWidget::updateImpl(ToolSculptCarve  & tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

    ViewDoubleSlider& intensityEdit = ViewUtil::slider (3, 0.0f, tool.intensity (), 0.05f);
    ViewUtil::connect (intensityEdit, [&tool] (float i) {
        tool.intensity(i);
    });
    properties.addStacked (QObject::tr ("Intensity"), intensityEdit);

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

void PropertiesWidget::updateImpl(ToolSculptDrag & tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

    ViewDoubleSlider& smoothnessEdit = ViewUtil::slider (2, 0.0f, tool.smoothness (), 1.0f);
    ViewUtil::connect (smoothnessEdit, [&tool] (float f) {
      tool.smoothness (f);
    });
    properties.addStacked (QObject::tr ("Smoothness"), smoothnessEdit);

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
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

    QCheckBox& primPlaneEdit = ViewUtil::checkBox ( QObject::tr ("Along primary plane")
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

void PropertiesWidget::updateImpl(ToolSculptSmooth & tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

	ViewDoubleSlider& intensityEdit = ViewUtil::slider (2, 0.1f, tool.intensity (), 1.0f);
	ViewUtil::connect (intensityEdit, [&tool] (float i) {
	  tool.intensity (i);
	});
	intensityEdit.setEnabled (!tool.relaxOnly ());
	properties.addStacked (QObject::tr ("Intensity"), intensityEdit);

	QCheckBox& relaxEdit = ViewUtil::checkBox (QObject::tr ("Relax only"), tool.relaxOnly ());
	ViewUtil::connect (relaxEdit, [&tool,&intensityEdit] (bool r) {
	  tool.relaxOnly (r);
	  intensityEdit.setEnabled (!r);
	});
	properties.add (relaxEdit);
}

void PropertiesWidget::updateImpl(ToolSculptFlatten& tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

	ViewDoubleSlider& intensityEdit = ViewUtil::slider (2, 0.1f, tool.intensity (), 1.0f);
	ViewUtil::connect (intensityEdit, [&tool] (float i) {
	  tool.intensity (i);
	});
	properties.addStacked (QObject::tr ("Intensity"), intensityEdit);
}

void PropertiesWidget::updateImpl(ToolSculptCrease & tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

	ViewDoubleSlider& intensityEdit = ViewUtil::slider (2, 0.1f, tool.intensity (), 0.9f);
	ViewUtil::connect (intensityEdit, [&tool] (float i) {
	  tool.intensity (i);
	});
	properties.addStacked (QObject::tr ("Intensity"), intensityEdit);

	QCheckBox& invertEdit = ViewUtil::checkBox (QObject::tr ("Invert"), tool.invert ());
	ViewUtil::connect (invertEdit, [&tool] (bool i) {
	  tool.invert (i);
	});
	properties.add (invertEdit);
}

void PropertiesWidget::updateImpl(ToolSculptPinch  & tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

	QCheckBox& invertEdit = ViewUtil::checkBox (QObject::tr ("Invert"), tool.invert ());
	ViewUtil::connect (invertEdit, [&tool] (bool i) {
	  tool.invert (i);
	});
	properties.add (invertEdit);
}

void PropertiesWidget::updateImpl(ToolSculptReduce & tool, ViewProperties& propertiesView)
{
	updateImpl(dynamic_cast<ToolSculpt&>(tool), propertiesView);
	ViewTwoColumnGrid& properties = propertiesView.body();

	ViewDoubleSlider& intensityEdit = ViewUtil::slider (2, 0.1f, tool.intensity (), 0.9f);
	ViewUtil::connect (intensityEdit, [&tool] (float i) {
	  tool.intensity (i);
	});
	properties.addStacked (QObject::tr ("Intensity"), intensityEdit);
}

void PropertiesWidget::updateImpl(ToolNewSketch& tool, ViewProperties& propertiesView)
{

}

void PropertiesWidget::updateImpl(ToolModifySketch& tool, ViewProperties& propertiesView)
{
    ViewTwoColumnGrid& properties = propertiesView.body();

    properties.add (QObject::tr ("Move along"), ViewUtil::emptyWidget ());
    QButtonGroup& constraint = ViewUtil::contraint((int)tool.constraint());
    for (auto button: constraint.buttons())
    {
        properties.add(*button);
    }
    ViewUtil::connect(constraint, [&tool] (int r) {
        tool.constraint(MovementConstraint(r));
    });

    properties.add (ViewUtil::horizontalLine ());

    QPushButton& syncButton = ViewUtil::pushButton ("tool_sketch_modify_syncmirror", QObject::tr ("Sync"));
    ViewUtil::connect (syncButton, [&tool] () {
      tool.syncMirror();
    });
    syncButton.setEnabled (tool.hasMirror ());

    QAbstractButton& mirrorEdit = ViewUtil::checkBox ( "tool_sculpt_modify_mirror", QObject::tr ("Mirror")
                                               , tool.hasMirror () );
    ViewUtil::connectCheck (mirrorEdit, [&tool,&syncButton] (bool m) {
      tool.mirror (m);
      syncButton.setEnabled (m);
    });

    QCheckBox& transformCEdit = ViewUtil::checkBox ( QObject::tr ("Transform children")
                                                   , tool.transformChildren() );
    ViewUtil::connect (transformCEdit, [&tool] (bool m) {
        tool.transformChildren(m);
    });
    properties.add (transformCEdit);

    QCheckBox& snapEdit = ViewUtil::checkBox (QObject::tr ("Snap"), tool.snap());
    ViewUtil::connect (snapEdit, [&tool] (bool s) {
      tool.snap(s);
    });
    properties.add (snapEdit);

    QSlider& snapWidthEdit = ViewUtil::slider (1, tool.snapWidth(), 10);
    snapWidthEdit.setEnabled (tool.snap());
    ViewUtil::connect (snapWidthEdit, [&tool] (int w) {
        tool.snapWidth(w);
    });
    properties.addStacked (QObject::tr ("Snap width"), snapWidthEdit);
}

void PropertiesWidget::updateImpl(ToolDeleteSketch& tool, ViewProperties& propertiesView)
{
    ViewTwoColumnGrid& properties = propertiesView.body();

    QCheckBox& deleteChildrenEdit = ViewUtil::checkBox ( QObject::tr ("Delete children")
                                                       , tool.deleteChildren() );
    ViewUtil::connect (deleteChildrenEdit, [&tool] (bool m) {
      tool.deleteChildren(m);
    });
    deleteChildrenEdit.setEnabled (tool.mode() == DeleteSketchMode::DeleteNode);

    QCheckBox& mirrorEdit = ViewUtil::checkBox ( QObject::tr ("Mirror")
                                               , tool.hasMirror () );
    ViewUtil::connect (mirrorEdit, [&tool] (bool m) {
      tool.mirror (m);
    });
    mirrorEdit.setEnabled (tool.mode() != DeleteSketchMode::DeleteSketch);

    QRadioButton& deleteSketchEdit = ViewUtil::radioButton ( QObject::tr ("Delete sketch")
                                                           , tool.mode() == DeleteSketchMode::DeleteSketch );
    ViewUtil::connect (deleteSketchEdit,
      [&tool, &deleteChildrenEdit, &mirrorEdit] (bool m)
    {
      tool.mode(DeleteSketchMode::DeleteSketch);

      deleteChildrenEdit.setEnabled (!m);
      mirrorEdit        .setEnabled (!m);
    });

    QRadioButton& deleteNodeEdit = ViewUtil::radioButton ( QObject::tr ("Delete node")
                                                         , tool.mode() == DeleteSketchMode::DeleteNode );
    ViewUtil::connect (deleteNodeEdit,
      [&tool, &deleteChildrenEdit, &mirrorEdit] (bool m)
    {
      tool.mode(DeleteSketchMode::DeleteNode);
      deleteChildrenEdit.setEnabled (m);
      mirrorEdit        .setEnabled (m);
    });

    QRadioButton& deleteSpheresEdit = ViewUtil::radioButton ( QObject::tr ("Delete spheres")
                                                            , tool.mode() == DeleteSketchMode::DeleteSpheres );
    ViewUtil::connect (deleteSpheresEdit,
      [&tool, &deleteChildrenEdit, &mirrorEdit] (bool m)
    {
      tool.mode(DeleteSketchMode::DeleteSpheres);

      deleteChildrenEdit.setEnabled (!m);
      mirrorEdit        .setEnabled (m);
    });

    properties.add (deleteSketchEdit);
    properties.add (deleteNodeEdit);
    properties.add (deleteSpheresEdit);
    properties.add (ViewUtil::horizontalLine ());
    properties.add (deleteChildrenEdit);
    properties.add (mirrorEdit);

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
    ViewTwoColumnGrid& properties = propertiesView.body ();

    QPushButton& syncButton = ViewUtil::pushButton ("sync", QObject::tr ("Sync"));
    ViewUtil::connect (syncButton, [&tool] () {
      tool.syncMirror();
    });
    syncButton.setEnabled (tool.hasMirror ());

    QCheckBox& mirrorEdit = ViewUtil::checkBox ( QObject::tr ("Mirror")
                                               , tool.hasMirror () );
    ViewUtil::connect (mirrorEdit, [&tool, &syncButton] (bool m) {
      tool.mirror (m);
      syncButton.setEnabled (m);
    });
    properties.add (mirrorEdit, syncButton);

    ViewDoubleSlider& radiusEdit = ViewUtil::slider ( 2, 0.01f, tool.radius(), 0.3f );
    ViewUtil::connect (radiusEdit, [&tool] (float r) {
        tool.radius(r);
    });
    properties.addStacked (QObject::tr ("Radius"), radiusEdit);

    ViewDoubleSlider& heightEdit = ViewUtil::slider ( 2, 0.01f, tool.height(), 0.45f );
    ViewUtil::connect (heightEdit, [&tool] (float d) {
        tool.height(d);
    });
    properties.addStacked (QObject::tr ("Height"), heightEdit);

    QButtonGroup& smoothEffectEdit = ViewUtil::buttonGroup({
                                  QObject::tr ("None")
                                , QObject::tr ("Embed")
                                , QObject::tr ("Embed and adjust")
                                , QObject::tr ("Pinch")
                                }, int (tool.smoothEffect()) );
    ViewUtil::connect (smoothEffectEdit, [&tool] (int id) {
        tool.smoothEffect(SketchPathSmoothEffect(id));
    });
}
