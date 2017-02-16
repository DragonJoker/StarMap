project( StarMapTestApp )

# file( GLOB ${PROJECT_NAME}_ASSETS ${CMAKE_CURRENT_SOURCE_DIR}/assets/*.* )
# file( GLOB_RECURSE ${PROJECT_NAME}_RES
	# RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/res
	# ${CMAKE_CURRENT_SOURCE_DIR}/res/*.* )

# foreach ( _FILE ${${PROJECT_NAME}_ASSETS} )
	# get_filename_component( _FILE ${_FILE} NAME )
	# set( PROJECT_ASSETS "${PROJECT_ASSETS}\n    <Content Include=\"assets\\${_FILE}\" />" )
# endforeach ()

# foreach ( _FILE ${${PROJECT_NAME}_RES} )
  # file( TO_NATIVE_PATH ${_FILE} _FILE )
	# set( PROJECT_RES "${PROJECT_RES}\n    <Content Include=\"res\\${_FILE}\" />" )
# endforeach ()

# configure_file( ${CMAKE_TEMPLATES_DIR}/Msvc/App.androidproj
	# ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.androidproj
# )

# configure_file( ${CMAKE_TEMPLATES_DIR}/Msvc/build.xml
	# ${CMAKE_CURRENT_SOURCE_DIR}/build.xml
	# @ONLY
# )

# configure_file( ${CMAKE_TEMPLATES_DIR}/Msvc/project.properties
	# ${CMAKE_CURRENT_SOURCE_DIR}/project.properties
# )

# configure_file( ${CMAKE_TEMPLATES_DIR}/Msvc/AndroidManifest.xml
	# ${CMAKE_CURRENT_SOURCE_DIR}/AndroidManifest.xml
# )

include_external_msproject(
	${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.androidproj
	TYPE FAE04EC0-301F-11D3-BF4B-00C04F79EFBC
	#TYPE 39E2626F-3545-4960-A6E8-258AD8476CE5
	#GUID 10DD81DC-585F-4214-982B-ECC6A7217987
	#${PROJECT_NAME}.NativeActivity
)
# add_dependencies( ${PROJECT_NAME}
# )