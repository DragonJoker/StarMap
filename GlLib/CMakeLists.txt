project( GlLib )

file( GLOB ${PROJECT_NAME}_CPP_FILES *.cc *.cpp )
file( GLOB ${PROJECT_NAME}_C_FILES *.c )
file( GLOB ${PROJECT_NAME}_HDR_FILES *.h *.hh *.hpp *.inl *.inc )
file( GLOB ${PROJECT_NAME}_DBG_FILES *.natvis )

if ( DESKTOP )
	source_add_forced_include(
		"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
		${${PROJECT_NAME}_C_FILES}
	)
	source_add_forced_include(
		"${CMAKE_CURRENT_SOURCE_DIR}/pch.h"
		${${PROJECT_NAME}_CPP_FILES}
	)
	file( GLOB ${PROJECT_NAME}_GL_SRC_FILES GL/*.c )
	file( GLOB ${PROJECT_NAME}_GL_HDR_FILES GL/*.h GL/*.hh GL/*.hpp GL/*.inl GL/*.inc )
	set( ${PROJECT_NAME}_C_FILES
		${${PROJECT_NAME}_C_FILES}
		${${PROJECT_NAME}_GL_SRC_FILES}
	)
	set( ${PROJECT_NAME}_HDR_FILES
		${${PROJECT_NAME}_HDR_FILES}
		${${PROJECT_NAME}_GL_HDR_FILES}
	)
	add_definitions(
		-DGLEW_STATIC
	)
else ()
	add_forced_include( "${CMAKE_CURRENT_SOURCE_DIR}/pch.h" )
endif ()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library( ${PROJECT_NAME} STATIC ${${PROJECT_NAME}_CPP_FILES} ${${PROJECT_NAME}_C_FILES} ${${PROJECT_NAME}_HDR_FILES} ${${PROJECT_NAME}_DBG_FILES} )
add_target_precompiled_header( ${PROJECT_NAME} pch.h pch.cpp ${CMAKE_CXX_FLAGS} ${${PROJECT_NAME}_CPP_FILES} )