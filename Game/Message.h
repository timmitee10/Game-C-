#pragma once
#include <ostream>
#include <type_traits>
#include <vector>

namespace NodelNet
{
	typedef float network_priority_t;
	typedef float message_priority_t;
	enum class NodelMessageType
	{
		ConnectionRequest = 0,
		ConnectionRespons = 1,
	};

#define MESSAGE_TRAIT_FLAG_ALLOW_ENCRYPTION 1 << 0 //Message encryption is allowed.
#define MESSAGE_TRAIT_FLAG_IS_ENCRYPTED 1 << 1 //Message is encrypted.
#define MESSAGE_TRAIT_FLAG_ALLOW_COMPRESSION 1 << 2 //Messages are allowed to use compression.
#define MESSAGE_TRAIT_FLAG_IS_COMPRESSED 1 << 3 //Message received is compressed.
#define MESSAGE_TRAIT_FLAG_IS_ACK 1 << 4 //Acknowledgment(did not receive the message).
#define MESSAGE_TRAIT_FLAG_IS_NACK 1 << 5 //Negative acknowledgment(did receive the message but the message was corrupted).
#define MESSAGE_TRAIT_FLAG_CACHE 1 << 6 //Should cache the resource.
#define MESSAGE_TRAIT_FLAG_RELIABLE 1 << 8 //Will ensure packet is received.

	//Followed by 4 bits that define how many ACKs.
	struct MessageTrait
	{
		typedef uint16_t trait_t;
		trait_t blob;
	private:
		static const trait_t kEncrypted = MESSAGE_TRAIT_FLAG_IS_ENCRYPTED;
		static const trait_t kCompressed = MESSAGE_TRAIT_FLAG_IS_COMPRESSED;
		static const trait_t kACK = MESSAGE_TRAIT_FLAG_IS_ACK;
	};

	template<typename T>
	struct MessageHeader
	{
		T id{};
		uint32_t size;
	};

	template<typename T>
	struct MessageHeaderWithTrait
	{
		using is_class_enum = std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>;
		static_assert(is_class_enum() != std::false_type::value, "Message id must be a enum class");
		T id{};
		uint32_t size;
		//trait is defined by each bit. 
		uint16_t trait;
	};
	//TO DO: Fix 
	template<typename T>
	struct MessaageHeaderWithTraitAndAck
	{
		using is_class_enum = std::integral_constant<bool, std::is_enum<T>::value && !std::is_convertible<T, int>::value>;
		static_assert(is_class_enum() != std::false_type::value, "Message id must be a enum class");
		T id{};
		uint32_t size;
		uint16_t trait;
		uint32_t ackNum; //Ack number, response must contain number.
	};

	enum class DataType
	{

	};

	/*
		Message is used over the network to send messages between computers.
		The message structure is intended to be used by anyone sending messages over the network and only includes the bare minimum.
	*/
	template <typename T>
	struct Message
	{
		using Header = MessageHeader<T>;
		Message() = default;

		Message(T msgType, const std::vector<uint8_t>& vec) : body(vec) { header.id = msgType; }

		Message(T msgType) : header.id(msgType) {};

		Message(T msgType, const std::vector<uint8_t>& vec, const uint32_t size) : body(vec) { header.id = msgType; header.size = size; };

		Message(T msgType, void* pData, size_t size) { //TO DO: fix size
			header.id = msgType;
			header.size = *(((char*)&pData) + sizeof(header.id));
			body.resize(size - sizeof(header.id) - sizeof(header.size));
			memcpy(body.data(), (((char*)pData) + sizeof(header.id) + sizeof(header.size)), size - sizeof(header.id) - sizeof(header.size));
		};

		//return the entire size of message in bytes.
		size_t size() const;
		Header header{};
		std::vector<uint8_t> body;
		friend std::ostream& operator << (std::ostream& os, const Message<T>& msg);
		//Push ant POD-like data into the message buffer.
		template<typename TDataType>
		friend Message<T>& operator<<(Message<T>& msg, const TDataType& data);
		//Data will be in reverse order.
		template<typename TDataType>
		friend Message<T>& operator>>(Message<T>& msg, TDataType& data);

		friend Message<T>& operator<<(Message<T>& msg, float_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, double_t lhs);

