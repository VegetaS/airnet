#ifndef COMMON_NET_SESSION_H
#define COMMON_NET_SESSION_H

#include <string>
#include <google/protobuf/message.h>
#include "common/net/tcpconnection.h"

namespace common
{
namespace net
{

class SessionManager;

class Session : public TcpConnection
{
public:
	typedef google::protobuf::Message& MessageRef;

	Session(EventLoop *ploop, int32_t sid, int32_t sockfd, SessionManager *pmanager);

	void ConnectCompleted();
	void Close();
	void SendProtocol(MessageRef packet);

protected:
	virtual void OnRecv(Buffer* pbuf) = 0;
	virtual void OnAddSession() = 0;
	virtual void OnDelSession() = 0;

	virtual void StartupRegisterMsgHandler() = 0;
	virtual void ClearPacketVec() = 0;
	virtual void OnConnected();
	virtual void OnClose();
protected:
	SessionManager *pmanager_;
	int32_t status_;
};

} // namespace net
} // namespace shared

#endif // COMMON_NET_SESSION_H
