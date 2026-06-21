
#include "KWindow.hpp"
#include "KVector3.hpp"
#include "KMat4.hpp"

#include <iostream>

int main()
{
	Kos::KWindow _win;

	_win.Create("KOS", 800, 800);
	
	

	while (true)
	{
		_win.ProcessMessages();
		
		if (!IsWindow(_win.GetWindowHandle()))
		{
			break;
		}
	}


}