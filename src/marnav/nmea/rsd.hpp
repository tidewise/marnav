#ifndef __MARNAV__NMEA__RSD__HPP__
#define __MARNAV__NMEA__RSD__HPP__

#include <array>
#include <marnav/nmea/sentence.hpp>
#include <marnav/utils/optional.hpp>

namespace marnav
{
namespace nmea
{
MARNAV_NMEA_DECLARE_SENTENCE_PARSE_FUNC(rsd)

/// @brief RSD - RADAR System Data
///
/// @code
///        1   2   3   4   5   6   7   8   9   10  11 12 13
///        |   |   |   |   |   |   |   |   |   |   |   | |
/// $--RSD,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,x.x,a,a*hh<CR><LF>
/// @endcode
///
/// Some fields are missing from this description
///
/// Field Number:
/// 1.  Unknown
/// 2.  Unknown
/// 3.  Unknown
/// 4.  Unknown
/// 5.  Unknown
/// 6.  Unknown
/// 7.  Unknown
/// 8.  Unknown
/// 9.  Cursor Range From Own Ship
/// 10. Cursor Bearing Degrees Clockwise From Zero
/// 11. Range Scale
/// 12. Range Units
/// 13. Unknown
///
class rsd : public sentence
{
	MARNAV_NMEA_SENTENCE_FRIENDS(rsd)

public:
	constexpr static const sentence_id ID = sentence_id::RSD;
	constexpr static const char * TAG = "RSD";

	rsd();
	rsd(const rsd &) = default;
	rsd & operator=(const rsd &) = default;
	rsd(rsd &&) = default;
	rsd & operator=(rsd &&) = default;

protected:
	rsd(const std::string & talker, fields::const_iterator first, fields::const_iterator last);
	virtual std::vector<std::string> get_data() const override;

private:
	std::array<double, 8> unknowns;
	double cursor_range = 0.0;
	double cursor_bearing = 0.0;
	double range_scale = 0.0;
	char range_unit = 'V';
	char unknown = 'V';

public:
	decltype(cursor_range) get_cursor_range() const { return cursor_range; }
	decltype(cursor_bearing) get_cursor_bearing() const { return cursor_bearing; }
	decltype(range_scale) get_range_scale() const { return range_scale; }
	decltype(range_unit) get_range_unit() const { return range_unit; }
	decltype(unknown) get_unknown() const { return unknown; }

	void set_unknowns(std::array<double, 8> t) noexcept { unknowns = t; }
	void set_cursor(double range, double bearing) noexcept;
	void set_range(double scale, char unit) noexcept;
	void set_unknown(char t) noexcept { unknown = t; }
};
}
}

#endif
