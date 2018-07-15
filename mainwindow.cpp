#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    UART_T = new UART_thread(this);
    SetDateAndT = new SetDateAndTime(this);
    //UART_T->
    OpenWindowDateAndTime();
    //this->showMaximized();
    lcdNumberColor();
    ui->label->hide();
    ui->lcdNumber->hide();
    ui->groupBox->hide();
    ui->groupFoto->hide();
    //ui->pushButton_EngineRight->hide();
    ui->pushButton_EngineLeft->hide();
    ui->pushButton_ActuatorUp->hide();
    ui->pushButton_ActuatorDown->hide();
    //ui->label_10->hide();
    //ui->label_11->hide();
    //ui->label_FotoR->hide();
    ui->label_FotoR_H->hide();
    ui->label_FotoR_V->hide();
    ui->lcdNumber_FotoR_Down->hide();
    ui->lcdNumber_FotoR_Right->hide();
    ui->lcdNumber_FotoR_Left->hide();

    //ui->
    /*
    ui->label_12->hide();
    ui->label_13->hide();
    ui->lineEdit_SliderValue->hide();
    ui->verticalSlider->hide();
    ui->progressBar->hide();
    ui->progressBar->setValue(ui->verticalSlider->value());
    */
    ui->widget->hide();

    ui->pushButton_Disconnect->setDisabled(true);
    ui->pushButton_AutoMode->setDisabled(true);
    ui->pushButton_ManualMode->setDisabled(true);

    connect(UART_T, SIGNAL(UpGUI()), this, SLOT(onUpGUI()));
    connect(UART_T, SIGNAL(MessageSend(char*)), this, SLOT(onMessageSend(char*)));
    connect(UART_T, SIGNAL(InitConnectionSend(int)), this, SLOT(onInitConnectionSend(int)));

    //connect(this, SIGNAL(SliderValueSend(int)), UART_T, SLOT(onSliderValueSend(int)));
    /*
    connect(UART_T, SIGNAL(IChargeAcuSend(QString)), this, SLOT(onIChargeAcuSend(QString)));
    connect(UART_T, SIGNAL(IDischargeAcuSend(QString)), this, SLOT(onIDischargeAcuSend(QString)));
    connect(UART_T, SIGNAL(VChargeAcuSend(QString)), this, SLOT(onVChargeAcuSend(QString)));
    connect(UART_T, SIGNAL(VDischargeAcuSend(QString)), this, SLOT(onVDischargeAcuSend(QString)));
    connect(UART_T, SIGNAL(IIn12VSend(QString)), this, SLOT(onIIn12VSend(QString)));
    */
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddTextToTextEdit(char*text)
{
    textE = ui->textEdit->toPlainText();

    QTextCursor c = ui->textEdit->textCursor();
    int max = 600;
    if(ui->textEdit->toPlainText().length() > max)
    {
        textE = textE.right(max);
        ui->textEdit->setText(textE + "\n" + text);
    }
    else
    {
        ui->textEdit->setText(textE + "\n" + text);
    }
    c.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(c);
}

void MainWindow::OpenWindowDateAndTime()
{
    SetDateAndT->exec();
}

void MainWindow::onMessageSend(char* msg)
{
    ui->label->setText(msg);
    AddTextToTextEdit(msg);
}

void MainWindow::onInitConnectionSend(int init_connection)
{
    if(init_connection == 1)
    {
        AddTextToTextEdit((char*)"Polaczenie nawiazane");
        UART_T->init_connection = 0;
    }
    else if (init_connection == 2)
    {
        AddTextToTextEdit((char*)"Ponawianie polaczenia");
        UART_T->init_connection = 0;
    }
    else if (UART_T->init_error == true)
    {
        AddTextToTextEdit((char*)"Blad polaczenia");
        UART_T->init_connection = 0;
        UART_T->terminate();
    }
}
/*
void MainWindow::onIIn12VSend(QString IIn12V)
{
    ui->lineEdit_6->setText(IIn12V);
}
void MainWindow::onIChargeAcuSend(QString IChargeAcu)
{
    ui->lineEdit_2->setText(IChargeAcu);
}
void MainWindow::onIDischargeAcuSend(QString IDischargeAcu)
{
    ui->lineEdit_3->setText(IDischargeAcu);
}
void MainWindow::onVChargeAcuSend(QString VChargeAcu)
{
    ui->lineEdit_4->setText(VChargeAcu);
}
void MainWindow::onVDischargeAcuSend(QString VDischargeAcu)
{
    ui->lineEdit_5->setText(VDischargeAcu);
}
*/
void MainWindow::lcdNumberColor()
{
    ui->lcdNumber_2->setPalette(Qt::black);
    ui->lcdNumber_3->setPalette(Qt::black);
    ui->lcdNumber_4->setPalette(Qt::black);
    ui->lcdNumber_5->setPalette(Qt::black);
    ui->lcdNumber_6->setPalette(Qt::black);
    ui->lcdNumber_7->setPalette(Qt::black);
    ui->lcdNumber_8->setPalette(Qt::black);
    ui->lcdNumber_FotoR_Right->setPalette(Qt::black);
    ui->lcdNumber_FotoR_Left->setPalette(Qt::black);
    ui->lcdNumber_FotoR_Down->setPalette(Qt::black);
}

