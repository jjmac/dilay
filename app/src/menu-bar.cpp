/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QDesktopServices>
#include <QFileDialog>
#include <QMenuBar>
#include "dilay/history.hpp"
#include "dilay/scene.hpp"
#include "dilay/state.hpp"
#include "dilay/tool/move-camera.hpp"
#include "dilay/view/floor-plane.hpp"
#include "configuration.hpp"
#include "gl-widget.hpp"
#include "main-window.hpp"
#include "menu-bar.hpp"
#include "view-util.hpp"

#define DLY_FILE_FILTER "Dilay (*.dly)"
#define OBJ_FILE_FILTER "Wavefront (*.obj)"

namespace {
bool hasSuffix (const std::string& string, const std::string& suffix) {
  if (string.size () >= suffix.size ()) {
    return string.compare (string.size () - suffix.size (), suffix.size (), suffix) == 0;
  }
  else {
    return false;
  }
}

  QAction& addAction ( QMenu& menu, const QString& label, const QKeySequence& keySequence
                     , const std::function <void ()>& f )
  {
    QAction* a = new QAction (label, nullptr);
    a->setShortcut (keySequence);
    menu.addAction (a);
    QObject::connect (a, &QAction::triggered, f);
    return *a;
  }

  QAction& addCheckableAction ( QMenu& menu, const QString& label, const QKeySequence& keySequence
                              , bool state, const std::function <void (bool)>& f )
  {
    QAction* a = new QAction (label, nullptr);
    a->setShortcut (keySequence);
    a->setCheckable (true);
    a->setChecked  (state);
    menu.addAction (a);
    QObject::connect (a, &QAction::toggled, f);
    return *a;
  }

  QString getFileDialogPath (const Scene& scene) {
    return scene.hasFileName () 
      ? QString (scene.fileName ().c_str ())
      : QStandardPaths::standardLocations (QStandardPaths::HomeLocation).front ();
  }

  QString filterAllFiles () { return QObject::tr ("All files (*.*)"); }
  QString filterDlyFiles () { return QObject::tr ("Dilay files (*.dly)"); }
  QString filterObjFiles () { return QObject::tr ("Wavefront files (*.obj)"); }

  QString fileDialogFilters () {
    return filterAllFiles () + ";;" + filterDlyFiles () + ";;" + filterObjFiles ();
  }

  QString selectedFilter (const Scene& scene) {
    if (scene.hasFileName ()) {
      if (hasSuffix (scene.fileName (), ".dly")) {
        return filterDlyFiles ();
      }
      else if (hasSuffix (scene.fileName (), ".obj")) {
        return filterObjFiles ();
      }
    }
    return filterAllFiles ();
  }
}

