/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QCloseEvent>
#include <QLabel>
#include <QShortcut>
#include <QStatusBar>
#include "scene.hpp"
#include "state.hpp"
#include "tool/move-camera.hpp"
#include "gl-widget.hpp"
#include "main-widget.hpp"
#include "main-window.hpp"
#include "menu-bar.hpp"
#include "view/tool-tip.hpp"
#include "view/util.hpp"
#include "view/properties.hpp"

ViewMainWindow::ViewMainWindow (Config& config, Cache& cache)
{
    m_mainWidget = new ViewMainWidget (*this, config, cache);
    m_statusBar = new QStatusBar();
    m_messageLabel = new QLabel();
    m_numFacesLabel = new QLabel();

    setCentralWidget(m_mainWidget);

    ViewMenuBar::setup (*this, m_mainWidget->glWidget ());

    setupShortcuts ();
    setupStatusBar ();
}

void ViewMainWindow::setupShortcuts ()
{
    auto addShortcut = [this] (const QKeySequence& keySequence, const std::function <void ()>& f) {
        QShortcut* s = new QShortcut (keySequence, this);
        QObject::connect (s, &QShortcut::activated, f);
    };

    addShortcut (Qt::Key_Escape, [this] () {
        if (this->m_mainWidget->glWidget ().state ().hasTool ())
        {
            this->m_mainWidget->glWidget ().state ().resetTool ();
        }
#ifndef NDEBUG
        else
        {
            close ();
        }
#endif
    });
#ifndef NDEBUG
    addShortcut (Qt::Key_I, [this] () {
      this->m_mainWidget->glWidget ().state ().scene ().printStatistics (false);
    });
    addShortcut (Qt::SHIFT + Qt::Key_I, [this] () {
        this->m_mainWidget->glWidget ().state ().scene ().printStatistics (true);
    });
#endif
    addShortcut (Qt::SHIFT + Qt::Key_C, [this] () {
        this->m_mainWidget->glWidget ()
                .toolMoveCamera ()
                .snap (this->m_mainWidget->glWidget ().state (), true);
    });
}

void ViewMainWindow::setupStatusBar ()
{
    setStatusBar (m_statusBar);

    m_statusBar->setStyleSheet      ("QStatusBar::item { border: 0px solid black };");
    m_statusBar->setSizeGripEnabled (false);
    m_statusBar->addPermanentWidget (m_messageLabel,1);
    m_statusBar->addPermanentWidget (m_numFacesLabel);

    showDefaultToolTip ();
    showNumFaces       (0);
}

void ViewMainWindow::showMessage (const QString& message)
{
    m_messageLabel->setText (message);
}

void ViewMainWindow::showToolTip (const ViewToolTip& tip)
{
    showMessage (tip.toString ());
}

void ViewMainWindow::showDefaultToolTip ()
{
    ViewToolTip tip;

    tip.add ( ViewToolTip::MouseEvent::Middle, QObject::tr ("Drag to rotate"));
    tip.add ( ViewToolTip::MouseEvent::Middle, ViewToolTip::Modifier::Shift
            , QObject::tr ("Drag to move"));
    tip.add ( ViewToolTip::MouseEvent::Middle, ViewToolTip::Modifier::Ctrl
            , QObject::tr ("Gaze"));


    showToolTip (tip);
}

void ViewMainWindow::showNumFaces (unsigned int n) {
    m_numFacesLabel->setText (QString::number (n).append (" faces"));
}

void ViewMainWindow::update ()
{
    QMainWindow::update ();
    m_mainWidget->update ();
}

void ViewMainWindow::resetProperties()
{
    m_mainWidget->properties ().reset ();
}

void ViewMainWindow::deselectTool()
{
    m_mainWidget->deselectTool ();
}

void ViewMainWindow::updateGl()
{
    m_mainWidget->glWidget ().update ();
}

ViewProperties& ViewMainWindow::viewProperties()
{
    return m_mainWidget->properties ();
}

glm::ivec2 ViewMainWindow::cursorPosition()
{
    return m_mainWidget->glWidget ().cursorPosition ();
}

void ViewMainWindow::closeEvent (QCloseEvent* e)
{
#ifndef NDEBUG
    e->accept ();
#else
    if (ViewUtil::question (*this, QObject::tr ("Do you want to quit?"))) {
      e->accept ();
    }
    else {
      e->ignore ();
    }
#endif
}
