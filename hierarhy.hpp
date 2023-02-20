#ifndef HIERARCHY_DEFF
#define HIERARCHY_DEFF

#include <netinet/in.h>

class FdHandler{
public:
    enum Events{InEvent = 16, OutEvent = 32, ErrEvent = 64, None = 0, 
        IOEvent = InEvent + OutEvent, IEEvent = InEvent + ErrEvent, 
        OEEvent = OutEvent + ErrEvent, IOEEvent = OutEvent + InEvent + 
        ErrEvent};
protected:
    int fd;
    Events events;
    short cond; 
    bool own;
    FdHandler(const FdHandler&);
    FdHandler& operator=(const FdHandler&);
public:
    enum{done = 10, work = 8};
    FdHandler(int a_fd, bool own_fd = true) : fd(a_fd), events(None), 
        cond(work), own(own_fd){};
    virtual ~FdHandler();
    virtual int HandleRead() = 0;
    virtual int HandleWrite() = 0;
    virtual int HandleError() = 0;
    void SetEvents(Events a_events){events = a_events;}
    int GetFd()const{return fd;}
    int GetEvents()const {return events;}
    short GetCond() const{return cond;}
};

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

class TcpSocket : public StreamHandler{
protected:
    struct sockaddr_in *client_address;
public:
    TcpSocket(int a_fd, bool own_fd = true) : StreamHandler(a_fd, own_fd), 
        client_address(0){}
    void SetAddr(struct sockaddr_in cl_addr){
        client_address = new struct sockaddr_in; *client_address = cl_addr;}
};

//tcp socket, read/write
class ControllSocketHandler : public TcpSocket{
protected:
    Buffer in;
    Buffer out;
public:
    ControllSocketHandler(int a_fd, bool own_fd, unsigned in_buff_size = 
        Buffer::default_b_s, unsigned out_buff_size = Buffer::default_b_s)
        : TcpSocket(a_fd, own_fd), in(in_buff_size), out(out_buff_size){}
    virtual int HandleRead();
    virtual int HandleWrite();
    virtual int HandleError();
    virtual ~ControllSocketHandler();
};

//pass socket already created with socket()
class AcceptSocketHandler : public FdHandler{
private:
    virtual int HandleWrite();
protected:
    enum{def_q_len = 3};
    TcpSocket *NewHandler;
    int accept_queue_len;
public:
   AcceptSocketHandler(int a_fd, bool own_fd = true, 
       int queue_len = def_q_len) : FdHandler(a_fd, own_fd), 
       NewHandler(0), accept_queue_len(queue_len){}
   virtual int HandleRead();
   virtual int HandleError();
};

class RStreamHandler: public StreamHandler{
private:
    virtual int HandleWrite();
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
protected:
   Buffer *wbuff; 
public:
   WStreamHandler(int a_fd, bool own_fd = true, Buffer *a_buffer = 0)
       : StreamHandler(a_fd, own_fd), wbuff(a_buffer){}
   virtual ~WStreamHandler(){}
   virtual int HandleWrite();
};

class RWStreamHandler : public WStreamHandler{
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

#endif
