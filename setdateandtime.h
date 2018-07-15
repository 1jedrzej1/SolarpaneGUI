#ifndef SETDATEANDTIME_H
#define SETDATEANDTIME_H

#include <QDialog>
#include <stdlib.h>

namespace Ui {
class SetDateAndTime;
}

class SetDateAndTime : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetDateAndTime(QWidget *parent = 0);
    ~SetDateAndTime();
    QString date_RRRR;
    QString date_MM;
    QString date_DD;
    QString time_HH;
    QString time_MM;
    QString time_SS;

    QString DandT_CommandLine;
    
private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void convertToCommandLine();

    void on_pushButton_clicked();

private:
    Ui::SetDateAndTime *ui;

};

#endif // SETDATEANDTIME_H
