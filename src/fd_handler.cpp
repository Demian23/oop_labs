#include "fd_handler.hpp"
#include "unistd.h"

FdHandler::~FdHandler()
{
    if(own)
        close(fd);
}

void FdHandler::SetEvents(int a_events)
{
    events = a_events;
}

