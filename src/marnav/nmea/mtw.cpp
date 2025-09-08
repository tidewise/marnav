#include <marnav/nmea/mtw.hpp>
#include <marnav/nmea/io.hpp>
#include <stdexcept>

namespace marnav
{
namespace nmea
{
constexpr sentence_id mtw::ID;
constexpr const char * mtw::TAG;

mtw::mtw()
	: sentence(ID, TAG, talker_id::integrated_instrumentation)
{
}

mtw::mtw(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 2)
		throw std::invalid_argument{"invalid number of fields in mtw"};

	read(*(first + 0), temperature_);
	read(*(first + 1), temperature_unit_);
}

void mtw::set_temperature(double t) noexcept
{
	temperature_ = t;
	temperature_unit_ = unit::temperature::celsius;
}

void mtw::append_data_to(std::string & s) const
{
	append(s, to_string(temperature_));
	append(s, to_string(temperature_unit_));
}
}
}
