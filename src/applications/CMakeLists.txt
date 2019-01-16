#######################################################################################################################
# Add the source files to SRCS_LIST
add_source_files(SRCS_LIST
	gqrx/gqrx.h
	gqrx/main.cpp
	gqrx/mainwindow.cpp
	gqrx/mainwindow.h
	gqrx/receiver.cpp
	gqrx/receiver.h
)

#######################################################################################################################
# Add the UI files to UI_SRCS_LIST
add_source_files(UI_SRCS_LIST
	gqrx/mainwindow.ui
	gqrx/remote_control_settings.ui
)
