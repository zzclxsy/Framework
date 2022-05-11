#include "XRingBuffer.h"
#include "./XLogPrint/XLogPrint.h"
XRingBuffer::XRingBuffer()
{
    m_buffer = nullptr;
    m_size = 0;
    m_wpos = 0;
    m_rpos = 0;
}

XRingBuffer::~XRingBuffer()
{
    if (m_buffer)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
}

void XRingBuffer::SetBufferSize(int size)
{
    m_dataList.clear();
    if (m_buffer)
    {
        delete[] m_buffer;
        m_buffer = nullptr;
    }
    m_buffer = new unsigned char[size];
    m_size = size;
    m_wpos = 0;
    m_rpos = 0;
}

int XRingBuffer::Write(const char *data, int length)
{
    int wpos = -1;
    if (m_wpos >= m_rpos)
    {
        if (m_wpos + length <= m_size)
        {
            wpos = m_wpos;
        }
        else if (m_rpos > length)
        {
            wpos = 0;
        }
    }
    else if (m_wpos + length < m_rpos)
    {
        wpos = m_wpos;
    }

    if (wpos >= 0)
    {
        XBuffer buffer(&m_buffer[wpos], length);
        memcpy(buffer.Data(), data, length);

        m_lock.lock();
        m_dataList.push_back(buffer);
        m_wpos = wpos + length;
        m_lock.unlock();

        return 0;
    }
    //XERROR << "JHRingBuffer::Push, buffer is full.";
    return -1;
}

XBuffer XRingBuffer::Read()
{
    if (m_dataList.empty())
    {
        return XBuffer();
    }

    m_lock.lock();
    XBuffer buffer = m_dataList.front();
    m_dataList.pop_front();
    m_rpos = (int)(buffer.Data() - m_buffer);
    m_lock.unlock();

    return buffer;
}
