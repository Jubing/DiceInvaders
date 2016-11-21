#include <windows.h>
#include <cassert>
#include <cstdio>
#include "DiceInvaders.h"
#include "game.h"

class DiceInvadersLib
{
public:
	explicit DiceInvadersLib(const char* libraryPath)
	{
		m_lib = LoadLibrary(libraryPath);
		assert(m_lib);

		DiceInvadersFactoryType* factory = (DiceInvadersFactoryType*)GetProcAddress(
				m_lib, "DiceInvadersFactory");
		m_interface = factory();
		assert(m_interface);
	}

	~DiceInvadersLib()
	{
		FreeLibrary(m_lib);
	}

	IDiceInvaders* get() const 
	{
		return m_interface;
	}

private: 
	DiceInvadersLib(const DiceInvadersLib&);
	DiceInvadersLib& operator=(const DiceInvadersLib&);

private: 
	IDiceInvaders* m_interface;
	HMODULE m_lib;
};


int APIENTRY WinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	LPSTR commandLine,
	int commandShow)
{
	DiceInvadersLib lib("DiceInvaders.dll");
	IDiceInvaders* system = lib.get();
	
	Game* game= new Game(system);
	game->update(system);

	delete game;
	system->destroy();

	return 0;
}



