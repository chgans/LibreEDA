#ifndef PENSETTINGSWIDGET_H
#define PENSETTINGSWIDGET_H

#include "utils/utils_global.h"

#include <QWidget>
#include <QPen>

class PenWidthComboBox;
class PenStyleComboBox;
class PenCapStyleComboBox;
class PenJoinStyleComboBox;

class UTILS_EXPORT PenSettingsWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QPen pen READ pen WRITE setPen NOTIFY penChanged)

public:
    explicit PenSettingsWidget(QWidget *parent = 0);

    QPen pen() const;

signals:
    void penChanged(const QPen &pen);

public slots:
    void setPen(const QPen &pen);

private:
    QPen m_pen;
    PenWidthComboBox *m_widthComboBox;
    PenStyleComboBox *m_styleComboBox;
    PenCapStyleComboBox *m_capStyleComboBox;
    PenJoinStyleComboBox *m_joinStyleComboBox;
};

#endif // PENSETTINGSWIDGET_H
