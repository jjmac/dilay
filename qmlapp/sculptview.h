#pragma once
#include <QtQuick/QQuickItem>
#include "config.hpp"
#include "cache.hpp"

class ViewGlRenderer;

class SculptView : public QQuickItem
{
   Q_OBJECT

public:
    SculptView();

public slots:

private slots:
    void handleWindowChanged(QQuickWindow *win);
    void initializeUnderlay();
    void synchronizeUnderlay();
    void renderUnderlay();
    void invalidateUnderlay();

private:
    ViewGlRenderer* m_renderer;
    Config config;
    Cache  cache;
};