void MainWindow::onUpGUI()
{
    ui->lcdNumber_2->display(UART_T->IChargeAcu.toInt());
    ui->lcdNumber_3->display(UART_T->IDischargeAcu.toInt());
    ui->lcdNumber_4->display(UART_T->VChargeAcu.toInt());
    ui->lcdNumber_5->display(UART_T->VDischargeAcu.toInt());
    ui->lcdNumber_6->display(UART_T->IIn12V.toInt());
    ui->lcdNumber_7->display(UART_T->ActuatorPosition);
    ui->lcdNumber_8->display(UART_T->EnginePosition);
    ui->lcdNumber_FotoR_Right->display(UART_T->FotoR_Right);
    ui->lcdNumber_FotoR_Left->display(UART_T->FotoR_Left);
    ui->lcdNumber_FotoR_Down->display(UART_T->FotoR_Down);

    if(UART_T->mode == 2)
    {
        /*
        QString PBvalue = QString::number(ui->progressBar->value());
        ui->lineEdit_SliderValue->setText(PBvalue);
        emit SliderValueSend(ui->progressBar->value());
        */
    }
}

void MainWindow::on_pushButton_ConnectAutoMode_clicked()
{
    UART_T->mode = 1;

    ui->groupBox->hide();
    ui->groupFoto->hide();
    ui->pushButton_ConnectManualMode->setDisabled(true);
    ui->pushButton_ConnectAutoMode->setDisabled(true);
    ui->pushButton_AutoMode->setDisabled(true);
    ui->pushButton_ManualMode->setDisabled(false);
    ui->pushButton_Disconnect->setDisabled(false);
    ui->widget->show();
    //ui->label_FotoR->hide();
    ui->label_FotoR_H->hide();
    ui->label_FotoR_V->hide();
    ui->lcdNumber_FotoR_Down->hide();
    ui->lcdNumber_FotoR_Right->hide();
    ui->lcdNumber_FotoR_Left->hide();

    ui->textEdit->setText((char*)"Inicjalizajca polaczenia w trybie AUTO");
    UART_T->start(QThread::HighestPriority);
}

void MainWindow::on_pushButton_ConnectManualMode_clicked()
{
    UART_T->mode = 2;

    ui->groupBox->show();
    ui->groupFoto->show();
    ui->pushButton_EngineRight->show();
    ui->pushButton_EngineLeft->show();
    ui->pushButton_ActuatorUp->show();
    ui->pushButton_ActuatorDown->show();
    //ui->label_10->show();
    //ui->label_11->show();
    //ui->label_FotoR->show();
    ui->label_FotoR_H->show();
    ui->label_FotoR_V->show();
    ui->lcdNumber_FotoR_Down->show();
    ui->lcdNumber_FotoR_Right->show();
    ui->lcdNumber_FotoR_Left->show();
    /*
    ui->label_12->show();
    ui->label_13->show();
    ui->lineEdit_SliderValue->show();
    ui->verticalSlider->show();
    ui->progressBar->show();
    */
    ui->widget->hide();

    ui->pushButton_ConnectManualMode->setDisabled(true);
    ui->pushButton_ConnectAutoMode->setDisabled(true);
    ui->pushButton_AutoMode->setDisabled(false);
    ui->pushButton_ManualMode->setDisabled(true);
    ui->pushButton_Disconnect->setDisabled(false);

    ui->textEdit->setText((char*)"Inicjalizajca polaczenia w trybie MANUAL");
    UART_T->start(QThread::HighestPriority);
}

