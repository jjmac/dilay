/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_MAIN_WIDGET
#define DILAY_VIEW_MAIN_WIDGET

#include <QSplitter>

class Cache;
class Config;
class ViewMainWindow;
class ViewGlWidget;
class PropertiesWidget;
class QAbstractButton;
class ViewProperties;

class ViewMainWidget : public QWidget
{
    Q_OBJECT

public:
    ViewMainWidget(ViewMainWindow&, Config&, Cache&);

    ViewGlWidget&   glWidget     ();
    ViewProperties& properties   ();
    void            deselectTool ();
    void            update       ();

private:
    QWidget* initalizeToolPane ();
    QWidget* initalizeSculptToolPane ();
    QWidget* initalizeSketchToolPane ();
    QWidget* initializeSettingsPane ();
    template <typename T>
    void addToolButton (QLayout* layout, const QString& name, const QString& label);
    void selectOnly (QAbstractButton& button);

private:
    ViewGlWidget*              m_glWidget;
    PropertiesWidget*            m_properties;
    std::vector <QAbstractButton*> m_toolButtons;
};

#endif
