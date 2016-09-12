#include "apa.hpp"
#include <marnav/nmea/checks.hpp>
#include <marnav/nmea/io.hpp>

namespace marnav
{
namespace nmea
{
MARNAV_NMEA_DEFINE_SENTENCE_PARSE_FUNC(apa)

constexpr const char * apa::TAG;

apa::apa()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

apa::apa(const std::string & talker, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talker)
{
	if (std::distance(first, last) != 10)
		throw std::invalid_argument{"invalid number of fields in apa"};

	read(*(first + 0), loran_c_blink_warning);
	read(*(first + 1), loran_c_cycle_lock_warning);
	read(*(first + 2), cross_track_error_magnitude);
	read(*(first + 3), direction_to_steer);
	read(*(first + 4), cross_track_unit);
	read(*(first + 5), status_arrival);
	read(*(first + 6), status_perpendicular_passing);
	read(*(first + 7), bearing_origin_to_destination);
	read(*(first + 8), bearing_origin_to_destination_ref);
	read(*(first + 9), waypoint_id);

	check();
}

void apa::set_bearing_origin_to_destination(double t, reference ref)
{
	check_value(bearing_origin_to_destination_ref, {reference::TRUE, reference::MAGNETIC},
		"bearing_origin_to_destination_ref");
	bearing_origin_to_destination = t;
	bearing_origin_to_destination_ref = ref;
}

void apa::check() const
{
	check_status(loran_c_blink_warning, "loran_c_blink_warning");
	check_status(loran_c_cycle_lock_warning, "loran_c_cycle_lock_warning");

	check_value(direction_to_steer, {side::left, side::right}, "direction_to_steer");
	check_value(cross_track_unit, {unit::distance::nm}, "cross_talk_unit");
	check_status(status_arrival, "status_arrival");
	check_status(status_perpendicular_passing, "status_perpendicular_passing");

	if (bearing_origin_to_destination && !bearing_origin_to_destination_ref)
		throw std::invalid_argument{"missing bearing_origin_to_destination_ref"};
	check_value(bearing_origin_to_destination_ref, {reference::TRUE, reference::MAGNETIC},
		"bearing_origin_to_destination_ref");
}

std::vector<std::string> apa::get_data() const
{
	return {to_string(loran_c_blink_warning), to_string(loran_c_cycle_lock_warning),
		format(cross_track_error_magnitude, 2), to_string(direction_to_steer),
		to_string(cross_track_unit), to_string(status_arrival),
		to_string(status_perpendicular_passing), format(bearing_origin_to_destination, 1),
		to_string(bearing_origin_to_destination_ref), to_string(waypoint_id)};
}
}
}