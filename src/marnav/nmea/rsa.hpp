#ifndef __NMEA__RSA__HPP__
#define __NMEA__RSA__HPP__

#include <marnav/nmea/sentence.hpp>
#include <marnav/utils/optional.hpp>

namespace marnav
{
namespace nmea
{

/// @brief RSA - Rudder Sensor Angle
///
/// @code
///        1   2 3   4
///        |   | |   |
/// $--RSA,x.x,A,x.x,A*hh<CR><LF>
/// @endcode
///
/// Field Number:
/// 1. Starboard (or single) rudder sensor, "-" means Turn To Port
/// 2. Status, A means data is valid
///    - A = data is valid
///    - V = invalid
/// 3. Port rudder sensor
/// 4. Status, A means data is valid
///    - A = data is valid
///    - V = invalid
///
class rsa : public sentence
{
public:
	constexpr static const sentence_id ID = sentence_id::RSA;
	constexpr static const char * TAG = "RSA";

	rsa();
	rsa(const rsa &) = default;
	rsa & operator=(const rsa &) = default;

	static std::unique_ptr<sentence> parse(const std::string & talker,
		const std::vector<std::string> & fields) throw(std::invalid_argument);

protected:
	virtual std::vector<std::string> get_data() const override;

private:
	utils::optional<double> rudder1;
	utils::optional<status> rudder1_valid;
	utils::optional<double> rudder2;
	utils::optional<status> rudder2_valid;

public:
	NMEA_GETTER(rudder1)
	NMEA_GETTER(rudder1_valid)
	NMEA_GETTER(rudder2)
	NMEA_GETTER(rudder2_valid)

	void set_rudder1(double t);
	void set_rudder2(double t);
};
}
}

#endif
