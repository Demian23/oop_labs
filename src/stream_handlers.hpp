#ifndef STREAM_HANDLERS_DEF
#define STREAM_HANDLERS_DEF
#include "fd_handler.hpp"

class StreamHandler : public FdHandler{
protected:
    struct Buffer{
        enum{default_b_s = 4096};
        char *stream_buffer;
        unsigned buffer_size;
        unsigned buffer_cond;
        bool can_read;
        short can_write;
        Buffer(unsigned size_in_bytes = default_b_s) 
            : buffer_size(size_in_bytes), buffer_cond(0), can_read(true), 
            can_write(0){stream_buffer = new char[buffer_size];}
        ~Buffer(){delete[] stream_buffer;}
    };
public:
    StreamHandler(int a_fd, bool own_fd = true) : FdHandler(a_fd, own_fd){}
    virtual int HandleError();
};

class RStreamHandler: public StreamHandler{
private:
    virtual int HandleWrite();
    virtual int HandleTimeout();
protected:
    Buffer rbuff;
public:
    RStreamHandler(int a_fd, bool own_fd = true, unsigned buffer_size = 
        Buffer::default_b_s)
        : StreamHandler(a_fd, own_fd), rbuff(buffer_size){}; 
    virtual ~RStreamHandler(){}
    virtual int HandleRead();
};

class WStreamHandler : public StreamHandler{
private:
    virtual int HandleRead();
    virtual int HandleTimeout();
protected:
   Buffer *wbuff; 
public:
   WStreamHandler(int a_fd, bool own_fd = true, Buffer *a_buffer = 0)
       : StreamHandler(a_fd, own_fd), wbuff(a_buffer){}
   virtual ~WStreamHandler(){}
   virtual int HandleWrite();
};

class RWStreamHandler : public WStreamHandler{
private:
    virtual int HandleTimeout();
protected:
    Buffer read_buff;
public:
    RWStreamHandler(int a_fd, bool own_fd = true, Buffer *a_buffer = 0, 
        unsigned read_b_s = Buffer::default_b_s) 
        : WStreamHandler(a_fd, own_fd, a_buffer), read_buff(read_b_s){}
    virtual int HandleError();
    virtual int HandleWrite();
    virtual int HandleRead();
    char *GetBuffer() const;
};

#endif // !STREAM_HANDLERS_DEF
