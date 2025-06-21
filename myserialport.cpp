#include "myserialport.h"
#include <QElapsedTimer>

#define SERIAL_ERROR		-1
#define SERIAL_OK			0

#define BUFFER_MAX_LEN		512

MySerialPort::MySerialPort(QObject *parent)
    : QSerialPort(parent), timeout_ms(100), timeout_flag(false), is_half_duplex(false)

{

}
int MySerialPort::myOpen(const QString &portName, QSerialPort::BaudRate baudRate, bool halfDuplex)
{
    setPortName(portName);
    if (!open(QIODevice::ReadWrite))
        return SERIAL_ERROR;

    if (!setBaudRate(baudRate))
        return SERIAL_ERROR;

    is_half_duplex = halfDuplex;
    return SERIAL_OK;
}


int MySerialPort::myRead(unsigned char *buf, unsigned int len)
{
    unsigned int bytes_to_read = len;
    int total_read = 0;
    timeout_flag = false;
    QElapsedTimer timer;
    timer.start();

    while (!timeout_flag && bytes_to_read > 0)
    {
        qint64 num = read(reinterpret_cast<char*>(buf + total_read), bytes_to_read);
        if (num < 0)
            break;
        if (num == 0) {
            if (waitForReadyRead(10)) // wait a bit for data
                continue;
        } else {
            bytes_to_read -= num;
            total_read += num;
        }
        timeout_flag = timer.elapsed() > static_cast<qint64>(timeout_ms);
    }

    return (bytes_to_read == 0) ? SERIAL_OK : SERIAL_ERROR;
}

int MySerialPort::myWrite(unsigned char *buf, unsigned int len)
{
    unsigned int bytes_to_write = len;
    int total_written = 0;
    timeout_flag = false;
    QElapsedTimer timer;
    timer.start();

    // Write loop with timeout
    while (!timeout_flag && bytes_to_write > 0)
    {
        qint64 num = write(reinterpret_cast<const char*>(buf + total_written), bytes_to_write);
        if (num < 0)
            break;

        if (!waitForBytesWritten(timeout_ms))
            break;

        bytes_to_write -= num;
        total_written += num;

        timeout_flag = timer.elapsed() > static_cast<qint64>(timeout_ms);
    }

    if (bytes_to_write != 0)
        return SERIAL_ERROR;

    // Half-duplex: read back what was sent
    if (is_half_duplex)
    {
        unsigned char read_buf[BUFFER_MAX_LEN];
        unsigned int bytes_to_read = len;
        int total_read = 0;
        timeout_flag = false;
        timer.restart();

        while (!timeout_flag && bytes_to_read > 0)
        {
            qint64 num = read(reinterpret_cast<char*>(read_buf + total_read), bytes_to_read);
            if (num < 0)
                break;
            if (num == 0) {
                if (waitForReadyRead(10))
                    continue;
            } else {
                bytes_to_read -= num;
                total_read += num;
            }
            timeout_flag = timer.elapsed() > static_cast<qint64>(timeout_ms);
        }

        if (bytes_to_read != 0)
            return SERIAL_ERROR;
    }

    return SERIAL_OK;
}

void MySerialPort::discardBuffer()
{
    clear(QSerialPort::Input);
}
