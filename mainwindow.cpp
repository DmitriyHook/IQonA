#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setIQVisualizer(QVector<IQVisualizer *> iqVisualizer)
{
    clearIQVisualizer();
    for(int i = 0; i < iqVisualizer.count(); i++)
    {
        ui->iqVisualizers_gridLayout->addWidget(iqVisualizer.at(i), i/2, i%2);
    }

}

void MainWindow::clearIQVisualizer()
{
    QLayoutItem *layoutItem;
    while ((layoutItem = ui->iqVisualizers_gridLayout->itemAt(0)))
    {
        ui->iqVisualizers_gridLayout->removeItem(layoutItem);
        ui->iqVisualizers_gridLayout->removeWidget(layoutItem->widget());
        delete layoutItem->widget();
        delete layoutItem;
        layoutItem = nullptr;
        ui->iqVisualizers_gridLayout->update();
    }
}

void MainWindow::on_exit_action_triggered()
{
    this->close();
}

void MainWindow::on_countChannal_1_action_triggered()
{
    emit countChannals_changed(1);
}

void MainWindow::on_countChannal_2_action_triggered()
{
    emit countChannals_changed(2);
}

void MainWindow::on_countChannal_3_action_triggered()
{
    emit countChannals_changed(3);
}

void MainWindow::on_countChannal_4_action_triggered()
{
    emit countChannals_changed(4);
}
