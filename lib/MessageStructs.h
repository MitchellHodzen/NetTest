#pragma once
#include <string>

enum MSG_TYPE {INVALID = 0, CONNECT_REQUEST = 1, CONNECT_RESPONSE = 2, DISCONNECT = 3, TEXT = 4, ACK = 5};
namespace MESSAGE
{
	MSG_TYPE DetermineMessageType(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		if (dataBuffer == nullptr || bufferSize < 4)
		{
			return MSG_TYPE::INVALID;
		}
		//The first four bytes of every message defines the enum of the message type
		MSG_TYPE messageType = (MSG_TYPE) (dataBuffer[0] | (dataBuffer[1]<<8) | (dataBuffer[2]<<16) | (dataBuffer[3]<<24));
		return messageType;
	}
}
struct MSG_CONNECT_REQUEST
{
public:
	MSG_TYPE messageType;

	MSG_CONNECT_REQUEST()
	{
		this->messageType = MSG_TYPE::CONNECT_REQUEST;
	}
};

struct MSG_CONNECT_RESPONSE
{
public:
	MSG_TYPE messageType;
	unsigned int networkId;

	MSG_CONNECT_RESPONSE(unsigned int networkId)
	{
		this->messageType = MSG_TYPE::CONNECT_RESPONSE;
		this->networkId = networkId;
	}

	MSG_CONNECT_RESPONSE(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		BuildFromByteArray(dataBuffer, bufferSize);
	}

private:

	void BuildFromByteArray(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		this->messageType = MSG_TYPE::CONNECT_RESPONSE;

		unsigned int networkId = dataBuffer[4] | (dataBuffer[5]<<8) | (dataBuffer[6]<<16) | (dataBuffer[7]<<24);
		this->networkId = networkId;
	}
};

struct MSG_DISCONNECT
{
public:
	MSG_TYPE messageType;
	unsigned int networkId;

	MSG_DISCONNECT(unsigned int networkId)
	{
		this->messageType = MSG_TYPE::DISCONNECT;
		this->networkId = networkId;
	}

	MSG_DISCONNECT(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		BuildFromByteArray(dataBuffer, bufferSize);
	}

private:

	void BuildFromByteArray(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		this->messageType = MSG_TYPE::DISCONNECT;

		unsigned int networkId = dataBuffer[4] | (dataBuffer[5]<<8) | (dataBuffer[6]<<16) | (dataBuffer[7]<<24);
		this->networkId = networkId;
	}
};

struct MSG_TEXT
{
public:
	static const int MAX_TEXT_BUFFER_LENGTH = 51; //Max length of the text buffer including null terminator
	MSG_TYPE messageType;
	unsigned int networkId;
	unsigned int textBufferLength; //Include null terminator in length
	char text[MAX_TEXT_BUFFER_LENGTH];

	MSG_TEXT(unsigned int networkId, unsigned int textBufferLength, const char* text)
	{
		this->messageType = MSG_TYPE::TEXT;
		this->networkId = networkId;
		this->textBufferLength = textBufferLength;
		if (this->textBufferLength > MAX_TEXT_BUFFER_LENGTH)
		{
			this-> textBufferLength = MAX_TEXT_BUFFER_LENGTH;
		}

		//Copy the passed in text into the text buffer to be sent
		if (text != nullptr)
		{
			for (int i = 0; i < this->textBufferLength; ++i)
			{
				this->text[i] = text[i];
			}
		}
	}

	MSG_TEXT(unsigned int networkId, std::string text)
	{
		this->messageType = MSG_TYPE::TEXT;
		this->networkId = networkId;
		this->textBufferLength = text.size() + 1;
		if (this->textBufferLength > MAX_TEXT_BUFFER_LENGTH)
		{
			this-> textBufferLength = MAX_TEXT_BUFFER_LENGTH;
		}

		//Copy the passed in text into the text buffer to be sent
		const char* text_array = text.c_str();
		for (int i = 0; i < this->textBufferLength; ++i)
		{
			this->text[i] = text_array[i];
		}
	}

	MSG_TEXT(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		BuildFromByteArray(dataBuffer, bufferSize);
	}

private:

	void BuildFromByteArray(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		this->messageType = MSG_TYPE::TEXT;

		unsigned int networkId = dataBuffer[4] | (dataBuffer[5]<<8) | (dataBuffer[6]<<16) | (dataBuffer[7]<<24);
		this->networkId = networkId;

		unsigned int textBufferLength = dataBuffer[8] | (dataBuffer[9]<<8) | (dataBuffer[10]<<16) | (dataBuffer[11]<<24);
		this->textBufferLength = textBufferLength;

		if (this->textBufferLength > MAX_TEXT_BUFFER_LENGTH)
		{
			this-> textBufferLength = MAX_TEXT_BUFFER_LENGTH;
		}

		unsigned int bufferTextStart = 12; //Text starts at the 12th byte
		for (int i = 0; i < this->textBufferLength; ++i)
		{
			this->text[i] = dataBuffer[bufferTextStart + i];
		}

	}
};

struct MSG_ACK
{
public:
	MSG_TYPE messageType;
	unsigned int networkId;

	MSG_ACK(unsigned int networkId)
	{
		this->messageType = MSG_TYPE::ACK;
		this->networkId = networkId;
	}

	MSG_ACK(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		BuildFromByteArray(dataBuffer, bufferSize);
	}

private:

	void BuildFromByteArray(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		this->messageType = MSG_TYPE::ACK;

		unsigned int networkId = dataBuffer[4] | (dataBuffer[5]<<8) | (dataBuffer[6]<<16) | (dataBuffer[7]<<24);
		this->networkId = networkId;
	}
};

struct TestStruct
{
public:

	unsigned int networkId;
	int integer;
	float floatingPoint;

	TestStruct() : TestStruct(0,0,0) {}

	TestStruct(unsigned int networkId, int integer, float floatingPoint)
	{
		this->networkId = networkId;
		this->integer = integer;
		this->floatingPoint = floatingPoint;
	}

	TestStruct(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		BuildFromByteArray(dataBuffer, bufferSize);
	}

private:

	void BuildFromByteArray(unsigned char* dataBuffer, unsigned int bufferSize)
	{
		unsigned int networkId = dataBuffer[0] | (dataBuffer[1]<<8) | (dataBuffer[2]<<16) | (dataBuffer[3]<<24);
		this->networkId = networkId;

		unsigned int intBytes = dataBuffer[4] | (dataBuffer[5]<<8) | (dataBuffer[6]<<16) | (dataBuffer[7]<<24);
		int integer = *(reinterpret_cast<int*>(&intBytes));
		this->integer = integer;

		unsigned int floatBytes = dataBuffer[8] | (dataBuffer[9]<<8) | (dataBuffer[10]<<16) | (dataBuffer[11]<<24);
		float floatingPoint = *(reinterpret_cast<float*>(&floatBytes));
		this->floatingPoint = floatingPoint;
	}
};

