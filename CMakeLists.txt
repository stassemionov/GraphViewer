cmake_minimum_required(VERSION 2.8)

project(GraphViewer)

find_package(Qt5Widgets REQUIRED)

set(SOURCE_PATH "./source")
set(HEADER_PATH "./include")
file(GLOB_RECURSE SOURCE_FILES 	 "${SOURCE_PATH}/*.cpp")
file(GLOB_RECURSE HEADER_FILES 	 "${HEADER_PATH}/*.h")
file(GLOB_RECURSE UI_FILES 	   	 "./*.ui")
file(GLOB_RECURSE RESOURCE_FILES "./*.qrc")

include_directories(${HEADER_PATH})

qt5_wrap_cpp(	  MOC_WRAPPERS ${HEADER_FILES})
qt5_wrap_ui( 	  UIC_WRAPPERS ${UI_FILES})
qt5_add_resources(QRC_WRAPPERS ${RESOURCE_FILES}
				  OPTIONS -no-compress)

add_compile_options(-std=c++11 -Wall -O2)

add_executable(GraphViewer
		 	   	${SOURCE_FILES}
			   	${HEADER_FILES}
				${MOC_WRAPPERS}
				${UIC_WRAPPERS}
				${QRC_WRAPPERS})

target_link_libraries(GraphViewer Qt5::Widgets)

#######################################

install(TARGETS GraphViewer
		RUNTIME
		DESTINATION .)
get_property(LIB_QT5CORE_PATH
			 TARGET 	Qt5::Core
			 PROPERTY 	IMPORTED_LOCATION_RELEASE)
get_filename_component(QT_LIBRARY_DIR
					   "${LIB_QT5CORE_PATH}"
					   DIRECTORY)
include(DeployQt4)
install_qt4_executable(
		"GraphViewer${CMAKE_EXECUTABLE_SUFFIX}"
		"" "" "${QT_LIBRARY_DIR}")
