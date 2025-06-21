#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#define SERIAL_ERROR		-1
#define SERIAL_OK			0

#define BUFFER_MAX_LEN		512

#include <QSerialPort>

class MySerialPort : public QSerialPort
{
public:
    MySerialPort(QObject *parent = nullptr);
    ~MySerialPort();

    int myOpen(const QString &portName, QSerialPort::BaudRate baudRate, bool halfDuplex);
    int myRead(unsigned char *buf, unsigned int len);
    int myWrite(unsigned char *buf, unsigned int len);
    void discardBuffer();

    void setTimeout(int ms) { timeout_ms = ms; }

private:
    int timeout_ms;
    bool timeout_flag;
    bool is_half_duplex;
};

#endif // MYSERIALPORT_H
