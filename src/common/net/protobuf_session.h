#ifndef COMMON_NET_PROTOBUF_SESSION_H
#define COMMON_NET_PROTOBUF_SESSION_H

#include "common/net/codec_proto.h"
#include "common/net/dispatcher_proto.h"
#include "common/net/session.h"

namespace common
{
namespace net
{

class ProtobufSession : public Session
{
public:
	ProtobufSession(SessionManager* pmanager, const muduo::net::TcpConnectionPtr& conn);
	virtual ~ProtobufSession();
protected:
	virtual void OnRecv(Buffer *pbuf) = 0;
	virtual void OnStartupRegisterMsgHandler() = 0;

	virtual void PacketDefaultHandler(const MessageRef packet);
	virtual void ClearPacketVec();
private:
	virtual void StartupRegisterMsgHandler();
protected:
	ProtobufCodec codec_;
	ProtobufDispatcher dispatcher_;

	typedef std::vector<ProtobufCodec::MessagePtr> ProtoPtrVec;
	ProtoPtrVec vec_packets_;
};

} // namespace net
} // namespace common

#endif // COMMON_NET_PROTOBUF_SESSION_H
