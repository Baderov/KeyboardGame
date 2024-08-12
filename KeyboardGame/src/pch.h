#ifndef PCH_H
#define PCH_H

#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/String.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp> 
#include <windows.h>
#include <filesystem>
#include <sqlext.h>
#include <random>
#undef MessageBox

#ifdef _DEBUG
#define DEBUG_MSG(str) do { std::wcout << str << "\n"; } while(false)
#else
#define DEBUG_MSG(str) do { } while (false)
#endif

#endif