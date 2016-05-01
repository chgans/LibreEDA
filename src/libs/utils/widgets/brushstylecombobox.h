#ifndef BRUSHSTYLECOMBOBOX_H
#define BRUSHSTYLECOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>
#include <QBrush>


class UTILS_EXPORT BrushStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit BrushStyleComboBox(QWidget *parent = nullptr);

    void addItem(Qt::BrushStyle style);

public slots:
    void setCurrentIndex(Qt::BrushStyle style);

signals:
    void activated(Qt::BrushStyle style);
    void currentIndexChanged(Qt::BrushStyle style);
    void highlighted(Qt::BrushStyle style);
};

Q_DECLARE_METATYPE(Qt::BrushStyle)

#endif // BRUSHSTYLECOMBOBOX_H
