if ( ANDROID )
	add_subdirectory( StarMapTestApp.NativeActivity )
	add_subdirectory( StarMapTestApp )
else ()
	add_subdirectory( StarMapLibDesktopTestApp )
endif ()
