/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QPushButton>
#include <QToolButton>
#include <QButtonGroup>
#include <QScrollArea>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSlider>
#include <QStackedLayout>
#include <QGridLayout>
#include <QTabBar>
#include "state.hpp"
#include "tools.hpp"
#include "gl-widget.hpp"
#include "properties-widget.hpp"
#include "main-widget.hpp"
#include "view/properties.hpp"
#include "view/util.hpp"
#include "properties-widget.hpp"
#include <QFrame>

ViewMainWidget::ViewMainWidget (ViewMainWindow& mW, Config& config, Cache& cache)
    : m_glWidget   (new ViewGlWidget (mW, config, cache))
    , m_properties (new PropertiesWidget)
{
    QGridLayout* selfLayout = new QGridLayout(this);
    selfLayout->addWidget (m_glWidget);

    QGridLayout *controlsLayout = new QGridLayout(m_glWidget);
    controlsLayout->setColumnStretch(0, 0);
    controlsLayout->setColumnStretch(1, 1);
    controlsLayout->setColumnStretch(2, 0);
    controlsLayout->addWidget(initalizeToolPane (), 0, 0, Qt::AlignLeft);
    controlsLayout->addWidget(initializeSettingsPane(), 0, 1, Qt::AlignLeft);
    controlsLayout->addWidget(m_properties, 0, 2, Qt::AlignLeft);
}

QWidget* ViewMainWidget::initalizeToolPane ()
{
	QWidget* newToolPane = new QWidget();

	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(128, 128, 128));
	newToolPane->setAutoFillBackground(true);
	newToolPane->setPalette(pal);
    QVBoxLayout* layout = new QVBoxLayout(newToolPane);
    QHBoxLayout* menuLayout = new QHBoxLayout();
    QAbstractButton& sculptButton = ViewUtil::pushButton ("sculpt_mode", "Sculpt");
    sculptButton.setCheckable(true);
    sculptButton.setChecked(true);
    menuLayout->addWidget(&sculptButton);
    QAbstractButton& sketchButton = ViewUtil::pushButton ("sketch_mode", "Sketch");
    sketchButton.setCheckable(true);
    menuLayout->addWidget(&sketchButton);
    QAbstractButton& settingsButton = ViewUtil::pushButton ("settings_mode", "Settings");
    settingsButton.setCheckable(true);
    menuLayout->addWidget(&settingsButton);

    layout->addLayout(menuLayout);
    QStackedLayout* toolsLayout = new QStackedLayout();
    toolsLayout->addWidget(initalizeSculptToolPane ());
    toolsLayout->addWidget(initalizeSketchToolPane ());
    layout->addLayout(toolsLayout);

    QObject::connect(&sculptButton, &QPushButton::toggled,
                     [toolsLayout](bool c) {
        if (c ) toolsLayout->setCurrentIndex(0);
    });

    QObject::connect(&sketchButton, &QPushButton::toggled,
                     [toolsLayout](bool c) {
        if (c ) toolsLayout->setCurrentIndex(1);
    });

    QButtonGroup * buttonGroup = new QButtonGroup(menuLayout);
    buttonGroup->setExclusive(true);
    buttonGroup->addButton(&sculptButton);
    buttonGroup->addButton(&sketchButton);

    return newToolPane;


//    QTabWidget* toolPane = new QTabWidget;
//    toolPane->setAttribute(Qt::WA_TranslucentBackground);
//    toolPane->addTab (this->initalizeSculptToolPane (), QObject::tr ("Sculpt"));
//    toolPane->widget(0)->setAttribute(Qt::WA_TranslucentBackground);
////    toolPane->tabBar()->tabButton(0, QTabBar::LeftSide)->setObjectName("tool_sculpt_selector");
//    toolPane->addTab (this->initalizeSketchToolPane (), QObject::tr ("Sketch"));
//    toolPane->widget(1)->setAttribute(Qt::WA_TranslucentBackground);
////    toolPane->tabBar()->tabButton(1, QTabBar::LeftSide)->setObjectName("tool_sketch_selector");

//    QObject::connect (toolPane, &QTabWidget::currentChanged, [this] (int) {
//      m_glWidget->state ().resetTool ();
//    });
//    return toolPane;
}

