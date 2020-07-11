#pragma once

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

