#include "common/net/protobuf/codec_proto.h"

#include <zlib.h> // adler32
#include <google/protobuf/descriptor.h>

#include "common/base/base_define.h"
#include "common/net/endian_ex.h"
#include "common/net/protobuf/google-inl.h"


using namespace std;
using namespace common;
using namespace common::net;

/// @code
/// +-----+---------+-------------+---------------+-----------+
/// | len | namelen | name_string | protobuf_data | checksum  |
/// |     |         |             |   (CONTENT)   | (adler32) |
/// +-----+---------+-------------+---------------+-----------+
/// @endcode
void ProtobufCodec::FillEmptyBuffer(Buffer* buf, const google::protobuf::Message& message)
{
	// buf->retrieveAll();
	assert(buf->ReadableBytes() == 0);

	const std::string& typeName = message.GetTypeName();
	int32_t nameLen = static_cast<int32_t>(typeName.size()+1);
	buf->AppendInt32(nameLen);
	buf->Append(typeName.c_str(), nameLen);

	// code copied from MessageLite::SerializeToArray() and MessageLite::SerializePartialToArray().
	GOOGLE_DCHECK(message.IsInitialized()) << InitializationErrorMessage("serialize", message);

	int byte_size = message.ByteSize();
	buf->EnsureWritableBytes(byte_size);

	uint8_t* start = reinterpret_cast<uint8_t*>(buf->BeginWrite());
	uint8_t* end = message.SerializeWithCachedSizesToArray(start);
	if (end - start != byte_size)
	{
		ByteSizeConsistencyError(byte_size, message.ByteSize(), static_cast<int>(end - start));
	}
	buf->HasWritten(byte_size);

	int32_t checkSum = static_cast<int32_t>(
			                   ::adler32(1,
				                 reinterpret_cast<const Bytef*>(buf->peek()),
				                 static_cast<int>(buf->ReadableBytes())));
	buf->AppendInt32(checkSum);
	assert(buf->ReadableBytes() == sizeof nameLen + nameLen + byte_size + sizeof checkSum);
	int32_t len = sockets::HostToNetwork32(static_cast<int32_t>(buf->ReadableBytes()));
	buf->Prepend(&len, sizeof len);
}

void ProtobufCodec::SendMsg(const muduo::net::TcpConnectionPtr& conn, const google::protobuf::Message& message)
{
	Buffer buf;
	FillEmptyBuffer(&buf, message);
	conn->Send(&buf);
}

//
// no more google code after this
//

//
// FIXME: merge with RpcCodec
//

namespace
{
	const string kNoErrorStr			= "NoError";
	const string kInvalidLengthStr		= "InvalidLength";
	const string kCheckSumErrorStr		= "CheckSumError";
	const string kInvalidNameLenStr		= "InvalidNameLen";
	const string kUnknownMessageTypeStr = "UnknownMessageType";
	const string kParseErrorStr			= "ParseError";
	const string kUnknownErrorStr		= "UnknownError";
}

const string& ProtobufCodec::ErrorCodeToString(ErrorCode errorCode)
{
	switch (errorCode)
	{
		case kNoError:
			return kNoErrorStr;
		case kInvalidLength:
			return kInvalidLengthStr;
		case kCheckSumError:
			return kCheckSumErrorStr;
		case kInvalidNameLen:
			return kInvalidNameLenStr;
		case kUnknownMessageType:
			return kUnknownMessageTypeStr;
		case kParseError:
			return kParseErrorStr;
		default:
			return kUnknownErrorStr;
	}
}

void ProtobufCodec::DefaultErrorCallback(const muduo::net::TcpConnectionPtr& conn,
		                                 common::net::Buffer* buf,
		                                 ErrorCode errorCode)
{
	if (conn && conn->connected())
	{
		conn->Shutdown();
	}
}

static int32_t AsInt32(const char* buf)
{
	int32_t be32 = 0;
	::memcpy(&be32, buf, sizeof(be32));
	return sockets::NetworkToHost32(be32);
}

int ProtobufCodec::ParseMessages(const muduo::net::TcpConnectionPtr& conn,
		                         common::net::Buffer* buf,
								 std::vector<MessagePtr>& vecMessagePtr)
{
	while (buf->ReadableBytes() >= kMinMessageLen + kHeaderLen)
	{
		const int32_t len = buf->PeekInt32();
		if (len > kMaxMessageLen || len < kMinMessageLen)
		{
			error_callback_(conn, buf, kInvalidLength);
			return -1;
		}
		else if (buf->ReadableBytes() >= implicit_cast<size_t>(len + kHeaderLen))
		{
			ErrorCode errorCode = kNoError;
			MessagePtr message = Parse(buf->peek()+kHeaderLen, len, &errorCode);
			if (errorCode == kNoError && message)
			{
				vecMessagePtr.push_back(message);
				buf->Retrieve(kHeaderLen+len);
			}
			else
			{
				error_callback_(conn, buf, errorCode);
				return -1;
			}
		}
		else
		{
			break;
		}
	}

	return 0;
}

google::protobuf::Message* ProtobufCodec::CreateMessage(const std::string& typeName)
{
	google::protobuf::Message* message = NULL;
	const google::protobuf::Descriptor* descriptor =
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
	if (descriptor)
	{
		const google::protobuf::Message* prototype =
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype)
		{
			message = prototype->New();
		}
	}
	return message;
}

ProtobufCodec::MessagePtr ProtobufCodec::Parse(const char* buf, int len, ErrorCode* error)
{
	MessagePtr message = NULL;

	// check sum
	int32_t expectedCheckSum = AsInt32(buf + len - kCheckSumLen);
	int32_t checkSum = static_cast<int32_t>(
			                    ::adler32(1,
				                      reinterpret_cast<const Bytef*>(buf),
				                      static_cast<int>(len - kCheckSumLen)));
	if (checkSum == expectedCheckSum)
	{
		// get message type name
		int32_t nameLen = AsInt32(buf);
		if (nameLen >= 2 && nameLen <= len - kCheckSumLen)
		{
			std::string typeName(buf + kNamelenLen, buf + kNamelenLen + nameLen - 1);
			// create message object
			message = CreateMessage(typeName);
			if (message)
			{
				// parse from buffer
				const char* data = buf + kNamelenLen + nameLen;
				int32_t dataLen = len - nameLen - kNamelenLen - kCheckSumLen;
				if (message->ParseFromArray(data, dataLen))
				{
					*error = kNoError;
				}
				else
				{
					*error = kParseError;
					SAFE_DELETE(message);
				}
			}
			else
			{
				LOG_ERROR << "protocol:" << typeName << "not found!";
				*error = kUnknownMessageType;
			}
		}
		else
		{
			*error = kInvalidNameLen;
		}
	}
	else
	{
		*error = kCheckSumError;
	}

	return message;
}
