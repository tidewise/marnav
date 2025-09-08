#include <marnav/nmea/tds.hpp>
#include <marnav/nmea/io.hpp>
#include <stdexcept>

namespace marnav
{
namespace nmea
{
constexpr sentence_id tds::ID;
constexpr const char * tds::TAG;

tds::tds()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

tds::tds(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 2)
		throw std::invalid_argument{"invalid number of fields in tds"};

	read(*(first + 0), distance_);
	read(*(first + 1), distance_unit_);
}

void tds::append_data_to(std::string & s) const
{
	append(s, to_string(distance_));
	append(s, to_string(distance_unit_));
}
}
}
