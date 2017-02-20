project( StarMapLibTestApp )

file( GLOB ${PROJECT_NAME}_SRC_FILES *.c *.cc *.cpp )
file( GLOB ${PROJECT_NAME}_HDR_FILES *.h *.hh *.hpp *.inl *.inc )

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set( TARGET_RSC
	${CMAKE_CURRENT_SOURCE_DIR}/StarMapLibDesktopTestApp.rc
)
source_group( "Resource Files" FILES ${TARGET_RSC} )
set( ${PROJECT_NAME}_HDR_FILES
	${${PROJECT_NAME}_HDR_FILES}
	${TARGET_RSC}
)
add_executable( ${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_SRC_FILES} ${${PROJECT_NAME}_HDR_FILES} )
target_link_libraries( ${PROJECT_NAME}
	GlLib
	RenderLib
	FreeType
	DesktopUtils
	StarMapLib
	opengl32
)
add_dependencies( ${PROJECT_NAME}
	DesktopUtils
	StarMapLib
)
file( GLOB ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/../StarMapTestApp/assets/*.* )

copy_target_files( ${PROJECT_NAME} "" ${ASSETS} )

if ( WIN32 )
	configure_file( ${CMAKE_SOURCE_DIR}/CMake/Templates/manifest.bin
		${CMAKE_CURRENT_BINARY_DIR}/manifest.bin )
endif ()