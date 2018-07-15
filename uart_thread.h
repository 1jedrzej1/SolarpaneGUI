#ifndef UART_THREAD_H
#define UART_THREAD_H

#include <QThread>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QMutex>
#include <QMutexLocker>

#include "UARTdef.h"
#include <termios.h>
#include <qdebug.h>
#include <unistd.h>
#include <fcntl.h>

#define FILE_PATH "/home/pi/SolarpaneGUIv4/SolarpaneGUI"
#define FILE_HEADER "IIn12V \t\t IChargeAcu \t\t IDischargeAcu \t\t VChargeAcu \t\t VDischargeAcu"
#define SAVE_TO_FILE_ITER 60

class UART_thread : public QThread
{
    Q_OBJECT
public:
    explicit UART_thread(QObject *parent = 0);

    int uart0_filestream;
    char data_recieve[6];

    int init_connection;
    bool init_error;
    int mode;

    QFile file;
    QMutex mutex;
    unsigned long temp_IIn12V;
    unsigned long temp_IChargeAcu;
    unsigned long temp_IDischargeAcu;
    unsigned long temp_VChargeAcu;
    unsigned long temp_VDischargeAcu;

    QString IIn12V;
    QString IChargeAcu;
    QString IDischargeAcu;
    QString VChargeAcu;
    QString VDischargeAcu;
    int ActuatorPosition;
    int EnginePosition;
    int FotoR_Right;
    int FotoR_Left;
    int FotoR_Down;

    void run();
    void CreateHistoryFile();
    void UART_Init();
    void UART_Transmit(char* data_transmit);
    void UART_Receive();

    void InitConnection();
    bool CheckFrame(char* data_recieve);
    //QString Wartosc_Pradu(char* data_recieve);
    //QString Wartosc_Napiecia(char* data_recieve);


    char FramePar(char* tab);
    int FrameValue(char* tab);
    void FrameInterpret(char* data_recieve);

    void GetCurrentIn12V();
    void GetCurrentChargeAcu();
    void GetCurrentDischargeAcu();
    void GetTensionChargeAcu();
    void GetTensionDischargeAcu();

    void SetManualMode();
    void SetAutoMode();

    void GetActuatorPosition();
    //int ConvToActuatorPosition(char* data_recieve);
    void MoveActuator();

    void GetEnginePosition();
    //int ConvToEnginePosition(char* data_recieve);
    void EngineLeft();
    void EngineRight();
    void ActuatorUp();
    void ActuatorDown();
    void ActuatorStop();

    void GetFotoR_Right();
    void GetFotoR_Left();
    void GetFotoR_Down();

    void UpdateGUI();
    void AddTempValue();
    void UpdateHistoryFile();

public slots:
    //void onSliderValueSend(int);
    
private:

signals:
    void UpGUI();
    void MessageSend(char*);
    void InitConnectionSend(int);

    /*
    void IIn12VSend(QString);
    void IChargeAcuSend(QString);
    void IDischargeAcuSend(QString);
    void VChargeAcuSend(QString);
    void VDischargeAcuSend(QString);
    */

public slots:
    
};

#endif // UART_THREAD_H
