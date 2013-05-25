#pragma once

namespace playstate
{
	//
	// Base class for resource objects. If we want an instance type to be avaiable via the ResourceManager
	// then it must implement this class.
	class ResourceObject
	{
	public:
		virtual ~ResourceObject() {}
	};
}

