#================= Project Setup ==========================

# CMake
cmake_minimum_required(VERSION 3.24.0)

# Project
# NOTE: DON'T USE TRAILING ZEROS IN VERSIONS
set(STAR_CALC_BASE_VERSION 0.1) # Required for CI/CD
project(StarCalc
    VERSION ${STAR_CALC_BASE_VERSION}
    LANGUAGES CXX
    DESCRIPTION "INSERT DESCRIPTION HERE"
)
set(FORMAL_NAME "FORMAL NAME")
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LC)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

# C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build augmentation
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    enable_language("RC")
endif()

# Add local modules
set(PROJ_SCRIPTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${PROJ_SCRIPTS_PATH}/module")

# Get helper scripts
include(StarCalc/FetchOBCMake)
fetch_ob_cmake("v0.1.1")

# General Variables
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(TARGET_ARCH x64)
else()
  set(TARGET_ARCH x86)
endif()

#--------------Top Level Project Setup-------------

if(${PROJECT_IS_TOP_LEVEL})
    message(STATUS "NOTE: ${PROJECT_NAME} is being configured as a top-level project")

    # Install (override the CMake default, but not a user set value)
    if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
        set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/out/install"
               CACHE PATH "Project install path" FORCE
        )
    endif()

    # Clean install when clean target is ran
    set_directory_properties(PROPERTIES ADDITIONAL_CLEAN_FILES "${CMAKE_INSTALL_PREFIX}")
else()
    message(STATUS "NOTE: ${PROJECT_NAME} is being configured as a sub-project")

    # Keep install components out of 'all' target
    set(SUB_PROJ_EXCLUDE_FROM_ALL "EXCLUDE_FROM_ALL")
endif()

#--------------------Setup Paths-------------------

# Package
set(PACKAGE_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/out/dist")

#------------Set Global Build Rules----------------

# Versioning
include(OB/VerboseVersioning)
setup_verbose_versioning(PROJECT_VERSION_VERBOSE)


#================= Build =========================

# Import Qt
find_package(Qt6 REQUIRED COMPONENTS Core)

# Disable deprecated code
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)

# Fetch Qx (build and import from source)
include(OB/FetchQx)
fetch_qx("v0.4")

# Set main target name
# Name here needs to be as unique as possible for when this project is inlcuded
# in another via FetchContent or add_subdirectory (prevent target clashes)
set(MAIN_TARGET_NAME ${PROJECT_NAME_LC}_${PROJECT_NAME_LC})

# Process source files and create target
add_subdirectory(src)

#================= Install ==========================

set(TOP_LEVEL_INSTALL_COMPONENT ${PROJECT_NAME_LC})

# Install README and LICENSE
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
    COMPONENT ${TOP_LEVEL_INSTALL_COMPONENT}
    DESTINATION .
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
)

#====================== CPack ==============================

set(CPACK_PACKAGE_VENDOR "oblivioncth")
set(CPACK_PACKAGE_DIRECTORY "${PACKAGE_PREFIX}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION_VERBOSE}_${CMAKE_SYSTEM_NAME}_${TARGET_ARCH}")
set(CPACK_GENERATOR "ZIP")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
include(CPack)

