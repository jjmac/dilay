#include "sculptview.h"
#include <QtQuick/qquickwindow.h>
#include "view/gl-renderer.hpp"

#include <QApplication>
#include <QStandardPaths>
#include <QtQuick/QQuickWindow>
#include <QDebug>
#include <QTimer>

SculptView::SculptView()
    : m_renderer(nullptr)
{
    const std::string configFileName = QStandardPaths::locate ( QStandardPaths::ConfigLocation
                                                                , "dilay.config" ).toStdString ();
    if (configFileName.empty () == false) {
      config.fromFile (configFileName);
    }
    connect(this, &QQuickItem::windowChanged, this, &SculptView::handleWindowChanged);

//    setClearBeforeRendering(false);

//   setResizeMode(SizeRootObjectToView);
//   rootCon
}

void SculptView::handleWindowChanged(QQuickWindow *win)
{
    qDebug() << "handleWindowChanged" << (win ? " Y" : " N");
    if (win) {
        connect(win,  &QQuickWindow::sceneGraphInitialized,
                this, &SculptView::initializeUnderlay,
                Qt::DirectConnection);
        connect(win,  &QQuickWindow::beforeSynchronizing,
                this, &SculptView::synchronizeUnderlay,
                Qt::DirectConnection);
        connect(win, &QQuickWindow::beforeRendering,
                this, &SculptView::renderUnderlay,
                Qt::DirectConnection);
        connect(win,  &QQuickWindow::sceneGraphInvalidated,
                this, &SculptView::invalidateUnderlay,
                Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
//        win->setResizeMode(SizeRootObjectToView);

    }
}

void SculptView::initializeUnderlay()
{
    if (!m_renderer) {

       m_renderer = new ViewGlRenderer(*window(), config, cache);
       m_renderer->initializeGL();
//       m_renderer->resizeGL(this->width(), this->height());

//        connect(window(), &QQuickWindow::beforeRendering,
//                this, &SculptView::paint,
//                Qt::DirectConnection);
        QTimer* timer = new QTimer(this);
        connect(timer, &QTimer::timeout, window(), &QQuickWindow::update, Qt::QueuedConnection);
        timer->start(100);
    }
    m_renderer->initializeGL();
    window()->resetOpenGLState();
//    resetOpenGLState();

//    QTimer* timer = new QTimer(this);
//    connect(timer, &QTimer::timeout, window(), &QQuickWindow::update, Qt::QueuedConnection);
//    timer->start(100);
}

void SculptView::renderUnderlay()
{
    qDebug() <<"SculptView::paint";
    m_renderer->paint();
    window()->resetOpenGLState();
}

void SculptView::invalidateUnderlay()
{
    qDebug() <<"SculptView::cleanup";
//    if (m_renderer) {
//        delete m_renderer;
//        m_renderer = nullptr;
//    }
}

void SculptView::synchronizeUnderlay()
{
    qDebug() <<"SculptView::sync";
//    m_renderer->resizeGL(width(), height());
}

