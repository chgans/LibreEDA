#include "pcbsettingspage.h"

#include <QWidget>

PcbSettingsPage::PcbSettingsPage(QObject *parent):
    ISettingsPage(parent)
{
    setCategoryName("Board layout");
    setCategoryIcon(QIcon(":/icons/pcb.png"));
    setPageName("Interface");
}

QWidget *PcbSettingsPage::widget()
{
    return new QWidget();
}

void PcbSettingsPage::apply()
{

}

void PcbSettingsPage::finish()
{

}
