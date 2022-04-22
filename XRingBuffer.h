#ifndef XRINGBUFFER_H
#define XRINGBUFFER_H

#include <list>
#include <mutex>

class XBuffer
{
protected:
    unsigned char * m_data;
    int m_size;

public:
    XBuffer()
        : m_data(nullptr)
        , m_size()
    {

    }

    XBuffer(unsigned char * p, int length)
        : m_data(p)
        , m_size(length)
    {

    }

    ~XBuffer()
    {

    }

    inline unsigned char * Data(){ return m_data; }
    inline const unsigned char * Data() const { return m_data; }
    inline int Size(){ return m_size; }
    inline bool IsEmpty()const {return m_size == 0; }
};


class XRingBuffer
{
public:
    XRingBuffer();
    ~XRingBuffer();

    void SetBufferSize(int size);

    int Write(const char * data, int length);
    XBuffer Read();

    inline bool IsEmpty(){ return m_dataList.empty(); }
//    inline int FreeSpace(){ return 0; }

protected:
    unsigned char * m_buffer;
    int m_size;

    int m_wpos;
    int m_rpos;

    std::list<XBuffer> m_dataList;
    std::mutex m_lock;
};

#endif // XRINGBUFFER_H
