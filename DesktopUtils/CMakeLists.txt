project( DesktopUtils )

file( GLOB ${PROJECT_NAME}_SRC_FILES *.c *.cc *.cpp )
file( GLOB ${PROJECT_NAME}_HDR_FILES *.h *.hh *.hpp *.inl *.inc )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/..
	${CMAKE_CURRENT_SOURCE_DIR}/../FreeType/include
	${CMAKE_CURRENT_SOURCE_DIR}/../FreeType/include/freetype
)

add_forced_include( "${CMAKE_CURRENT_SOURCE_DIR}/pch.h" )
add_library( ${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC_FILES} ${${PROJECT_NAME}_HDR_FILES} )
add_dependencies( ${PROJECT_NAME}
	RenderLib
	FreeType
)
