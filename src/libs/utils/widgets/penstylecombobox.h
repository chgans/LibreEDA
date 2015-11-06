#ifndef PENSTYLECOMBOBOX_H
#define PENSTYLECOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>
#include <QPen>

class UTILS_EXPORT PenStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit PenStyleComboBox(QWidget *parent = 0);

    void addItem(Qt::PenStyle style);

public slots:
    void setCurrentIndex(Qt::PenStyle style);

signals:
    void activated(Qt::PenStyle style);
    void currentIndexChanged(Qt::PenStyle style);
    void highlighted(Qt::PenStyle style);
};

Q_DECLARE_METATYPE(Qt::PenStyle)

#endif // PENSTYLECOMBOBOX_H
