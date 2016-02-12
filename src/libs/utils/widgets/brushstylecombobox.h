#ifndef BRUSHSTYLECOMBOBOX_H
#define BRUSHSTYLECOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>
#include <QBrush>


class UTILS_EXPORT BrushStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    BrushStyleComboBox(QWidget *parent = 0);

    void addItem(Qt::BrushStyle style);

public slots:
    void setCurrentIndex(Qt::BrushStyle style);

signals:
    void activated(Qt::BrushStyle style);
    void currentIndexChanged(Qt::BrushStyle style);
    void highlighted(Qt::BrushStyle style);
};

#endif // BRUSHSTYLECOMBOBOX_H
