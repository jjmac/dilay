#ifndef DILAY_PROPERTIES_WIDGET
#define DILAY_PROPERTIES_WIDGET

#include <QWidget>
#include "tools.hpp"
#include "view/properties.hpp"

class PropertiesWidget : public QWidget
{
public:
    PropertiesWidget();

    template<class T>
    void update(T& tool)
    {
        ViewProperties* pv = new ViewProperties();
        updateImpl(tool, *pv);
        setView(pv);
    }

private:
    void setView(ViewProperties* properties);

    void updateImpl(ToolMoveMesh& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolDeleteMesh& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolNewMesh& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptCarve  & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptDrag   & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptGrab   & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptSmooth & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptFlatten& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptCrease & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptPinch  & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSculptReduce & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolNewSketch& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolModifySketch& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolDeleteSketch& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolRebalanceSketch& tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolConvertSketch & tool, ViewProperties& propertiesView) ;
    void updateImpl(ToolSketchSpheres& tool, ViewProperties& propertiesView) ;

private:
    std::unique_ptr<ViewProperties> m_properties;
};

#endif