		friend Message<T>& operator<<(Message<T>& msg, uint8_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, uint16_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, uint32_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, uint64_t lhs);

		friend Message<T>& operator<<(Message<T>& msg, int8_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, int16_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, int32_t lhs);
		friend Message<T>& operator<<(Message<T>& msg, int64_t lhs);

		friend Message<T>& operator<<(Message<T>& msg, std::string lhs);
		friend Message<T>& operator<<(Message<T>& msg, flatbuffers::FlatBufferBuilder fbb);
		template<typename TType>
		friend Message<T>& operator<<(Message<T>& msg, std::vector<TType> lhs);


		friend Message<T>& operator>>(Message<T>& msg, float_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, double_t lhs);

		friend Message<T>& operator>>(Message<T>& msg, uint8_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, uint16_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, uint32_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, uint64_t lhs);

		friend Message<T>& operator>>(Message<T>& msg, int8_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, int16_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, int32_t lhs);
		friend Message<T>& operator>>(Message<T>& msg, int64_t lhs);

		friend Message<T>& operator>>(Message<T>& msg, std::string lhs);
		friend Message<T>& operator>>(Message<T>& msg, flatbuffers::FlatBufferBuilder fbb);
		template<typename TType>
		friend Message<T>& operator>>(Message<T>& msg, std::vector<TType> lhs);
	};

	template<typename T>
	size_t Message<T>::size() const
	{
		return sizeof(Header) + body.size();
	}
	template<typename T>
	std::ostream& operator<< (std::ostream& os, const Message<T>& msg)
	{
		os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
		return os;
	}

	template<typename T, typename TDataType>
	Message<T>& operator<<(Message<T>& msg, const TDataType& data)
	{
		//check that the type of data being pushed is trivially copyable.
		static_assert(std::is_standard_layout<TDataType>::value, "Data is too complex to be pushed into message");
		//Cache current size of vector.
		size_t i = msg.body.size();
		//Resize the vector by the size of the data being pushed. 
		msg.body.resize(msg.body.size() + sizeof(TDataType));
		//Copy the data into the newly allocated vector space.
		std::memcpy(msg.body.data() + i, &data, sizeof(TDataType));
		//Recalculate the message size.
		msg.header.size = msg.size();
		//Return the target message so it can be "chained".
		return msg;
	}

	template<typename T, typename TDataType>
	Message<T>& operator>>(Message<T>& msg, TDataType& data)
	{
		//check that the type of data being pushed is trivially copyable.
		static_assert(std::is_standard_layout<TDataType>::value, "Data is too complex to be pushed into variable");
		//Cache end location of vector.
		size_t i = msg.body.size() - sizeof(TDataType);
		//Copy data to the user variable.
		std::memcpy(&data, msg.body.data() + i, sizeof(TDataType));
		//Shrink the vector to remove read bytes and reset end point.
		msg.body.resize(i);
		//Recalculate the message size.
		msg.header.size = msg.size();
		//Return the target message so it can be "chained".
		return msg;
	}

	//// A FUCK TON OF MESSAGES 
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, float_t lhs)
	//{

	//}
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, double_t lhs)
	//{

	//}

	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, uint8_t lhs)
	//{

	//}
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, uint16_t lhs)
	//{

	//}
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, uint32_t lhs)
	//{

	//}
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, uint64_t lhs)
	//{

	//}

	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, int8_t lhs);
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, int16_t lhs);
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, int32_t lhs);
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, int64_t lhs);

	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, std::string lhs);
	//template<typename T>
	//Message<T>& operator<<(Message<T>& msg, flatbuffers::FlatBufferBuilder fbb);
	//template<typename T, typename TType>
	//Message<T>& operator<<(Message<T>& msg, std::vector<TType> lhs);

	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, float_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, double_t lhs);

	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, uint8_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, uint16_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, uint32_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, uint64_t lhs);

	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, int8_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, int16_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, int32_t lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, int64_t lhs);

	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, std::string lhs);
	//template<typename T>
	//Message<T>& operator>>(Message<T>& msg, flatbuffers::FlatBufferBuilder fbb);
	//template<typename T,typename TType>
	//Message<T>& operator>>(Message<T>& msg, std::vector<TType> lhs);

	typedef Message<NodelMessageType> NodelProtocolMessage;

}