void MainWindow::on_pushButton_Disconnect_clicked()
{
    UART_T->mode = 0;

    ui->groupBox->hide();
    ui->groupFoto->hide();
    ui->pushButton_EngineRight->hide();
    ui->pushButton_EngineLeft->hide();
    ui->pushButton_ActuatorUp->hide();
    ui->pushButton_ActuatorDown->hide();
    //ui->label_10->hide();
    //ui->label_11->hide();
    //ui->label_FotoR->hide();
    ui->label_FotoR_H->hide();
    ui->label_FotoR_V->hide();
    ui->lcdNumber_FotoR_Down->hide();
    ui->lcdNumber_FotoR_Right->hide();
    ui->lcdNumber_FotoR_Left->hide();
    /*
    ui->label_12->hide();
    ui->label_13->hide();
    ui->lineEdit_SliderValue->hide();
    ui->verticalSlider->hide();
    ui->progressBar->hide();
    */
    ui->widget->hide();

    ui->pushButton_ConnectAutoMode->setDisabled(false);
    ui->pushButton_ConnectManualMode->setDisabled(false);
    ui->pushButton_Disconnect->setDisabled(true);
    ui->pushButton_AutoMode->setDisabled(true);
    ui->pushButton_ManualMode->setDisabled(true);

    UART_T->terminate();
    AddTextToTextEdit((char*)"Zakonczenie polaczenia");
    UART_T->IIn12V = "0";
    UART_T->IChargeAcu = "0";
    UART_T->IDischargeAcu = "0";
    UART_T->VChargeAcu = "0";
    UART_T->VDischargeAcu = "0";
    UART_T->ActuatorPosition = 0;
    UART_T->EnginePosition = 0;
    UART_T->FotoR_Right = 0;
    UART_T->FotoR_Left = 0;
    UART_T->FotoR_Down = 0;
    onUpGUI();
}

void MainWindow::on_pushButton_AutoMode_clicked()
{
    UART_T->mode = 1;
    UART_T->SetAutoMode();

    ui->groupBox->hide();
    ui->groupFoto->hide();
    ui->pushButton_EngineRight->hide();
    ui->pushButton_EngineLeft->hide();
    ui->pushButton_ActuatorUp->hide();
    ui->pushButton_ActuatorDown->hide();
    //ui->label_10->hide();
    //ui->label_11->hide();
    //ui->label_FotoR->hide();
    ui->label_FotoR_H->hide();
    ui->label_FotoR_V->hide();
    ui->lcdNumber_FotoR_Down->hide();
    ui->lcdNumber_FotoR_Right->hide();
    ui->lcdNumber_FotoR_Left->hide();
    /*
    ui->label_12->hide();
    ui->label_13->hide();
    ui->lineEdit_SliderValue->hide();
    ui->verticalSlider->hide();
    ui->progressBar->hide();
    */
    ui->widget->show();

    ui->pushButton_ConnectManualMode->setDisabled(true);
    ui->pushButton_ConnectAutoMode->setDisabled(true);
    ui->pushButton_AutoMode->setDisabled(true);
    ui->pushButton_ManualMode->setDisabled(false);
    ui->pushButton_Disconnect->setDisabled(false);
}

void MainWindow::on_pushButton_ManualMode_clicked()
{
    UART_T->mode = 2;
    UART_T->SetManualMode();

    ui->label->hide();
    ui->lcdNumber->hide();
    ui->groupBox->show();
    ui->groupFoto->show();
    ui->pushButton_EngineRight->show();
    ui->pushButton_EngineLeft->show();
    ui->pushButton_ActuatorUp->show();
    ui->pushButton_ActuatorDown->show();
    //ui->label_10->show();
    //ui->label_11->show();
    //ui->label_FotoR->show();
    ui->label_FotoR_H->show();
    ui->label_FotoR_V->show();
    ui->lcdNumber_FotoR_Down->show();
    ui->lcdNumber_FotoR_Right->show();
    ui->lcdNumber_FotoR_Left->show();

    /*
    ui->label_12->show();
    ui->label_13->show();
    ui->lineEdit_SliderValue->show();
    ui->verticalSlider->show();
    ui->progressBar->show();
    */
    ui->widget->hide();

    ui->pushButton_ConnectManualMode->setDisabled(true);
    ui->pushButton_ConnectAutoMode->setDisabled(true);
    ui->pushButton_AutoMode->setDisabled(false);
    ui->pushButton_ManualMode->setDisabled(true);
    ui->pushButton_Disconnect->setDisabled(false);
}




void MainWindow::on_pushButton_EngineRight_pressed()
{
    UART_T->EngineRight();
}

void MainWindow::on_pushButton_EngineLeft_pressed()
{
    UART_T->EngineLeft();
}

void MainWindow::on_pushButton_ActuatorUp_pressed()
{
    UART_T->ActuatorUp();
}
void MainWindow::on_pushButton_ActuatorDown_pressed()
{
    UART_T->ActuatorDown();
}

void MainWindow::on_pushButton_ActuatorUp_released()
{
    UART_T->ActuatorStop();
}

void MainWindow::on_pushButton_ActuatorDown_released()
{
    UART_T->ActuatorStop();
}
