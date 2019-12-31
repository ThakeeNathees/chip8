#pragma once



#ifdef _WIN32 // WINDOWS
	
	#define NOMINMAX
	#include <windows.h>
	
	#ifndef __MINGW32__ // Visual Studio
/* using sfml 2.5.1

bin output dir		: $(SolutionDir)bin\$(Configuration)-$(Platform)\$(ProjectName)\
bin-int output dir	: $(SolutionDir)bin-int\$(Configuration)-$(Platform)\$(ProjectName)\

addnl include dirs	: $(ProjectDir)vender/SFML-2.5.1/include
addnl linking dirs	: $(ProjectDir)vender/SFML-2.5.1/lib
*/
		#pragma comment(lib, "opengl32.lib")
		#pragma comment(lib, "user32.lib")
		#pragma comment(lib, "gdi32.lib")
		
		#ifdef _DEBUG
			#pragma comment(lib, "sfml-main-d.lib")
			#pragma comment(lib, "sfml-audio-d.lib")
			#pragma comment(lib, "sfml-graphics-d.lib")
			#pragma comment(lib, "sfml-network-d.lib")
			#pragma comment(lib, "sfml-window-d.lib")
			#pragma comment(lib, "sfml-system-d.lib")
		#else
			#pragma comment(lib, "sfml-main.lib")
			#pragma comment(lib, "sfml-audio.lib")
			#pragma comment(lib, "sfml-graphics.lib")
			#pragma comment(lib, "sfml-network.lib")
			#pragma comment(lib, "sfml-window.lib")
			#pragma comment(lib, "sfml-system.lib")
		#endif 
	#endif
	

#endif

#ifdef __linux__ // LINUX
#endif


#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
