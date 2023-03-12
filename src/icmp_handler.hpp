#ifndef ICMP_HANDLER_DEF
#define ICMP_HANDLER_DEF
#include "fd_handler.hpp"
class RecvEchoHandler : public FdHandler{
private:
    int seq;
    int id;
    virtual int HandleWrite();
    virtual int HandleTimeout();
public:
    RecvEchoHandler(int a_fd, bool own, int a_id);
    virtual int HandleError();
    virtual int HandleRead();
    bool Exists() const {return seq;}
    virtual ~RecvEchoHandler(){} 
};

#endif
