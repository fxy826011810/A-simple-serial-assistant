#ifndef HARDWARE_H
#define HARDWARE_H
#include <QSerialPort>
#include <QMutex>
class hardware
{
public:
   hardware(QSerialPort *port);
   void init();
   void send();
   QString receive();
private:
    QSerialPort *port;
    QMutex *sendlock;
signals:


};

#endif // HARDWARE_H
