#ifndef PENJOINSTYLECOMBOBOX_H
#define PENJOINSTYLECOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>
#include <QPen>

class UTILS_EXPORT PenJoinStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit PenJoinStyleComboBox(QWidget *parent = nullptr);

    void addItem(Qt::PenJoinStyle style);

public slots:
    void setCurrentIndex(Qt::PenJoinStyle style);

signals:
    void activated(Qt::PenJoinStyle style);
    void currentIndexChanged(Qt::PenJoinStyle style);
    void highlighted(Qt::PenJoinStyle style);

};

Q_DECLARE_METATYPE(Qt::PenJoinStyle)

#endif // PENJOINSTYLECOMBOBOX_H
