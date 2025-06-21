#include "comminterface.h"

#define SENDTIME
CommInterface::CommInterface(MySerialPort *com_port, int serialProtocol, QObject *parent)
    : QThread(parent), com(com_port), protocol(serialProtocol)
{
   // mutex.Enter();
    QMutexLocker locker(&mutex);
    fifo_buf_i = fifo_buf_ii = 0;
    for( int i = 0; i < FIFO_BUF_DIM; i++ )
    {
        fifo_buf[i].state = FIFO_STATE_EMPTY;
    }

    com = com_port;
    protocol = serialProtocol;
    mutex.Leave();


}
void CommInterface::run()
{
    char state;

    for (;;)
    {
        for (int i = 0; i < FIFO_BUF_DIM; i++)
        {
            mutex.lock();
            state = fifo_buf[fifo_buf_ii].state;
            mutex.unlock();

            if (state == FIFO_STATE_RTS)
                break;

            fifo_buf_ii++;
            if (fifo_buf_ii == FIFO_BUF_DIM)
                fifo_buf_ii = 0;
        }

        if (state == FIFO_STATE_RTS)
        {
            mutex.lock();

            com->DiscardBuffer();

            if (com->MyWrite(fifo_buf[fifo_buf_ii].io_buf, fifo_buf[fifo_buf_ii].io_bytes) == SERIAL_OK)
            {
                if (fifo_buf[fifo_buf_ii].io_buf[0] == 0)
                    fifo_buf[fifo_buf_ii].state = FIFO_STATE_DONE;
                else
                {
                    if (protocol == PROTOCOL_TWS)
                    {
                        if (com->MyRead(fifo_buf[fifo_buf_ii].io_buf, 4) == SERIAL_OK)
                        {
                            short len = 256 * fifo_buf[fifo_buf_ii].io_buf[2] + fifo_buf[fifo_buf_ii].io_buf[3];
                            if (len >= 0 && len <= IO_BUF_DIM - 6)
                            {
                                if (com->MyRead(fifo_buf[fifo_buf_ii].io_buf + 4, len + 2) == SERIAL_OK)
                                {
                                    fifo_buf[fifo_buf_ii].io_bytes = len + 6;
                                    fifo_buf[fifo_buf_ii].state = FIFO_STATE_DONE;
                                }
                            }
                        }
                    }
                    else if (protocol == PROTOCOL_EVER)
                    {
                        if (com->MyRead(fifo_buf[fifo_buf_ii].io_buf, fifo_buf[fifo_buf_ii].expected_in_bytes) == SERIAL_OK)
                        {
                            fifo_buf[fifo_buf_ii].io_bytes = fifo_buf[fifo_buf_ii].expected_in_bytes;
                            fifo_buf[fifo_buf_ii].state = FIFO_STATE_DONE;
                        }
                    }
                }
            }

            if (fifo_buf[fifo_buf_ii].state != FIFO_STATE_DONE)
                fifo_buf[fifo_buf_ii].state = FIFO_STATE_ERROR;

            mutex.unlock();
        }

        if (QThread::currentThread()->isInterruptionRequested())
            return;

        msleep(SENDTIME);
    }
}


int CommInterface::Send( const unsigned char *sx_buf, unsigned short sx_bytes, unsigned short expected_bytes )
{
    int ret_val;
    bool isFull = true;

    QMutexLocker locker(&mutex);

    for (int i = 0; i < FIFO_BUF_DIM; i++)
    {
        if (fifo_buf[fifo_buf_i].state == FIFO_STATE_EMPTY)
        {
            isFull = false;
            break;
        }

        fifo_buf_i++;
        if (fifo_buf_i == FIFO_BUF_DIM)
            fifo_buf_i = 0;
    }
    if (isFull)
    {
        return -1;
    }

    memcpy(fifo_buf[fifo_buf_i].io_buf, sx_buf, sx_bytes);
    fifo_buf[fifo_buf_i].io_bytes = sx_bytes;
    fifo_buf[fifo_buf_i].expected_in_bytes = expected_bytes;
    fifo_buf[fifo_buf_i].state = FIFO_STATE_RTS;

    ret_val = fifo_buf_i;

    fifo_buf_i++;
    if (fifo_buf_i == FIFO_BUF_DIM)
        fifo_buf_i = 0;

    return ret_val;
}

int CommInterface::Receive( int slot, unsigned char *rx_buf, unsigned short &rx_bytes )
{
    int ret_val;
    QMutexLocker locker(&mutex);
    ret_val = fifo_buf[slot].state;

    if (ret_val == FIFO_STATE_DONE || ret_val == FIFO_STATE_ERROR)
    {
        memcpy(rx_buf, fifo_buf[slot].io_buf, fifo_buf[slot].io_bytes);
        rx_bytes = fifo_buf[slot].io_bytes;
        fifo_buf[slot].state = FIFO_STATE_EMPTY;
    }
    return ret_val;
}
