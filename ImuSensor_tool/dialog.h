#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include "hardware.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
public:
    void port_update();
    void portScan();
    void portChooseDisable(bool b);
    void serialportinit();
    void resetPort();
    void openPort();
    void closePort();
    void setPortName();
    void setBaudRate();
    void setDataBits();
    void setParity();
    void setStopBits();
    Ui::Dialog *ui;
private slots:

    void on_closeButton_clicked();


    void on_openButton_clicked();

    void on_recClearButton_clicked();
     void timerEvent(QTimerEvent *event);
private:
    QList<QSerialPortInfo> ports,lastPorts;
    QStringList comlist;
    QTimer *time1;
    QTimer *time2;

    QSerialPort *port;
    hardware *driver;
};

#endif // DIALOG_H
