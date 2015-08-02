#ifndef __MATH__QUATERNION_HPP__
#define __MATH__QUATERNION_HPP__

#include <marnav/math/vectors.hpp>
#include <cmath>
#include <cassert>

namespace marnav
{
namespace math
{

class quaternion
{
public:
	using value_type = double;

	enum class part { w, x, y, z };

	quaternion(value_type w = 0.0, value_type x = 0.0, value_type y = 0.0, value_type z = 0.0)
		: w(w)
		, x(x)
		, y(y)
		, z(z)
	{
	}
	quaternion(const quaternion &) = default;
	quaternion(quaternion &&) = default;
	quaternion(value_type yaw, value_type pitch, value_type roll) { euler(yaw, pitch, roll); }
	quaternion(const vec3 & v) { set(0.0, v[0], v[1], v[2]); }
	quaternion & set(value_type a, value_type b, value_type c, value_type d)
	{
		w = a;
		x = b;
		y = c;
		z = d;
		return *this;
	}
	quaternion(value_type angle, const vec3 & axis)
	{
		value_type d = axis.length();
		assert(d != 0.0 && "quaternion Axis is Zero");
		angle *= (M_PI / 180.0);
		value_type s = sin(angle * 0.5) / d;
		set(cos(angle * 0.5), axis[0] * s, axis[1] * s, axis[2] * s);
	}
	quaternion & euler(value_type yaw, value_type pitch, value_type roll)
	{
		yaw *= (M_PI / 180.0) / 2.0;
		pitch *= (M_PI / 180.0) / 2.0;
		roll *= (M_PI / 180.0) / 2.0;
		value_type c_yaw = cos(yaw);
		value_type s_yaw = sin(yaw);
		value_type c_pitch = cos(pitch);
		value_type s_pitch = sin(pitch);
		value_type c_roll = cos(roll);
		value_type s_roll = sin(roll);
		set(c_roll * c_pitch * c_yaw + s_roll * s_pitch * s_yaw,
			c_roll * s_pitch * c_yaw + s_roll * c_pitch * s_yaw,
			c_roll * c_pitch * s_yaw - s_roll * s_pitch * c_yaw,
			s_roll * c_pitch * c_yaw - c_roll * s_pitch * s_yaw);
		return *this;
	}
	inline value_type dot(const quaternion & q)
	{
		return (w * q.w + x * q.x + y * q.y + z * q.z);
	}
	inline quaternion & normalize(value_type len = 1.0)
	{
		value_type l = length();
		return (l != 0.0) ? (*this *= (len / l)) : (*this);
	}
	inline value_type length() const { return sqrt(length2()); }
	inline value_type length2() const { return w * w + x * x + y * y + z * z; }
	inline vec3 getvec3() const { return vec3(x, y, z); }
	inline vec4 getvec4() const { return vec4(w, x, y, z); }
	inline vec3 & rot(vec3 & v) const
	{
		v = ((*this * quaternion(v)) * quaternion(w, -x, -y, -z)).getvec3();
		v *= 1.0 / length2();
		return v;
	}
	inline vec3 rot(const vec3 & v) const
	{
		vec3 t(v);
		return rot(t);
	}
	inline value_type operator[](part p) const
	{
		switch (p) {
			case part::w:
				return w;
			case part::x:
				return x;
			case part::y:
				return y;
			case part::z:
				return z;
		}
		return 0.0;
	}
	inline quaternion & operator=(const quaternion &) = default;
	inline quaternion & operator=(quaternion &&) = default;
	inline bool operator==(const quaternion & q)
	{
		return (w == q.w) && (x == q.x) && (y == q.y) && (z == q.z);
	}
	inline quaternion & operator*=(const quaternion & q)
	{
		return * this = quaternion(*this) *= q;
	}
	inline quaternion & operator*=(value_type s)
	{
		w *= s;
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline quaternion & operator+=(const quaternion & q)
	{
		w += q.w;
		x += q.x;
		y += q.y;
		z += q.z;
		return *this;
	}
	inline quaternion & operator-=(const quaternion & q)
	{
		w -= q.w;
		x -= q.x;
		y -= q.y;
		z -= q.z;
		return *this;
	}
	friend quaternion operator+(const quaternion & a, const quaternion & b)
	{
		return quaternion(a) += b;
	}
	friend quaternion operator-(const quaternion & a, const quaternion & b)
	{
		return quaternion(a) -= b;
	}
	friend quaternion operator*(const quaternion & a, const quaternion & b)
	{
		return quaternion(a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z,
			a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y,
			a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z,
			a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x);
	}
	friend quaternion operator*(const quaternion & q, value_type s)
	{
		return quaternion(q) *= s;
	}
	friend quaternion operator*(value_type s, const quaternion & q)
	{
		return quaternion(q) *= s;
	}
	friend quaternion operator*(const vec3 & v, const quaternion & q)
	{
		return quaternion(0, v) *= q;
	}
	friend quaternion operator*(const quaternion & q, const vec3 & v)
	{
		return q * quaternion(0, v);
	}

	static quaternion rot_to(const vec3 & from, const vec3 & to)
	{
		vec3 f = from.normalize();
		vec3 t = to.normalize();
		value_type d = f.dot(t);
		if (d >= 1.0)
			return quaternion(1.0, 0.0, 0.0, 0.0); // identiy quaternion
		if (d < std::numeric_limits<value_type>::epsilon())
			return quaternion(1.0, 0.0, 0.0, 0.0);
		value_type s = 1.0 / sqrt((1 + d) * 2.0); // TODO: why?
		vec3 c = s * f.cross(t);
		return quaternion(s / 2.0, c).normalize();
	}

private:
	value_type w, x, y, z;
};

}
}

#endif
