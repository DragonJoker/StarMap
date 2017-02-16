// RenderLibDesktopTestApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "StarMapLibDesktopTestApp.h"
#include "Engine.h"

namespace
{
	std::string doLoadString( HINSTANCE instance
		, int resourceID )
	{
		static constexpr int MaxSize = 100;
		std::string result;
		result.resize( MaxSize + 1 );
		auto count = ::LoadStringA( instance, resourceID, &result[0], MaxSize );
		result.resize( count + 1 );
		return result;
	}
}

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	Window window;

	window.create( hInstance
		, doLoadString( hInstance, IDC_STARMAPLIBDESKTOPTESTAPP )
		, IDI_STARMAPLIBDESKTOPTESTAPP
		, IDI_SMALL
		, doLoadString( hInstance, IDS_APP_TITLE ) );

	MSG msg;

	// Main message loop:
	while ( GetMessage( &msg, nullptr, 0, 0 ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	return ( int )msg.wParam;
}
