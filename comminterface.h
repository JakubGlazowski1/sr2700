#ifndef COMMINTERFACE_H
#define COMMINTERFACE_H

#include "myserialport.h"
#include <QThread>
#include <QMutex>
#define FIFO_BUF_DIM		128
#define IO_BUF_DIM			BUFFER_MAX_LEN

#define FIFO_STATE_ERROR	-1
#define FIFO_STATE_EMPTY	0
#define FIFO_STATE_RTS		1
#define FIFO_STATE_DONE		2


enum SerialProtocolEnum
{
    PROTOCOL_TWS,
    PROTOCOL_EVER
};


class CommInterface : public QThread
{
    Q_OBJECT
public:
    explicit
    CommInterface( MySerialPort *com_port, int serialProtocol, QObject *parent = nullptr );
    void run() override;

    int Send( const unsigned char *buf, unsigned short sx_bytes, unsigned short expected_bytes = 0 );
    int Receive( int slot, unsigned char *rx_buf, unsigned short &rx_bytes );

signals:
private:
    struct CommUnit
    {
        unsigned char io_buf[IO_BUF_DIM];
        short io_bytes;
        short expected_in_bytes;
        int state;
    };
    CommUnit fifo_buf[FIFO_BUF_DIM];
    int fifo_buf_i, fifo_buf_ii;

    QMutex mutex;
    MySerialPort *com;
    int protocol;
};

#endif // COMMINTERFACE_H
