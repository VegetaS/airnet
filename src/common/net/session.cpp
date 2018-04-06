#include "session.h"
#include "session_manager.h"

#include <boost/bind.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

namespace common
{
namespace net
{
    static void onWriteCompleteWeak(const boost::weak_ptr<Session>& psess,
            const muduo::net::TcpConnectionPtr& conn)
    {
        boost::shared_ptr<Session> ses = psess->lock();
        if (ses)
        {
            ses->onWriteComplete(conn);
        }
    }

    Session::Session(size_t watermark, SessionManager *pmanager, const muduo::net::TcpConnectionPtr& conn)
        : pmanager_(pmanager),
        conn_(conn)
    {
        conn_->setConnectionCallback(
                boost::bind(&Session::OnConnected, shared_from_this(), _1));
        conn_->setMessageCallback(
                boost::bind(&Session::OnMessage, shared_from_this(), _1, _2, _3));
        conn_->setCloseCallback(
                boost::bind(&Session::OnClose, shared_from_this(), _1));
        conn_->setHighWaterMarkCallback(
                boost::bind(&Session::OnHighWaterMark,
                    boost::weak_ptr<Session>(shared_from_this()), _1, _2), watermark);
    }

    void Session::SendProtocol(MessageRef packet)
    {
    }

    void Session::OnConnected(const muduo::net::TcpConnectionPtr& conn)
    {
        if (conn->connected())
        {
            conn->setTcpNoDelay(true);
        }
        return ;
    }

    void Session::OnClose(const muduo::net::TcpConnectionPtr& conn)
    {
        return ;
    }

    void Session::OnWriteComplete(const muduo::net::TcpConnectionPtr& conn)
    {
        conn_->startRead();
        conn_->setWriteCompleteCallback(muduo::net::WriteCompleteCallback());
    }

    void Session::OnHighWaterMark(const muduo::net::TcpConnectionPtr& conn)
    {
        if (conn_->outputBuffer()->readableBytes() > 0)
        {
            conn_->stopRead();
            conn_->setWriteCompleteCallback(
                boost::bind(&onWriteCompleteWeak, 
                    boost::weak_ptr<Session>(shared_from_this()), _1));
        }
    }
    

} // namespace net
} // namespace common
