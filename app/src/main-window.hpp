/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_MAIN_WINDOW
#define DILAY_VIEW_MAIN_WINDOW

#include <QMainWindow>
#include "controller.h"

class Cache;
class Config;
class QCloseEvent;
class ViewMainWidget;
class ViewToolTip;
class QLabel;

class ViewMainWindow : public QMainWindow, public Controller
{
    Q_OBJECT

public:
    ViewMainWindow(Config&, Cache&);

    ViewMainWidget& mainWidget         ();
    void            showMessage        (const QString&);
    void            showToolTip        (const ViewToolTip&);
    void            showDefaultToolTip ();
    void            showNumFaces       (unsigned int);
    void            update             ();

    void resetProperties();
    // this->mainWindow->mainWidget ().properties ().reset ();
    void deselectTool();
    // this->mainWindow->mainWidget ().deselectTool ();
    void updateGl();
    // this->state.mainWindow ().mainWidget ().glWidget ().update ();
    ViewProperties& viewProperties();
    // return this->state.mainWindow ().mainWidget ().properties ();
    glm::ivec2 cursorPosition();
    // return this->state.mainWindow ().mainWidget ().glWidget ().cursorPosition ();


protected:
    void            closeEvent         (QCloseEvent*);

private:
    void setupShortcuts ();
    void setupStatusBar();

private:

    ViewMainWidget* m_mainWidget;
    QStatusBar*     m_statusBar;
    QLabel*         m_messageLabel;
    QLabel*         m_numFacesLabel;
};
#endif
