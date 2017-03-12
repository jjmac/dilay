/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_VECTOR_EDIT
#define DILAY_VIEW_VECTOR_EDIT

#include <QWidget>
#include <glm/glm.hpp>

class QLineEdit;

class ViewVectorEdit : public QWidget {
  Q_OBJECT

  public:
    ViewVectorEdit(const glm::vec3&, QWidget* = nullptr);

    void vector (const glm::vec3&);
    void x      (float);
    void y      (float);
    void z      (float);

  signals:
    void vectorEdited (const glm::vec3&);

private:
    void changeComponent (int i, float v);

  private:
    glm::vec3         m_vectorData;
    QLineEdit*        m_edit [3];
};

#endif
