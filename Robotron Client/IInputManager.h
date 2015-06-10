#include <dinput.h>


class IInputManager
{
public:
	IInputManager()
	{
	}

	virtual ~IInputManager()
	{
	}

	virtual bool Initialise() = 0;
	virtual bool Shutdown() = 0;
};