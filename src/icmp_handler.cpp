#include "icmp_handler.hpp"
int RecvEchoHandler::HandleError()
{
    return -1;
}

int RecvEchoHandler::HandleRead()
{
//    struct msghdr msg;
//    ssize_t len = DICMP::recv_reply(fd , &msg);
//    if(DICMP::process_reply(fd, reinterpret_cast<char *>(msg.msg_iov->iov_base), len, id))
//        seq++;
    return 0;
}

int RecvEchoHandler::HandleWrite()
{
    return -1;
}

int RecvEchoHandler::HandleTimeout()
{
    return -1;
}
