#ifndef BRUSHSETTINGSWIDGET_H
#define BRUSHSETTINGSWIDGET_H

#include "utils/utils_global.h"

#include <QWidget>
#include <QBrush>

class BrushStyleComboBox;
class PenColorComboBox;

class BrushSettingsWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QBrush brush READ brush WRITE setBrush NOTIFY brushChanged)

public:
    explicit BrushSettingsWidget(QWidget *parent = 0);

    QBrush brush() const;

signals:
    void brushChanged(const QBrush &brush);

public slots:
    void setBrush(const QBrush &brush);

private:
    QBrush m_brush;
    PenColorComboBox *m_colorComboBox;
    BrushStyleComboBox *m_styleComboBox;

};


#endif // BRUSHSETTINGSWIDGET_H
