#include <ostream>
#include <cassert>

namespace KE::KOMATH
{
	struct Mat4x4
	{
		float Mat4[16];
		
		Mat4x4() = default;
		
		Mat4x4(
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3,
			float x4, float y4, float z4, float w4
		
		) : Mat4{ x1,y1,z1,w1,
				 x2,y2,z2,w2,
				 x3,y3,z3,w3,
				 x4,y4,z4,w4 }
		{
		};
			
		inline float& operator[](int i)
		{
			assert(i < 16 && i >= 0);
			return Mat4[i];
		}

		inline const float& operator[](int i) const
		{
			assert(i < 16 && i >= 0);
			return Mat4[i];
		}
	};
	
	inline std::ostream& operator<<(std::ostream& os, const Mat4x4& a)
	{
		for (int i = 0; i < 1; i++)
		{
			return os << "[" << a[i] << "," << a[i + 1] << "," << a[i + 2] << "," << a[i + 3] << "]" << "\n"
				<< "[" << a[i + 4] << "," << a[i + 5] << "," << a[i + 6] << "," << a[i + 7] << "]" << "\n"
				<< "[" << a[i + 8] << "," << a[i + 9] << "," << a[i + 10] << "," << a[i + 11] << "]" << "\n"
				<< "[" << a[i + 12] << "," << a[i + 13] << "," << a[i + 14] << "," << a[i + 15] << "]" << "\n";
		}
	}
}