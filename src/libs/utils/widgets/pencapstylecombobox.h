#ifndef PENCAPSTYLECOMBOBOX_H
#define PENCAPSTYLECOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>
#include <QPen>

class UTILS_EXPORT PenCapStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit PenCapStyleComboBox(QWidget *parent = 0);

    void addItem(Qt::PenCapStyle style);

public slots:
    void setCurrentIndex(Qt::PenCapStyle style);

signals:
    void activated(Qt::PenCapStyle style);
    void currentIndexChanged(Qt::PenCapStyle style);
    void highlighted(Qt::PenCapStyle style);
};

Q_DECLARE_METATYPE(Qt::PenCapStyle)

#endif // PENCAPSTYLECOMBOBOX_H