QWidget* ViewMainWidget::initalizeSculptToolPane ()
{
    QWidget*     toolPane       = new QWidget;
    toolPane->setAttribute(Qt::WA_TranslucentBackground);
    QVBoxLayout* toolPaneLayout = new QVBoxLayout;

    toolPane->setLayout (toolPaneLayout);
    this->addToolButton <ToolNewMesh>       (toolPaneLayout, "tool_sculpt_new_mesh", QObject::tr ("New mesh"));
    this->addToolButton <ToolDeleteMesh>    (toolPaneLayout, "tool_sculpt_delete_mesh", QObject::tr ("Delete mesh"));
    this->addToolButton <ToolMoveMesh>      (toolPaneLayout, "tool_sculpt_move_mesh", QObject::tr ("Move mesh"));
    toolPaneLayout->addWidget (&ViewUtil::horizontalLine ());
    this->addToolButton <ToolSculptCarve>   (toolPaneLayout, "tool_sculpt_carve", QObject::tr ("Carve"));
    this->addToolButton <ToolSculptCrease>  (toolPaneLayout, "tool_sculpt_crease", QObject::tr ("Crease"));
    this->addToolButton <ToolSculptGrab>    (toolPaneLayout, "tool_sculpt_grab", QObject::tr ("Grab"));
    this->addToolButton <ToolSculptDrag>    (toolPaneLayout, "tool_sculpt_drag", QObject::tr ("Drag"));
    this->addToolButton <ToolSculptFlatten> (toolPaneLayout, "tool_sculpt_flatten", QObject::tr ("Flatten"));
    this->addToolButton <ToolSculptSmooth>  (toolPaneLayout, "tool_sculpt_smooth", QObject::tr ("Smooth"));
    this->addToolButton <ToolSculptPinch>   (toolPaneLayout, "tool_sculpt_pinch", QObject::tr ("Pinch"));
    this->addToolButton <ToolSculptReduce>  (toolPaneLayout, "tool_sculpt_reduce", QObject::tr ("Reduce"));

    toolPaneLayout->addStretch (1);
    return toolPane;
}

QWidget* ViewMainWidget::initializeSettingsPane ()
{
    QWidget*     toolPane       = new QWidget;
    toolPane->setObjectName("settings_pane");
    QFormLayout* toolPaneLayout = new QFormLayout;
    toolPane->setLayout (toolPaneLayout);

    toolPaneLayout->addRow("button size", new QSlider(toolPane));
    toolPaneLayout->addRow("button spacing", new QSlider(toolPane));
    //      toolPaneLayout->addRow("hide button label", new QCheckBox(toolPane));
    return toolPane;
}

QWidget* ViewMainWidget::initalizeSketchToolPane ()
{
    QWidget*     toolPane       = new QWidget;
    QVBoxLayout* toolPaneLayout = new QVBoxLayout;

    toolPane->setLayout (toolPaneLayout);
    this->addToolButton <ToolNewSketch>        (toolPaneLayout, "tool_sketch_new", QObject::tr ("New sketch"));
    this->addToolButton <ToolModifySketch>     (toolPaneLayout, "tool_sketch_modify", QObject::tr ("Modify sketch"));
    this->addToolButton <ToolDeleteSketch>     (toolPaneLayout, "tool_sketch_delete", QObject::tr ("Delete sketch"));
    this->addToolButton <ToolRebalanceSketch>  (toolPaneLayout, "tool_sketch_rebalance", QObject::tr ("Rebalance sketch"));
    toolPaneLayout->addWidget (&ViewUtil::horizontalLine ());
    this->addToolButton <ToolSketchSpheres>    (toolPaneLayout, "tool_sketch_spheres", QObject::tr ("Sketch spheres"));
    toolPaneLayout->addWidget (&ViewUtil::horizontalLine ());
    this->addToolButton <ToolConvertSketch>    (toolPaneLayout, "tool_sketch_convert", QObject::tr ("Convert sketch"));

    toolPaneLayout->addStretch (1);
    return toolPane;
}

template <typename T>
void ViewMainWidget::addToolButton (QLayout* layout, const QString& name, const QString& label) {
//    QPushButton& button = ViewUtil::pushButton (name, label);
    QAbstractButton& button = ViewUtil::pushButton (name, label);
    button.setCheckable (true);
    
    ViewUtil::connectRelease (button, [this, &button] () {
        selectOnly (button);

        State& s = m_glWidget->state ();
		m_properties->reset();
        s.resetTool (false);
		T* t = new T (s);
		s.setTool   (std::move (*t));
		m_properties->update(*t);

		this->m_glWidget->layout()->update();
    });
	layout->addWidget(&button);
    m_toolButtons.push_back (&button);
}

void ViewMainWidget::selectOnly (QAbstractButton& button)
{
    for (QAbstractButton* b : m_toolButtons)
    {
        b->setChecked (b == &button);
    }
}

ViewGlWidget& ViewMainWidget::glWidget ()
{
    return *m_glWidget;
}

ViewProperties& ViewMainWidget::properties ()
{
    static ViewProperties dummy;
    return dummy;
}

void ViewMainWidget::deselectTool ()
{
    assert (m_glWidget->state ().hasTool () == false);
    for (QAbstractButton* b : m_toolButtons)
    {
        b->setChecked (false);
    }
}

void ViewMainWidget::update ()
{
//    this->self->QSplitter::update ();
    m_glWidget->update ();
}
