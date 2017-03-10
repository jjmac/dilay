#ifndef DILAY_TOOLTIP_WIDGET
#define DILAY_TOOLTIP_WIDGET

#include <QLabel>
#include <QWidget>
#include "tools.hpp"
class ViewToolTip;

class TooltipWidget : public QLabel
{
public:
	TooltipWidget();

	void reset();

    template<class T>
    void update(T& tool)
    {
		updateImpl(tool);
    }

	void showMessage (const QString& message);
	void showTooltip(const ViewToolTip& tip);
private:
	void setView(QLabel* properties);

	void updateImpl(ToolMoveMesh& tool) ;
	void updateImpl(ToolDeleteMesh& tool) ;
	void updateImpl(ToolNewMesh& tool) ;
	void updateImpl(ToolSculpt& tool, ViewToolTip& toolTip);
	void updateImpl(ToolSculptCarve  & tool) ;
	void updateImpl(ToolSculptDrag   & tool) ;
	void updateImpl(ToolSculptGrab   & tool) ;
	void updateImpl(ToolSculptSmooth & tool) ;
	void updateImpl(ToolSculptFlatten& tool) ;
	void updateImpl(ToolSculptCrease & tool) ;
	void updateImpl(ToolSculptPinch  & tool) ;
	void updateImpl(ToolSculptReduce & tool) ;
	void updateImpl(ToolNewSketch& tool) ;
	void updateImpl(ToolModifySketch& tool) ;
	void updateImpl(ToolDeleteSketch& tool) ;
	void updateImpl(ToolRebalanceSketch& tool) ;
	void updateImpl(ToolConvertSketch & tool) ;
	void updateImpl(ToolSketchSpheres& tool) ;

private:
	std::unique_ptr<QLabel> m_properties;
};

#endif

