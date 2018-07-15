#include "setdateandtime.h"
#include "ui_setdateandtime.h"
#include <QDebug>
#include <QProcess>

SetDateAndTime::SetDateAndTime(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDateAndTime)
{
    ui->setupUi(this);
    date_RRRR = ui->lineEdit_1->text();
    date_MM = ui->lineEdit_2->text();
    date_DD = ui->lineEdit_3->text();
    time_HH = ui->lineEdit_4->text();
    time_MM = ui->lineEdit_5->text();
    time_SS = ui->lineEdit_6->text();
}

SetDateAndTime::~SetDateAndTime()
{
    delete ui;
}

void SetDateAndTime::on_pushButton_1_clicked()
{
    int temp;
    date_RRRR = ui->lineEdit_1->text();
    temp = date_RRRR.toInt();
    if (temp < 0) temp = 0;
    temp ++;
    date_RRRR = QString::number(temp);
    ui->lineEdit_1->setText(date_RRRR);
}

void SetDateAndTime::on_pushButton_2_clicked()
{
    int temp;
    date_RRRR = ui->lineEdit_1->text();
    temp = date_RRRR.toInt();
    if (temp < 0) temp = 0;
    temp --;
    date_RRRR = QString::number(temp);
    ui->lineEdit_1->setText(date_RRRR);
}

void SetDateAndTime::on_pushButton_3_clicked()
{
    int temp;
    date_MM = ui->lineEdit_2->text();
    temp = date_MM.toInt();
    temp ++;
    if (temp > 12) temp = 1;
    date_MM = QString::number(temp);
    ui->lineEdit_2->setText(date_MM);
}

void SetDateAndTime::on_pushButton_4_clicked()
{
    int temp;
    date_MM = ui->lineEdit_2->text();
    temp = date_MM.toInt();
    temp --;
    if (temp < 1) temp = 12;
    date_MM = QString::number(temp);
    ui->lineEdit_2->setText(date_MM);
}

void SetDateAndTime::on_pushButton_5_clicked()
{
    int temp;
    date_DD = ui->lineEdit_3->text();
    temp = date_DD.toInt();
    temp ++;
    if (temp > 31) temp = 1;
    date_DD = QString::number(temp);
    ui->lineEdit_3->setText(date_DD);
}

void SetDateAndTime::on_pushButton_6_clicked()
{
    int temp;
    date_DD = ui->lineEdit_3->text();
    temp = date_DD.toInt();
    temp --;
    if (temp < 1) temp = 31;
    date_DD = QString::number(temp);
    ui->lineEdit_3->setText(date_DD);
}

void SetDateAndTime::on_pushButton_7_clicked()
{
    int temp;
    time_HH = ui->lineEdit_4->text();
    temp = time_HH.toInt();
    temp ++;
    if (temp > 23) temp = 0;
    time_HH = QString::number(temp);
    ui->lineEdit_4->setText(time_HH);
}

void SetDateAndTime::on_pushButton_8_clicked()
{
    int temp;
    time_HH = ui->lineEdit_4->text();
    temp = time_HH.toInt();
    temp --;
    if (temp < 0) temp = 23;
    time_HH = QString::number(temp);
    ui->lineEdit_4->setText(time_HH);
}

void SetDateAndTime::on_pushButton_9_clicked()
{
    int temp;
    time_MM = ui->lineEdit_5->text();
    temp = time_MM.toInt();
    temp ++;
    if (temp > 59) temp = 0;
    time_MM = QString::number(temp);
    ui->lineEdit_5->setText(time_MM);
}

void SetDateAndTime::on_pushButton_10_clicked()
{
    int temp;
    time_MM = ui->lineEdit_5->text();
    temp = time_MM.toInt();
    temp --;
    if (temp < 0) temp = 59;
    time_MM = QString::number(temp);
    ui->lineEdit_5->setText(time_MM);
}

void SetDateAndTime::on_pushButton_11_clicked()
{
    int temp;
    time_SS = ui->lineEdit_6->text();
    temp = time_SS.toInt();
    temp ++;
    if (temp > 59) temp = 0;
    time_SS = QString::number(temp);
    ui->lineEdit_6->setText(time_SS);
}

void SetDateAndTime::on_pushButton_12_clicked()
{
    int temp;
    time_SS = ui->lineEdit_6->text();
    temp = time_SS.toInt();
    temp --;
    if (temp < 0) temp = 59;
    time_SS = QString::number(temp);
    ui->lineEdit_6->setText(time_SS);
}

void SetDateAndTime::convertToCommandLine()
{
    if(date_MM == "1") date_MM = "JAN";
    if(date_MM == "2") date_MM = "FEB";
    if(date_MM == "3") date_MM = "MAR";
    if(date_MM == "4") date_MM = "APR";
    if(date_MM == "5") date_MM = "MAY";
    if(date_MM == "6") date_MM = "JUN";
    if(date_MM == "7") date_MM = "JUL";
    if(date_MM == "8") date_MM = "AUG";
    if(date_MM == "9") date_MM = "SEP";
    if(date_MM == "10") date_MM = "OCT";
    if(date_MM == "11") date_MM = "NOV";
    if(date_MM == "12") date_MM = "DEC";
    if(time_HH.toInt() < 10) time_HH = "0" + time_HH;
    if(time_MM.toInt() < 10) time_MM = "0" + time_MM;
    if(time_SS.toInt() < 10) time_SS = "0" + time_SS;
}

void SetDateAndTime::on_pushButton_clicked()
{
    convertToCommandLine();
    DandT_CommandLine = "sudo date -s \"" + date_DD + " " + date_MM + " " + date_RRRR + " " + time_HH + ":" + time_MM + ":" + time_SS + "\"";
    QByteArray ba = DandT_CommandLine.toLocal8Bit();
    const char *D_T_CommandLine = ba.data();
    system(D_T_CommandLine);
    this->close();
}
