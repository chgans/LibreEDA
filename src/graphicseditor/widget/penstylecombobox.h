#ifndef PENSTYLECOMBOBOX_H
#define PENSTYLECOMBOBOX_H

#include <QComboBox>
#include <QPen>

class PenStyleComboBox : public QComboBox
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

#endif // PENSTYLECOMBOBOX_H
