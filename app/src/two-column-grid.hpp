/* This file is part of Dilay
 * Copyright © 2015,2016 Alexander Bau
 * Use and redistribute under the terms of the GNU General Public License
 */
#ifndef DILAY_VIEW_TWO_COLUMN_GRID
#define DILAY_VIEW_TWO_COLUMN_GRID

#include <QWidget>
#include <vector>

class QButtonGroup;
class QString;
class QGridLayout;

class ViewTwoColumnGrid : public QWidget
{
    Q_OBJECT

  public:
    ViewTwoColumnGrid();

    void add          (QWidget&);
    void add          (const QString&, QWidget&);
    void add          (QWidget&, QWidget&);
    void addStacked   (const QString&, QWidget&);
    void add          (QButtonGroup&, const std::vector <QString>&);
    void addLeft      (const QString&);
    void addCenter    (const QString&);
    void addStretcher ();

private:
    QGridLayout*       m_layout;
    unsigned int       m_numProperties;
};

#endif
