#ifndef FD_HANDLER_DEF
#define FD_HANDLER_DEF

class FdHandler{
public:
    enum Events{InEvent = 16, OutEvent = 32, ErrEvent = 64, None = 0, Timeout = 8, 
        IOEvent = InEvent + OutEvent, IEEvent = InEvent + ErrEvent, 
        OEEvent = OutEvent + ErrEvent, IOEEvent = OutEvent + InEvent + ErrEvent,
        IETEvent = InEvent + ErrEvent + Timeout,
        IOETEvent = OutEvent + ErrEvent + InEvent + Timeout};
protected:
    int fd;
    int events;
    short cond; 
    bool own;
    FdHandler(const FdHandler&);
    FdHandler& operator=(const FdHandler&);
public:
    enum{done = 10, work = 8};
    FdHandler(int a_fd, bool own_fd = true) 
        : fd(a_fd), events(None), cond(work), own(own_fd){};
    virtual ~FdHandler();
    virtual int HandleRead() = 0;
    virtual int HandleWrite() = 0;
    virtual int HandleError() = 0;
    virtual int HandleTimeout() = 0;
    void SetEvents(int a_events);
    int GetFd()const{return fd;}
    int GetEvents()const {return events;}
    short GetCond() const{return cond;}
    void ExplicitlyEnd(){cond = done;}
};
#endif
