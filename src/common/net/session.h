#ifndef COMMON_NET_SESSION_H
#define COMMON_NET_SESSION_H

#include <google/protobuf/message.h>
#include <boost/enable_shared_from_this.hpp>

#include <muduo/net/TcpConnection.h>

namespace common
{
namespace net
{

class SessionManager;

class Session : common::noncopyable,
                public boost::enable_shared_from_this<Session>
{
public:
	typedef google::protobuf::Message& MessageRef;

	Session(SessionManager* pmanager, const muduo::net::TcpConnectionPtr& conn);

	void SendProtocol(MessageRef packet);

protected:
	virtual void OnMessage(const muduo::net::TcpConnectionPtr& conn,
            muduo::net::Buffer* buffer,
            muduo::Timestamp) = 0;
    virtual void OnConnected(const muduo::net::TcpConnectionPtr& conn) = 0;
	virtual void OnClose(const muduo::net::TcpConnectionPtr& conn) = 0;
    virtual void OnWriteComplete(const muduo::net::TcpConnectionPtr& conn);
    virtual void OnHighWaterMark(const muduo::net::TcpConnectionPtr& conn);

	virtual void OnAddSession() = 0;
	virtual void OnDelSession() = 0;

	virtual void StartupRegisterMsgHandler() = 0;
	
protected:
	SessionManager *pmanager_;
    Muduo::net::TcpConnectionPtr conn_;
};

} // namespace net
} // namespace common

#endif // COMMON_NET_SESSION_H
