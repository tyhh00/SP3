/******************************************************************************/
/*!
\file	Debug.h
\author Tan Yong Hong
\brief
Handles all Debug Mode COUT Tests.

Used to prevent Couts from occuring in release builds.
*/
/******************************************************************************/
#include <iostream>
#ifdef _DEBUG
#define DEBUG_MSG(str) do { std::cout << "LOG: " << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false ) //Forces a ';' at the end of the macro call.
#endif