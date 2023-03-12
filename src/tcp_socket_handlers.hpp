#ifndef TCP_SOCKET_HANDLERS_DEF
#define TCP_SOCKET_HANDLERS_DEF

#include "stream_handlers.hpp"
#include <netdb.h>
class TcpSocket : public StreamHandler{
protected:
    sockaddr_in *client_address;
public:
    TcpSocket(int a_fd, bool own_fd, const char *ip_addr);
};

class ControllSocketHandler : public TcpSocket{
protected:
    Buffer in;
    Buffer out;
public:
    ControllSocketHandler(int a_fd, bool own_fd, const char *dest_ip,
        unsigned in_buff_size = Buffer::default_b_s, 
        unsigned out_buff_size = Buffer::default_b_s)
        : TcpSocket(a_fd, own_fd, dest_ip), in(in_buff_size), out(out_buff_size){}
    virtual int HandleRead();
    virtual int HandleWrite();
    virtual int HandleError();
    virtual int HandleTimeout();
    virtual ~ControllSocketHandler();
};

#endif // !TCP_SOCKET_HANDLERS_DEF
