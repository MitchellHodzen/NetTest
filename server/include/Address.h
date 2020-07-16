#pragma once
#include <functional>
#include <iomanip>

struct Address
{
	unsigned int address;
	unsigned short port;

	bool operator==(const Address &other) const
	{
		return !(address != other.address || port != other.port);
	}
};

//Hash function to use in map
namespace std
{
    template<> 
	struct hash<Address>
    {
        std::size_t operator()(Address const& s) const noexcept
        {
			return s.address + s.port;
        }
    };
}