
#include "KWindow.hpp"
#include "KVector3.hpp"
#include "KOptional.hpp"
#include "KMat4.hpp"
#include "KLog.hpp"

#include <iostream>

int main()
{
	Kos::KWindow _win;

	_win.Create("KOS", 800, 800);
	
	Kos::math::KVector3 vec(2.0f, 0.0f, 0.0f);
	std::cout << vec << "\n";

	std::cout << Kos::math::KVector3::unit_vector << "\n";

	Kos::KOptional<uint32_t> value;

	std::cout << value.HasValue() << std::endl;


	//Kos::KLog::WriteLog(Kos::LogType::Info, "Does my logger work?");

	while (true)
	{
		_win.ProcessMessages();
		
		if (!IsWindow(_win.GetWindowHandle()))
		{
			break;
		}
	}


}