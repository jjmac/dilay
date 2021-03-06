/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_PROPERTIES
#define DILAY_VIEW_PROPERTIES

#include <QVBoxLayout>
#include "two-column-grid.hpp"

class ViewProperties : public QWidget
{
    Q_OBJECT

  public:
    ViewProperties();

    void               reset  ();
    ViewTwoColumnGrid& header ();
    ViewTwoColumnGrid& body   ();
    ViewTwoColumnGrid& footer ();

private:
    void resetTwoColumnGrid (ViewTwoColumnGrid*& grid);
  private:
    QVBoxLayout        layout;
    ViewTwoColumnGrid* _header;
    ViewTwoColumnGrid* _body;
    ViewTwoColumnGrid* _footer;
};

#endif
