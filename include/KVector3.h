#pragma once
#include <ostream>
#include <cassert>


namespace KE::KOMATH
{

	struct Vec3D
	{
		float Vec3[3];

		//Constructers
		inline Vec3D() = default;
		inline Vec3D(float x, float y, float z) : Vec3{ x,y,z } {};

		inline const float& x() const { return Vec3[0]; }
		inline const float& y() const { return Vec3[1]; }
		inline const float& z() const { return Vec3[2]; }

		
		inline float& operator[](int i)
		{
			assert(i < 3 && i >= 0);
			return Vec3[i];
		}

		inline const float& operator[](int i) const
		{
			assert(i < 3 && i >= 0);
			return Vec3[i];
		}

		inline Vec3D operator *=(float s)
		{
			Vec3[0] *= s;
			Vec3[1] *= s;
			Vec3[2] *= s;

			return *this;
		}

		inline Vec3D operator /=(float s)
		{
			//reciprocal of s
			s = 1.0f / s;

			Vec3[0] /= s;
			Vec3[1] /= s;
			Vec3[2] /= s;

			return *this;
		}

		inline Vec3D operator *(float s)
		{
			Vec3[0] * s;
			Vec3[1] * s;
			Vec3[2] * s;

			return *this;
		}

		inline Vec3D operator +=(float s)
		{
			Vec3[0] += s;
			Vec3[1] += s;
			Vec3[2] += s;

			return *this;
		}

		inline Vec3D operator -=(float s)
		{
			Vec3[0] -= s;
			Vec3[1] -= s;
			Vec3[2] -= s;

			return *this;
		}
	};
	
	inline std::ostream& operator<<(std::ostream& os, const Vec3D& a)
	{
		return os << "(" << a.x() << ", " << a.y() << ", " << a.z() << ")" << "\n";
	}
	
}