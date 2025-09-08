#include <marnav/ais/message_11.hpp>
#include <marnav/ais/angle.hpp>
#include <stdexcept>

namespace marnav
{
namespace ais
{
constexpr message_id message_11::ID;

message_11::message_11()
	: message_04(ID)
{
}

message_11::message_11(const raw & bits)
	: message_04(ID)
{
	if (bits.size() != SIZE_BITS)
		throw std::invalid_argument{"invalid number of bits in message_11"};
	read_data(bits);
}
}
}
