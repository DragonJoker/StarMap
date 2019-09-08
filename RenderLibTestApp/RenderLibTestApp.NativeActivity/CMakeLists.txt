project( RenderLibTestApp.NativeActivity )

set( ${PROJECT_NAME}_SRC_FILES 
	${CMAKE_CURRENT_SOURCE_DIR}/Engine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set( ${PROJECT_NAME}_HDR_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/Engine.h
	${CMAKE_CURRENT_SOURCE_DIR}/pch.h
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/pch.h" )

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/../..
	${CMAKE_CURRENT_SOURCE_DIR}
	${OPENGLES3_INCLUDE_DIR}
)

add_library( ${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC_FILES} ${${PROJECT_NAME}_HDR_FILES} )
target_link_libraries( ${PROJECT_NAME}
	native_app_glue
	AndroidUtils
	FreeType
	RenderLib
	GlLib
	EGL
	${OPENGLES3_LIBRARY}
	log
	android
)
add_dependencies( ${PROJECT_NAME}
	AndroidUtils
)