void ViewMenuBar :: setup (ViewMainWindow& mainWindow, ViewGlWidget& glWidget) {
  QMenuBar& menuBar  = *mainWindow.menuBar ();
  QMenu&    fileMenu = *menuBar.addMenu (QObject::tr ("&File"));
  QMenu&    editMenu = *menuBar.addMenu (QObject::tr ("&Edit"));
  QMenu&    viewMenu = *menuBar.addMenu (QObject::tr ("&View"));
  QMenu&    helpMenu = *menuBar.addMenu (QObject::tr ("&Help"));

  addAction ( fileMenu, QObject::tr ("&Open..."), QKeySequence::Open
            , [&mainWindow, &glWidget] ()
  {
    Scene&            scene    = glWidget.state ().scene ();
          QString     filter   = filterAllFiles ();
    const std::string fileName = QFileDialog::getOpenFileName ( &mainWindow
                                                          , QObject::tr ("Open")
                                                          , getFileDialogPath (scene)
                                                          , fileDialogFilters ()
                                                          , &filter ).toStdString ();
    if (fileName.empty () == false) {
      if (scene.isEmpty () == false) {
        if (ViewUtil::question (mainWindow, QObject::tr ("Replace existent scene?"))) {
          scene.reset ();
          glWidget.state ().history ().reset ();
        }
        else {
          glWidget.state ().history ().snapshotAll (scene);
        }
      }
      if (scene.fromDlyFile (glWidget.state ().config (), fileName) == false) {
        ViewUtil::error (mainWindow, QObject::tr ("Could not open file."));
      }
      mainWindow.update ();
    }
  });
  QAction& saveAsAction = addAction ( fileMenu, QObject::tr ("Save &as...")
                                    , QKeySequence::SaveAs
                                    , [&mainWindow, &glWidget] () 
  {
    Scene&            scene    = glWidget.state ().scene ();
          QString     filter   = selectedFilter (scene);
    const std::string fileName = QFileDialog::getSaveFileName ( &mainWindow
                                                          , QObject::tr ("Save as")
                                                          , getFileDialogPath (scene)
                                                          , fileDialogFilters ()
                                                          , &filter ).toStdString ();
    if (fileName.empty () == false) {
      const bool saveAsObj = hasSuffix (fileName, ".obj") || filter == filterObjFiles ();

      if (scene.toDlyFile (fileName, saveAsObj) == false) {
        ViewUtil::error (mainWindow, QObject::tr ("Could not save to file."));
      }
      else if (saveAsObj && scene.numSketchMeshes () > 0) {
        ViewUtil::info (mainWindow, QObject::tr ("Sketches are omitted when saving Wavefront files."));
      }
    }
  });
  addAction ( fileMenu, QObject::tr ("&Save"), QKeySequence::Save
            , [&mainWindow, &glWidget, &saveAsAction] ()
  {
    Scene& scene = glWidget.state ().scene ();
    if (scene.hasFileName ()) {
      const bool saveAsObj = hasSuffix (scene.fileName (), ".obj");

      if (scene.toDlyFile (saveAsObj) == false) {
        ViewUtil::error (mainWindow, QObject::tr ("Could not save to file."));
      }
    }
    else {
      saveAsAction.trigger ();
    }
  });
  fileMenu.addSeparator ();
  addAction (fileMenu, QObject::tr ("&Quit"), QKeySequence::Quit , [&mainWindow] () {
    mainWindow.close ();
  });
  addAction (editMenu, QObject::tr ("&Undo"), QKeySequence::Undo, [&glWidget] () {
    glWidget.state ().undo ();
    glWidget.update();
  });
  addAction (editMenu, QObject::tr ("&Redo"), QKeySequence::Redo, [&glWidget] () {
    glWidget.state ().redo ();
    glWidget.update();
  });
  addAction ( editMenu, QObject::tr ("&Configuration"), QKeySequence ()
            , [&mainWindow, &glWidget] ()
  {
    ViewConfiguration::show (mainWindow, glWidget);
  });
  addAction (viewMenu, QObject::tr ("&Snap camera"), Qt::Key_C, [&glWidget] () {
    glWidget.toolMoveCamera ().snap (glWidget.state (), false);
  });
  addAction ( viewMenu, QObject::tr ("Reset &gaze point"), Qt::CTRL + Qt::Key_C
            , [&glWidget] ()
  {
    glWidget.toolMoveCamera ().resetGazePoint (glWidget.state ());
  });
  viewMenu.addSeparator ();
  addAction ( viewMenu, QObject::tr ("Toggle &wireframe"), Qt::Key_W
            , [&mainWindow, &glWidget] ()
  {
    glWidget.state ().scene ().toggleWireframe ();
    mainWindow.update ();
  });
  addAction ( viewMenu, QObject::tr ("Toggle &shading"), Qt::SHIFT + Qt::Key_W
            , [&mainWindow, &glWidget] ()
  {
    glWidget.state ().scene ().toggleShading ();
    mainWindow.update ();
  });
  addCheckableAction ( viewMenu, QObject::tr ("Show &floor plane"), Qt::Key_F
                     , false, [&mainWindow, &glWidget] (bool a)
  {
    glWidget.floorPlane ().isActive (a);
    mainWindow.update ();
  });
  addAction (helpMenu, QObject::tr ("&Manual..."), QKeySequence (), [&mainWindow] () {
    if (QDesktopServices::openUrl (QUrl ("http://abau.org/dilay/manual.html")) == false) {
      ViewUtil::error (mainWindow, QObject::tr ("Could not open manual."));
    }
  });
  addAction (helpMenu, QObject::tr ("&About Dilay..."), QKeySequence (), [&mainWindow] () {
    ViewUtil::about (mainWindow, QString ("Dilay %1 - ").arg(DILAY_VERSION)
                               + QObject::tr ("a 3D sculpting application")
                               + QString ("\n\n")
                               + QString ("Copyright © 2015,2016 Alexander Bau")
                               + QString ("\n\n")
                               + QObject::tr ("Use and redistribute under the terms of the GNU General Public License"));
  });
}
