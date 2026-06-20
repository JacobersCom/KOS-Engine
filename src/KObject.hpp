#pragma once

#include <iostream>


namespace Kos
{
	/*
	*KObject is a base class for everything thing else in the engine.
	*This class is the base for KOS's reflection system
	*/

	class KObject
	{
	public:

		KObject() { mType.classID = 0; }
		virtual ~KObject() = default;

		struct Type
		{
			uint32_t classID;
			std::string className;
		};

		/*Returns the class name and ID. Unique to each class in the engine*/
		//KObject::Type KType();

	protected:
		
		Type mType;
	};
}