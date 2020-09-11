#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "IQVisualizer/iqvisualizer.h"

namespace Ui {
class MainWindow;
}
/**
 * @brief The MainWindow class
 * Главное окно приложения
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /**
     * @brief setIQVisualizer
     * Установка виджетов визуализации сигналов
     * @param iqVisualizer
     */
    void setIQVisualizer(QVector <IQVisualizer*> iqVisualizer);

private slots:
    void on_exit_action_triggered();
    void on_countChannal_1_action_triggered();
    void on_countChannal_2_action_triggered();
    void on_countChannal_3_action_triggered();
    void on_countChannal_4_action_triggered();

signals:
    /**
     * @brief countChannals_changed
     * Сообщает о необходимости изменения количества каналов
     * @param value
     */
    void countChannals_changed(int value);

private:
    Ui::MainWindow *ui;
    QVector<IQVisualizer*> m_iqVisualizer;
    /**
     * @brief clearIQVisualizer
     * Удаление всех виджетов визуализации сигналов из главного окна
     */
    void clearIQVisualizer();
};

#endif // MAINWINDOW_H
