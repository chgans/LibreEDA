#include "graphicsgrid.h"

// See http://www.qcad.org/doc/qcad/2.2/reference/en/chapter14.html
// for snap/constraint ideas

GraphicsGrid::GraphicsGrid():
    mPriority(0),
    mCoarseMultiplier(10),
    mFineLineStyle(Qt::SolidLine),
    mFineLineColor(QColor("#4d4d5c")),
    mCoarseLineStyle(Qt::SolidLine),
    mCoarseLineColor(QColor("#918d90")),
    mEnabledForComponents(true)
{
}

GraphicsGrid::GraphicsGrid(const GraphicsGrid &other):
    mLabel(other.mLabel),
    mPriority(other.mPriority),
    mOrigin(other.mOrigin),
    mCoarseMultiplier(other.mCoarseMultiplier),
    mFineLineStyle(other.mFineLineStyle),
    mFineLineColor(other.mFineLineColor),
    mCoarseLineStyle(other.mCoarseLineStyle),
    mCoarseLineColor(other.mCoarseLineColor),
    mEnabledForComponents(other.mEnabledForComponents)
{

}

GraphicsGrid::~GraphicsGrid()
{

}

QBrush GraphicsGrid::backgroundBrush() const
{
    return Qt::NoBrush;
}
