/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QVBoxLayout>
#include <QLineEdit>
#include "vector-edit.hpp"
#include "view-util.hpp"

namespace {
    static const int  numDecimals = 2;
}

ViewVectorEdit::ViewVectorEdit (const glm::vec3& v, QWidget* parent)
    : QWidget(parent)
    , m_vectorData (v)
{
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing         (0);
    layout->setContentsMargins (0,11,0,11);
    setLayout      (layout);

    for (int i = 0; i <= 2; i++) {
      m_edit[i] = &ViewUtil::lineEdit (v[i], numDecimals);

      ViewUtil::connectFloat (*m_edit[i], [this,i] (float v) {
        this->m_vectorData[i] = v;
        emit vectorEdited (m_vectorData);
      });

      layout->addWidget (m_edit[i]);
    }
}

void ViewVectorEdit::vector (const glm::vec3& v) { this->x (v.x); this->y (v.y); this->z (v.z); }
void ViewVectorEdit::x      (float v)            { this->changeComponent (0,v); }
void ViewVectorEdit::y      (float v)            { this->changeComponent (1,v); }
void ViewVectorEdit::z      (float v)            { this->changeComponent (2,v); }

void ViewVectorEdit::changeComponent (int i, float v) {

    m_vectorData[i] = v;
    m_edit[i]->setText (QString::number (v, 'f', numDecimals));
}

