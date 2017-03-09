/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#include <QVBoxLayout>
#include "properties.hpp"
#include <assert.h>


ViewProperties::ViewProperties ()
    : _header (nullptr)
    , _body   (nullptr)
    , _footer (nullptr)
{
    setLayout (&this->layout);
    layout.setContentsMargins (0,0,0,0);
    reset ();
}

ViewTwoColumnGrid& ViewProperties::header () {
    assert (this->_header);
    return *this->_header;
}

ViewTwoColumnGrid& ViewProperties::body () {
    assert (this->_body);
    return *this->_body;
}

ViewTwoColumnGrid& ViewProperties::footer () {
    assert (this->_footer);
    return *this->_footer;
}

void ViewProperties::resetTwoColumnGrid (ViewTwoColumnGrid*& grid) {
    if (grid) {
        this->layout.removeWidget (grid);
        delete grid;
    }
    grid = new ViewTwoColumnGrid;
    this->layout.addWidget (grid);
}

void ViewProperties::reset () {
    this->resetTwoColumnGrid (this->_header);
    this->resetTwoColumnGrid (this->_body);
    this->resetTwoColumnGrid (this->_footer);
}

