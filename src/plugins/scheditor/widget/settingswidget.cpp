#include "settingswidget.h"
#include "ui_settingswidget.h"

#include "schscene.h"
#include "grid/graphicscartesiangrid.h"
#include "palette.h"

SettingsWidget::SettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);

    auto scene = new SchScene(this);
    scene->setSceneRect(0, 0, 297, 210);
    auto grid = new GraphicsCartesianGrid();
    scene->setGrid(grid);
    ui->graphicsView->setScene(scene);
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}
