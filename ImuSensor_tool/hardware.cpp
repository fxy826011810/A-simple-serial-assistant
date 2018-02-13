#include "hardware.h"
#include <QDebug>
#include <QMutex>
#include "dialog.h"
#include "ui_dialog.h"
hardware::hardware(QSerialPort *port)
{
    this->port=port;
}

void hardware::init(void)
{
    if(port->isOpen())
        port->close();
}

void hardware::send()
{

}
QString hardware::receive()
{
        signed short anglebuf[3]={0};
        const size_t bufSize = 2048;
        size_t readLen = 0;
        uint8_t buf[bufSize];
        static uint8_t rbuf[8];
        static int debuglen=0,lock=0,j=0,ok=0;

        QString str;

            if(port->bytesAvailable())
            {
                readLen = port->read((char *)buf, bufSize);

                for (size_t i = 0; i < readLen; i++)
                {
                    if(buf[i]==0xaa)
                    {
                        lock=1;
                    }
                    if(lock==1)
                    {
                        rbuf[j++]=buf[i];
                    }
                    if(buf[i]==0xbb)
                    {
                        lock=0;
                        j=0;
                        ok=1;
                    }

                    str.append(" 0x");
                    str.append(QString::number(buf[i], 16));
                }
                if(rbuf[0]==0xAA&&rbuf[7]==0xBB)
                {
                    ok=0;
                    anglebuf[0]=(rbuf[1]<<8|rbuf[2]);
                    anglebuf[1]=(rbuf[3]<<8|rbuf[4]);
                    anglebuf[2]=(rbuf[5]<<8|rbuf[6]);

                    qDebug("%4d   %f %f %f",debuglen++,(float)anglebuf[0]/100,(float)anglebuf[1]/100,(float)anglebuf[2]/100);
//                   emit angleOk(anglebuf);
//                    if((abs(anglebuf[0])<=180&&abs(anglebuf[1])<=180&&abs(anglebuf[2])<=180))
                }
            }

        if (!str.isEmpty())
            return (str);
        return NULL;
}

