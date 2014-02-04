INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_GSM gsm)

FIND_PATH(
    GSM_INCLUDE_DIRS
    NAMES gsm/api.h
    HINTS $ENV{GSM_DIR}/include
        ${PC_GSM_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    GSM_LIBRARIES
    NAMES gnuradio-gsm
    HINTS $ENV{GSM_DIR}/lib
        ${PC_GSM_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(GSM DEFAULT_MSG GSM_LIBRARIES GSM_INCLUDE_DIRS)
MARK_AS_ADVANCED(GSM_LIBRARIES GSM_INCLUDE_DIRS)

