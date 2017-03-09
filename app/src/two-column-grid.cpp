/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QButtonGroup>
#include <QGridLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include "two-column-grid.hpp"
#include "view-util.hpp"

ViewTwoColumnGrid::ViewTwoColumnGrid()
    : m_layout        (nullptr)
    , m_numProperties (0)
{
    m_layout = new QGridLayout;
    m_layout->setColumnStretch (0,0);
    m_layout->setColumnStretch (1,1);
    setLayout (m_layout);
    m_numProperties = 0;
}

void ViewTwoColumnGrid::add (QWidget& w)
{
    m_layout->addWidget (&w, this->m_numProperties, 0, 1, 2);
    m_layout->setRowStretch (this->m_numProperties, 0);
    m_numProperties++;
}

void ViewTwoColumnGrid::add (const QString& label, QWidget& w)
{
    add (*new QLabel (label), w);
}

void ViewTwoColumnGrid::add (QWidget& w1, QWidget& w2)
{
    m_layout->addWidget (&w1, this->m_numProperties, 0);
    m_layout->addWidget (&w2, this->m_numProperties, 1);
    m_layout->setRowStretch (this->m_numProperties, 0);
    m_numProperties++;
}

void ViewTwoColumnGrid::addStacked (const QString& l, QWidget& w)
{
    QVBoxLayout& stack = *new QVBoxLayout;
    QLabel&      label = *new QLabel (l);

    label.setAlignment (Qt::AlignHCenter);
    stack.addWidget (&label);
    stack.addWidget (&w);

    m_layout->addLayout (&stack, this->m_numProperties, 0, 1, 2);
    m_layout->setRowStretch (this->m_numProperties, 0);
    m_numProperties++;
}

void ViewTwoColumnGrid::add (QButtonGroup& group, const std::vector <QString>& labels)
{
    int id = 0;
    for (const QString& label : labels)
    {
        QRadioButton& button = ViewUtil::radioButton (label);

        group.addButton (&button, id);
        add       (button);
        id++;
    }
}

void ViewTwoColumnGrid::addLeft (const QString& label)
{
    add (label, *new QWidget);
}

void ViewTwoColumnGrid::addCenter (const QString& label)
{
    add (*new QLabel (label));
}

void ViewTwoColumnGrid::addStretcher ()
{
    add (*new QWidget, *new QWidget);
    m_layout->setRowStretch (this->m_numProperties, 1);
}
