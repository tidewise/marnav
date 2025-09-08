#include <marnav/nmea/rmb.hpp>
#include "convert.hpp"
#include <marnav/nmea/io.hpp>
#include <stdexcept>

namespace marnav
{
namespace nmea
{
constexpr sentence_id rmb::ID;
constexpr const char * rmb::TAG;

rmb::rmb()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

rmb::rmb(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	// before and after NMEA 2.3
	const auto size = std::distance(first, last);
	if ((size < 13) || (size > 14))
		throw std::invalid_argument{"invalid number of fields in rmb"};

	read(*(first + 0), active_);
	read(*(first + 1), cross_track_error_);
	read(*(first + 2), steer_dir_);
	read(*(first + 3), waypoint_to_);
	read(*(first + 4), waypoint_from_);
	read(*(first + 5), lat_);
	read(*(first + 6), lat_hem_);
	read(*(first + 7), lon_);
	read(*(first + 8), lon_hem_);
	read(*(first + 9), range_);
	read(*(first + 10), bearing_);
	read(*(first + 11), dst_velocity_);
	read(*(first + 12), arrival_status_);

	// NMEA 2.3 or newer
	if (size > 13)
		read(*(first + 13), mode_ind_);

	// instead of reading data into temporary lat/lon, let's correct values afterwards
	lat_ = correct_hemisphere(lat_, lat_hem_);
	lon_ = correct_hemisphere(lon_, lon_hem_);
}

utils::optional<geo::longitude> rmb::get_longitude() const
{
	return (lon_ && lon_hem_) ? lon_ : utils::optional<geo::longitude>{};
}

utils::optional<geo::latitude> rmb::get_latitude() const
{
	return (lat_ && lat_hem_) ? lat_ : utils::optional<geo::latitude>{};
}

void rmb::set_lat(const geo::latitude & t)
{
	lat_ = t;
	lat_hem_ = convert_hemisphere(t);
}

void rmb::set_lon(const geo::longitude & t)
{
	lon_ = t;
	lon_hem_ = convert_hemisphere(t);
}

void rmb::append_data_to(std::string & s) const
{
	append(s, to_string(active_));
	append(s, to_string(cross_track_error_));
	append(s, to_string(steer_dir_));
	append(s, to_string(waypoint_to_));
	append(s, to_string(waypoint_from_));
	append(s, to_string(lat_));
	append(s, to_string(lat_hem_));
	append(s, to_string(lon_));
	append(s, to_string(lon_hem_));
	append(s, to_string(range_));
	append(s, to_string(bearing_));
	append(s, to_string(dst_velocity_));
	append(s, to_string(arrival_status_));
	append(s, to_string(mode_ind_));
}
}
}
