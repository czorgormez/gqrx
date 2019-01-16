#--------------------------------------------------------------------------------
#
# Qmake project file for gqrx - http://gqrx.dk
#
# Common options you may want to passs to qmake:
#
#   AUDIO_BACKEND=portaudio     Use portaudio backend
#   CONFIG+=debug          Enable debug mode
#   PREFIX=/some/prefix    Installation prefix
#   BOOST_SUFFIX=-mt       To link against libboost-xyz-mt (needed for pybombs)
#--------------------------------------------------------------------------------

QT       += core gui widgets svg

lessThan(QT_MAJOR_VERSION,5) {
    error("Gqrx requires Qt 5.")
}

PKGCONFIG_EXISTS = $$system(pkg-config --version)
isEmpty(PKGCONFIG_EXISTS) {
    error("Gqrx requires pkg-config to build.")
}

TEMPLATE = app

macx {
    TARGET = Gqrx
    ICON = resources/icons/gqrx.icns
    DEFINES += GQRX_OS_MACX
} else {
    TARGET = gqrx
}

# enable pkg-config to find dependencies
CONFIG += link_pkgconfig


RESOURCES += \
    resources/icons.qrc \
    resources/textfiles.qrc

# make clean target
QMAKE_CLEAN += gqrx

# make install target
isEmpty(PREFIX) {
    message("No prefix given. Using /usr/local")
    PREFIX=/usr/local
}

target.path  = $$PREFIX/bin
INSTALLS    += target 

#CONFIG += debug

# disable debug messages in release
CONFIG(debug, debug|release) {
    # Use for valgrind
    #QMAKE_CFLAGS_DEBUG += '-g -O0'

    # Define version string (see below for releases)
    VER = $$system(git describe --abbrev=8)
    ##VER = 2.11.5

} else {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    VER = $$system(git describe --abbrev=1)
    ##VER = 2.11.5

    # Release binaries with gr bundled
    # QMAKE_RPATH & co won't work with origin
    ## QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/lib\''
}


# Tip from: http://www.qtcentre.org/wiki/index.php?title=Version_numbering_using_QMake
VERSTR = '\\"$${VER}\\"'          # place quotes around the version string
DEFINES += VERSION=\"$${VERSTR}\" # create a VERSION macro containing the version string

SOURCES += \
    src/applications/gqrx/main.cpp \
    src/applications/gqrx/mainwindow.cpp \
    src/applications/gqrx/receiver.cpp \
    src/qtgui/ctk/ctkRangeSlider.cpp \
    src/qtgui/dockinputctl.cpp \
    src/qtgui/dockfft.cpp \
    src/qtgui/freqctrl.cpp \
    src/qtgui/ioconfig.cpp \
    src/qtgui/meter.cpp \
    src/qtgui/plotter.cpp \
    src/qtgui/qtcolorpicker.cpp \

HEADERS += \
    src/applications/gqrx/gqrx.h \
    src/applications/gqrx/mainwindow.h \
    src/applications/gqrx/receiver.h \
    src/qtgui/ctk/ctkPimpl.h \
    src/qtgui/ctk/ctkRangeSlider.h \
    src/qtgui/dockfft.h \
    src/qtgui/dockinputctl.h \
    src/qtgui/freqctrl.h \
    src/qtgui/ioconfig.h \
    src/qtgui/meter.h \
    src/qtgui/plotter.h \
    src/qtgui/qtcolorpicker.h \

FORMS += \
    src/applications/gqrx/mainwindow.ui \
    src/qtgui/dockfft.ui \
    src/qtgui/dockinputctl.ui \
    src/qtgui/ioconfig.ui \

INCPATH += src/


OTHER_FILES += \
    bookmarks.csv \
    gqrx.desktop \
    README.md \
    COPYING \
    news.txt

LIBS += -lSoapySDR
