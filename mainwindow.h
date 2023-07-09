#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadbtn_clicked();

    void on_dispbtn_clicked();

    //void on_comboBox_currentIndexChanged(int index);



    void on_searchbtn_clicked();

    void on_selectbtn_clicked();

    void on_clearbtn_clicked();

    //void on_addbtn_clicked();

    //void on_donebtn_clicked();

    void handleButtonClicked();

    void on_searchblckbtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
