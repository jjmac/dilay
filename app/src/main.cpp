/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include "cache.hpp"
#include "config.hpp"
#include "opengl.hpp"
#include "main-window.hpp"


#include <QSurfaceFormat>

void opengl_setDefaultFormat () {
  QSurfaceFormat format;

  format.setVersion           (2, 1);
  format.setDepthBufferSize   (24);
  format.setStencilBufferSize (1);
  format.setProfile           (QSurfaceFormat::NoProfile);
  format.setRenderableType    (QSurfaceFormat::OpenGL);

  QSurfaceFormat::setDefaultFormat (format);
}

int main (int argv, char **args) {
  QCoreApplication::setAttribute (Qt::AA_UseDesktopOpenGL);
  opengl_setDefaultFormat ();

  QApplication app (argv, args);

  const std::string configFileName = QStandardPaths::locate ( QStandardPaths::ConfigLocation
                                                            , "dilay.config" ).toStdString ();
  Config config;
  Cache  cache;

  if (configFileName.empty () == false) {
    config.fromFile (configFileName);
  }

  ViewMainWindow mainWindow (config, cache);
  mainWindow.resize ( config.get <int> ("window/initialWidth")
					, config.get <int> ("window/initialHeight") );
  mainWindow.show   ();

  QObject::connect (&app, &QApplication::aboutToQuit, [&config] () {
    const QString configDirName (QStandardPaths::writableLocation (QStandardPaths::ConfigLocation));
    const QDir    configDir     (configDirName);

    if (configDirName.isEmpty () == false) {
      if (configDir.exists () == false) {
        configDir.mkpath (".");
      }
      config.toFile (configDir.filePath ("dilay.config").toStdString ());
    }
  });
  return app.exec();
}
