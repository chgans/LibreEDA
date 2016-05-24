#include "grid/graphicsgrid.h"
#include "palette.h"

// See http://www.qcad.org/doc/qcad/2.2/reference/en/chapter14.html
// for snap/constraint ideas

Grid::Grid():
    m_priority(0),
    m_coarseMultiplier(5),
    m_fineLineStyle(Qt::SolidLine),
    m_fineLineColor(QColor("#4d4d5c")),
    m_coarseLineStyle(Qt::SolidLine),
    m_coarseLineColor(QColor("#918d90"))
{
}

Grid::Grid(const Grid &other):
    m_label(other.m_label),
    m_priority(other.m_priority),
    m_origin(other.m_origin),
    m_coarseMultiplier(other.m_coarseMultiplier),
    m_fineLineStyle(other.m_fineLineStyle),
    m_fineLineColor(other.m_fineLineColor),
    m_coarseLineStyle(other.m_coarseLineStyle),
    m_coarseLineColor(other.m_coarseLineColor)
{

}

Grid::~Grid()
{

}
