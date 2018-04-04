#ifndef SRC_QUEUE_MSG_TYPE_H
#define SRC_QUEUE_MSG_TYPE_H

#include "common/copyable.h"
#include "common/blocking_queue.h"
#include "common/rwlock_queue.h"

#include "muduo/net/protobuf/ProtobufCodecLite.h"

class QueuedRecvMsg : public common::copyable
{
public:
    QueuedRecvMsg() : sendback_sid(-1), msg_ptr(NULL) 
    { }    

    QueuedRecvMsg(int32_t sendbackSid, 
                  muduo::net::MessagePtr msgPtr)
        : sendback_sid(sendbackSid),
          msg_ptr(msgPtr) 
    { }

    int32_t sendback_sid;
    muduo::net::MessagePtr msg_ptr;
};

#endif
