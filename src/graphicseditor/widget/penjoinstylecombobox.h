#ifndef PENJOINSTYLECOMBOBOX_H
#define PENJOINSTYLECOMBOBOX_H

#include <QComboBox>
#include <QPen>

class PenJoinStyleComboBox : public QComboBox
{
    Q_OBJECT

public:
    explicit PenJoinStyleComboBox(QWidget *parent = 0);

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
