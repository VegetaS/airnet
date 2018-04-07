#ifndef COMMON_NET_PROTOBUF_CODEC_PROTO_H_
#define COMMON_NET_PROTOBUF_CODEC_PROTO_H_

#include <vector>
#include <google/protobuf/message.h>

#include "common/base/noncopyable.h"
#include "common/base/types.h"
#include "common/net/buffer.h"


namespace common {
namespace net {

///
/// FIXME: merge with RpcCodec
///
class ProtobufCodec : noncopyable
{
public:
	enum ErrorCode
	{
		kNoError = 0,
		kInvalidLength,
		kCheckSumError,
		kInvalidNameLen,
		kUnknownMessageType,
		kParseError,
	};

	typedef google::protobuf::Message* MessagePtr;
	typedef google::protobuf::Message& MessageRef;

	typedef void (*ErrorCallback)(const muduo::net::TcpConnectionPtr&,
			                      common::net::Buffer*,
								  ErrorCode);

	explicit ProtobufCodec(const ErrorCallback& errorCb = DefaultErrorCallback)
		: error_callback_(errorCb)
	{ }

	int ParseMessages(const muduo::net::TcpConnectionPtr& conn,
					  common::net::Buffer* buf,
					  std::vector<MessagePtr>& vecMessagePtr);

	void Send(const muduo::net::TcpConnectionPtr& conn, 
			  const google::protobuf::Message& message)
	{
		// FIXME: serialize to TcpConnection::OutputBuffer();
		common::net::Buffer buf;
		FillEmptyBuffer(&buf, message);
		conn->Send(&buf);
	}

	static void SendMsg(const muduo::net::TcpConnectionPtr& conn, const google::protobuf::Message& message);

	static const std::string& ErrorCodeToString(ErrorCode errorCode);
	static void FillEmptyBuffer(common::net::Buffer* buf, const google::protobuf::Message& message);
	static google::protobuf::Message* CreateMessage(const std::string& type_name);
	static MessagePtr Parse(const char* buf, int len, ErrorCode* errorCode);

private:
	static void DefaultErrorCallback(const muduo::net::TcpConnectionPtr& conn,
			                         common::net::Buffer*,
								     ErrorCode);
	ErrorCallback error_callback_;

	const static int kHeaderLen		= sizeof(int32_t);
	const static int kNamelenLen    = sizeof(int32_t);
	const static int kCheckSumLen   = sizeof(int32_t);
	const static int kMinMessageLen = kHeaderLen + 2 + kCheckSumLen; // nameLen + typeName + checkSum
	const static int kMaxMessageLen = 64*1024*1024; // same as codec_stream.h kDefaultTotalBytesLimit
};

} // namespace net
} // namespace common

#endif // COMMON_NET_PROTOBUF_CODEC_PROTO_H_
