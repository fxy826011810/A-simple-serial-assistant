#include "dialog.h"
#include "ui_dialog.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include "hardware.h"
#include <QTimer>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    Dialog::serialportinit();

}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::serialportinit()
{
    port=new QSerialPort(this);
    driver=new hardware(port);
    resetPort();
    time1=new QTimer(this);
    time2=new QTimer(this);

    time1->setInterval(50);
    time2->setInterval(20);
    connect(time1,&QTimer::timeout,this,&Dialog::portScan);
//    connect(time2,&QTimer::timeout,this,&Dialog::port_update);
    connect(port,&QSerialPort::readyRead,this,&Dialog::port_update);

    time1->start();
    time2->start();
}

void Dialog::resetPort()
{
   driver->init();
   QString BaudRate[]={"9600","38400","100000","115200","128000","921600"};
   QString stopBits[]={"1","1.5","2"};
   QString dataBits[]={"8","7","6","5"};
   QString parity[]={"无","奇校验","偶校验"};
   QStringList comlist,brlist,sblist,pylist,dblist;
   for(uint i=0;i<(sizeof(BaudRate)/sizeof(QString));i++)
   {
       brlist.append(BaudRate[i]);
   }
   for(uint i=0;i<(sizeof(stopBits)/sizeof(QString));i++)
   {
       sblist.append(stopBits[i]);
   }
   for(uint i=0;i<(sizeof(parity)/sizeof(QString));i++)
   {
       pylist.append(parity[i]);
   }
   for(uint i=0;i<(sizeof(dataBits)/sizeof(QString));i++)
   {
       dblist.append(dataBits[i]);
   }
   ui->boudBox->clear();
   ui->stopBox->clear();
   ui->parityBox->clear();
   ui->databitBox->clear();

    ui->boudBox->addItems(brlist);
    ui->stopBox->addItems(sblist);
    ui->parityBox->addItems(pylist);
    ui->databitBox->addItems(dblist);
}
void Dialog::portChooseDisable(bool b)
{
    ui->boudBox->setDisabled(b);
    ui->comBox->setDisabled(b);
    ui->databitBox->setDisabled(b);
    ui->parityBox->setDisabled(b);
    ui->stopBox->setDisabled(b);
    ui->openButton->setDisabled(b);
}
void Dialog::openPort()
{
    setPortName();
    setBaudRate();
    setDataBits();
    setStopBits();
    setParity();

    port->open(QIODevice::ReadWrite);
    if(port->isOpen())
    {
        portChooseDisable(1);
        ui->comlabel->setText(port->portName().append(" open"));
        ui->boudlabel->setNum((double)port->baudRate());
    }
    else
    {
        ui->comlabel->setText(port->portName().append(" open faild"));
        ui->boudlabel->clear();

    }

}
void Dialog::closePort()
{
    port->close();
    if(port->isOpen())
    {
        ui->comlabel->setText(port->portName().append(" close faild"));
    }
    else
    {
        portChooseDisable(0);
        ui->comlabel->setText(port->portName().append(" close"));
        ui->boudlabel->clear();
    }
}

void Dialog::setPortName()
{
port->setPortName(ui->comBox->currentText());
}
void Dialog::setBaudRate()
{
    int BaudRate=ui->boudBox->currentText().toInt();
    qDebug("%d",BaudRate);
    port->setBaudRate(BaudRate);
}
void Dialog::setDataBits()
{
    switch(ui->databitBox->currentText().toInt())
    {
    case 8:port->setDataBits(QSerialPort::Data8);
        break;
    case 7:port->setDataBits(QSerialPort::Data7);
        break;
    case 6:port->setDataBits(QSerialPort::Data6);
        break;
    case 5:port->setDataBits(QSerialPort::Data5);
        break;
    }
}
void Dialog::setStopBits()
{
    if(ui->stopBox->currentText().toFloat()==1)
    {
        port->setStopBits(QSerialPort::OneStop);
    }
    else if(ui->stopBox->currentText().toFloat()==1.5)
    {
        port->setStopBits(QSerialPort::OneAndHalfStop);
    }
    else if(ui->stopBox->currentText().toFloat()==2)
    {
        port->setStopBits(QSerialPort::TwoStop);
    }

}
void Dialog::setParity()
{
    if(ui->parityBox->currentText()=="无")
    {
        port->setParity(QSerialPort::NoParity);
    }
    else if(ui->parityBox->currentText()=="奇校验")
    {
        port->setParity(QSerialPort::NoParity);
    }
    else if(ui->parityBox->currentText()=="偶校验")
    {
        port->setParity(QSerialPort::NoParity);
    }
}


void Dialog::portScan()
{

    ports=QSerialPortInfo::availablePorts();
    if(ports.length()!=lastPorts.length())
    {
        ui->comBox->clear();
        comlist.clear();
        for(int i=0;i<ports.length();i++)
        {
            comlist.append(ports[i].portName());
        }
        ui->comBox->addItems(comlist);
   }
    lastPorts=ports;
}
void Dialog::port_update()
{
    if(port->isOpen())
    {
        QString temp=driver->receive();
        ui->recTextBrowser->append(temp);

    }
}




void Dialog::on_closeButton_clicked()
{
    closePort();
}
void Dialog::on_openButton_clicked()
{
    if(port->isOpen()==true)
    {
     port->close();
    }
    if(port->isOpen()==false)
    openPort();
}
void Dialog::on_recClearButton_clicked()
{
    ui->recTextBrowser->clear();
}


void Dialog::timerEvent(QTimerEvent *event)
{
static int time=0;
    time++;
    if(time==100)
    {
//      portScan();
      time=0;
    }
}

