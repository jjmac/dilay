#include "tooltip-widget.hpp"
#include "view-util.hpp"
#include "tool-tip.hpp"


TooltipWidget::TooltipWidget()
	: QLabel()
{
	ViewToolTip tip;

	tip.add ( ViewToolTip::MouseEvent::Middle, QObject::tr ("Drag to rotate"));
	tip.add ( ViewToolTip::MouseEvent::Middle, ViewToolTip::Modifier::Shift
			, QObject::tr ("Drag to move"));
	tip.add ( ViewToolTip::MouseEvent::Middle, ViewToolTip::Modifier::Ctrl
			, QObject::tr ("Gaze"));


	showTooltip (tip);
}

void TooltipWidget::reset()
{
	setText("");
}

void TooltipWidget::showMessage (const QString& message)
{
	setText(message);
}

void TooltipWidget::showTooltip(const ViewToolTip& tip)
{
	setText(tip.toString());
}

void TooltipWidget::updateImpl(ToolMoveMesh& tool)
{
	ViewToolTip toolTip;
	toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Drag to move"));

	if (tool.constraint() != MovementConstraint::CameraPlane) {
	  toolTip.add ( ViewToolTip::MouseEvent::Left, ViewToolTip::Modifier::Shift
				  , QObject::tr ("Drag to move orthogonally") );
	}
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolDeleteMesh& tool)
{
	ViewToolTip toolTip;
	toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Delete selection"));
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolNewMesh& tool)
{
}

void TooltipWidget::updateImpl(ToolSculpt& tool, ViewToolTip& toolTip)
{
	toolTip.add ( ViewToolTip::MouseEvent::Wheel, ViewToolTip::Modifier::Shift
				, QObject::tr ("Change radius") );

}

void TooltipWidget::updateImpl(ToolSculptCarve  & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptDrag & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptGrab   & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptSmooth & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptFlatten& tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptCrease & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptPinch  & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSculptReduce & tool)
{
	ViewToolTip toolTip;
	updateImpl(dynamic_cast<ToolSculpt&>(tool), toolTip);
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolNewSketch& tool)
{

}

void TooltipWidget::updateImpl(ToolModifySketch& tool)
{
	ViewToolTip toolTip;
	toolTip.add ( ViewToolTip::MouseEvent::Left, QObject::tr ("Drag to sketch"));
	toolTip.add ( ViewToolTip::MouseEvent::Left, ViewToolTip::Modifier::Shift
				, QObject::tr ("Drag to smooth"));
	toolTip.add ( ViewToolTip::MouseEvent::Wheel, ViewToolTip::Modifier::Shift
				, QObject::tr ("Change radius") );
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolDeleteSketch& tool)
{
	ViewToolTip toolTip;
	toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Delete selection"));
	showTooltip (toolTip);
}

void TooltipWidget::updateImpl(ToolRebalanceSketch& tool)
{
	ViewToolTip toolTip;
	toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Set new root"));
	showTooltip (toolTip);
}

void TooltipWidget::updateImpl(ToolConvertSketch & tool)
{
	ViewToolTip toolTip;
	toolTip.add (ViewToolTip::MouseEvent::Left, QObject::tr ("Convert selection"));
	showTooltip(toolTip);
}

void TooltipWidget::updateImpl(ToolSketchSpheres& tool)
{
	ViewToolTip toolTip;
	toolTip.add ( ViewToolTip::MouseEvent::Left, QObject::tr ("Drag to sketch"));
	toolTip.add ( ViewToolTip::MouseEvent::Left, ViewToolTip::Modifier::Shift
				, QObject::tr ("Drag to smooth"));
	toolTip.add ( ViewToolTip::MouseEvent::Wheel, ViewToolTip::Modifier::Shift
				, QObject::tr ("Change radius") );
	showTooltip (toolTip);
}
