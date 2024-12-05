cmake_minimum_required(VERSION 3.16)
project(args
    VERSION     3.3.1
    LANGUAGES   C
    DESCRIPTION "An argument-parsing library for C."
)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ------------------------------------------------ #
#  Setting a variable for the name of the library  #
# ------------------------------------------------ #
set(LIB_NAME args)

# ---------------------------------- #
#  Set C standard and compile flags  #
# ---------------------------------- #

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS_COMMANDS
    "${CMAKE_C_FLAGS} -Wall -Wextra --pedantic -Wno-unused-parameter")

add_library(${LIB_NAME}
    src/args.c
)
    
target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# ------------------------- #
#  Set installation config  #
# ------------------------- #

install(TARGETS         ${LIB_NAME}
    EXPORT              ${LIB_NAME}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES src/args.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# --------------------------------------------------------------- #
#  Export targets so that they can be fetched by another project  #
# --------------------------------------------------------------- #

install(EXPORT  ${LIB_NAME}Targets
    FILE        ${LIB_NAME}Targets.cmake
    NAMESPACE   dmullholl::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
)

configure_package_config_file(
    cmake/argsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${LIB_NAME}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${LIB_NAME}
)

# ----------------------------------- #
#  Special config for build and test  #
# ----------------------------------- #

include(cmake/tests.cmake)
include(cmake/examples.cmake)
