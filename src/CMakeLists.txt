#######################################################################################################################
# Add the source subdirectories
add_subdirectory(applications)
# add_subdirectory(dsp)
# add_subdirectory(interfaces)
add_subdirectory(qtgui)
# add_subdirectory(receivers)

#######################################################################################################################
# bring in the global properties
get_property(${PROJECT_NAME}_SOURCE GLOBAL PROPERTY SRCS_LIST)
get_property(${PROJECT_NAME}_UI_SOURCE GLOBAL PROPERTY UI_SRCS_LIST)


#######################################################################################################################
# Process the UI files
QT5_WRAP_UI(UIS_HDRS
    ${${PROJECT_NAME}_UI_SOURCE}
)

#######################################################################################################################
# Process the resources
QT5_ADD_RESOURCES(RESOURCES_LIST
    ../resources/icons.qrc
    ../resources/textfiles.qrc
)

########################################################################
# Resource file - adds an icon to GQRX executable
if (WIN32)
    set(ICON_SOURCE ${CMAKE_SOURCE_DIR}/resources/icons/gqrx.ico)
    set(RES_FILES "${CMAKE_CURRENT_BINARY_DIR}/gqrx.rc")
    file(WRITE "${RES_FILES}" "id ICON \"${ICON_SOURCE}\"")
    set(CMAKE_RC_COMPILER_INIT windres)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    list(APPEND RESOURCES_LIST ${RES_FILES})
endif(WIN32)

#######################################################################################################################
# Build the program
add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE} ${UIS_HDRS} ${RESOURCES_LIST})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
# The pulse libraries are only needed on Linux. On other platforms they will not be found, so having them here is fine.
target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Network
    Qt5::Widgets
    Qt5::Svg
    ${Boost_LIBRARIES}
)

#build a win32 app, not a console app
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "/entry:mainCRTStartup ${CMAKE_EXE_LINKER_FLAGS}")
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE ON)
endif (WIN32)

set(INSTALL_DEFAULT_BINDIR "bin" CACHE STRING "Appended to CMAKE_INSTALL_PREFIX")
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${INSTALL_DEFAULT_BINDIR})
