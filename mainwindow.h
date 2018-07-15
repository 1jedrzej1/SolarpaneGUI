#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QSlider>
#include <QPixmap>
#include "uart_thread.h"
#include "setdateandtime.h"
#include "qcustomplot.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    UART_thread *UART_T;
    SetDateAndTime *SetDateAndT;
    QString textE;

    void AddTextToTextEdit(char* text);
    void OpenWindowDateAndTime();
    void lcdNumberColor();
private:
    Ui::MainWindow *ui;


signals:
    //void SliderValueSend(int);

public slots:
    void onUpGUI();
    void onMessageSend(char*);
    void onInitConnectionSend(int);

    /*
    void onIIn12VSend(QString);
    void onIChargeAcuSend(QString);
    void onIDischargeAcuSend(QString);
    void onVChargeAcuSend(QString);
    void onVDischargeAcuSend(QString);
    */

private slots:

    void on_pushButton_ConnectAutoMode_clicked();
    void on_pushButton_ConnectManualMode_clicked();
    void on_pushButton_Disconnect_clicked();
    void on_pushButton_AutoMode_clicked();
    void on_pushButton_ManualMode_clicked();

    void on_pushButton_EngineRight_pressed();
    void on_pushButton_EngineLeft_pressed();
    void on_pushButton_ActuatorUp_pressed();
    void on_pushButton_ActuatorDown_pressed();

    void on_pushButton_ActuatorUp_released();
    void on_pushButton_ActuatorDown_released();
};

#endif // MAINWINDOW_H
