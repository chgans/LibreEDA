#ifndef PENWIDTHCOMBOBOX_H
#define PENWIDTHCOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>

class UTILS_EXPORT PenWidthComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit PenWidthComboBox(QWidget *parent = 0);

    void addItem(const QString &label, qreal width);

public slots:
    void setCurrentIndex(qreal width);

signals:
    void activated(qreal width);
    void currentIndexChanged(qreal width);
    void highlighted(qreal width);

};

#endif // PENWIDTHCOMBOBOX_H
