#pragma once
#include <ostream>
#include <cassert>


namespace Kos::math
{
	
	struct KVector3
	{

		inline float* Data() { return &x; };

		float x, y, z;

		static const KVector3 unit_vector;
		static const KVector3 zero_vector;

		inline KVector3(float x, float y, float z) 
		{
			this->x = x;
			this->y = y;
			this->z = z;
		};

		inline KVector3 operator *=(float s)
		{
			this->x *= s;
			this->y *= s;
			this->z *= s;

			return *this;
		}

		inline KVector3 operator /=(float s)
		{
			//reciprocal of s
			s = 1.0f / s;

			this->x *= s;
			this->y *= s;
			this->z *= s;

			return *this;
		}

		inline KVector3 operator *(float s)
		{
			this->x* s;
			this->y* s;
			this->z* s;

			return *this;
		}

		inline KVector3 operator +=(float s)
		{
			this->x += s;
			this->y += s;
			this->z += s;

			return *this;
		}

		inline KVector3 operator -=(float s)
		{
			this->x -= s;
			this->y -= s;
			this->z -= s;

			return *this;
		}

		//Allows vector data to be treated like an array of floats
	};
	
	inline std::ostream& operator<<(std::ostream& os, const KVector3& a)
	{
		return os << "(" << a.x << ", " << a.y << ", " << a.z << ")" << "\n";
	}
	
}