#include "common/net/protobuf_session.h"

namespace common {
namespace net{
    ProtobufSession(SessionManager* pmanager, const muduo::net::TcpConnectionPtr& conn)
        : Session(pmanager, conn),
          dispatcher_(boost::bind())
    {
    }

    ProtobufSession::~ProtobufSession()
    {
        ClearPacketVec();
    }

    void ProtobufSession::ClearPacketVec()
    {
        ProtoPtrVec::iterator it = vec_packets_.begin();
        for (; it!= vec_packets_.end(); )
        {
            it.reset();
            it = vec_packets_.erase(it);
        }
    }

    void ProtobufSession::OnRecv(Buffer* pbuf)
    {
    }

    void ProtobufSession::PacketDefaultHandler(const muduo::net::TcpConnectionPtr& conn,
            const MessageRef packet)
    {
        return ;
    }

    void ProtobufSession::StartupRegisterMsgHandler()
    {
        OnStartupRegisterMsgHandler();
    }

}
}
