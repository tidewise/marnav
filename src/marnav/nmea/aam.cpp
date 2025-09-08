#include <marnav/nmea/aam.hpp>
#include "checks.hpp"
#include <marnav/nmea/io.hpp>
#include <stdexcept>

namespace marnav
{
namespace nmea
{
constexpr sentence_id aam::ID;
constexpr const char * aam::TAG;

aam::aam()
	: sentence(ID, TAG, talker_id::global_positioning_system)
{
}

aam::aam(talker talk, fields::const_iterator first, fields::const_iterator last)
	: sentence(ID, TAG, talk)
{
	if (std::distance(first, last) != 5)
		throw std::invalid_argument{"invalid number of fields in aam"};

	read(*(first + 0), arrival_circle_entered_);
	read(*(first + 1), perpendicualar_passed_);
	read(*(first + 2), arrival_circle_radius_);
	read(*(first + 3), arrival_circle_radius_unit_);
	read(*(first + 4), waypoint_id_);

	check();
}

void aam::set_arrival_circle_entered(status s)
{
	check_status(s, "arrival_circle_entered");
	arrival_circle_entered_ = s;
}

void aam::set_perpendicular_passed(status s)
{
	check_status(s, "perpendicualar_passed");
	perpendicualar_passed_ = s;
}

void aam::set_arrival_circle_radius(double t)
{
	if (t < 0.0)
		throw std::invalid_argument{"invalid argument, arrival radius below zero"};
	arrival_circle_radius_ = t;
	arrival_circle_radius_unit_ = unit::distance::nm;
}

void aam::check() const
{
	check_status(arrival_circle_entered_, "arrival_circle_entered");
	check_status(perpendicualar_passed_, "perpendicualar_passed");
	if (arrival_circle_radius_ && !arrival_circle_radius_unit_)
		throw std::invalid_argument{"unit missing in sentence"};
	check_value(
		arrival_circle_radius_unit_, {unit::distance::nm}, "arrival_circle_radius_unit");
}

void aam::append_data_to(std::string & s) const
{
	append(s, to_string(arrival_circle_entered_));
	append(s, to_string(perpendicualar_passed_));
	append(s, to_string(arrival_circle_radius_));
	append(s, to_string(arrival_circle_radius_unit_));
	append(s, to_string(waypoint_id_));
}
}
}
