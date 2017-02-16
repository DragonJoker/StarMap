project( StarMapLib )

file( GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES *.c *.cc *.cpp )
file( GLOB_RECURSE ${PROJECT_NAME}_HDR_FILES *.h *.hh *.hpp *.inl *.inc )

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/RenderLib
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_forced_include( "${CMAKE_CURRENT_SOURCE_DIR}/pch.h" )
add_library( ${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SRC_FILES} ${${PROJECT_NAME}_HDR_FILES} )
add_dependencies( ${PROJECT_NAME}
	GlLib
	RenderLib
)
