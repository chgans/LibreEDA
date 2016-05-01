#ifndef PENCOLORCOMBOBOX_H
#define PENCOLORCOMBOBOX_H

#include "utils/utils_global.h"

#include <QComboBox>
#include <QColor>
#include <QString>

class UTILS_EXPORT PenColorComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit PenColorComboBox(QWidget *parent = nullptr);

    void addItem(const QString &label, QColor color);

public slots:
    void setCurrentIndex(QColor color);

signals:
    void activated(QColor color);
    void currentIndexChanged(QColor color);
    void highlighted(QColor color);

};

#endif // PENCOLORCOMBOBOX_H
