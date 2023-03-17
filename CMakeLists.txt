#================= Project Setup ==========================

# CMake
cmake_minimum_required(VERSION 3.23.0...3.25.0)

# Project
# NOTE: DON'T USE TRAILING ZEROS IN VERSIONS
project(STARpp
    VERSION 0.2.0.5
    LANGUAGES CXX
    DESCRIPTION "C++ implementation of STAR voting"
)

# Get helper scripts
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/FetchOBCMake.cmake)
fetch_ob_cmake("fa4641c34c6f7f11111d5ae73a3b41c59e095caa")

# Initialize project according to standard rules
include(OB/Project)
ob_standard_project_setup()

# Standard setup overrides
set(PROJECT_NAMESPACE "STAR")
string(TOLOWER "${PROJECT_NAMESPACE}" PROJECT_NAMESPACE_LC)
string(TOUPPER "${PROJECT_NAMESPACE}" PROJECT_NAMESPACE_UC)

# Additional Project Variables
set(PROJECT_NAME_FORMAL "STAR++")

# Configuration options
option(STARPP_TESTS "Build STAR++ tests" OFF)
option(STARPP_DOCS "Build STAR++ documentation" OFF)

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

#================= Build =========================

# Import Qt
set(STARPP_QT_COMPONENTS
    Core
)

if(STARPP_TESTS)
    list(APPEND STARPP_QT_COMPONENTS Test)
endif()

# Find Qt package
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)
include(OB/BetterFindQt6)
ob_find_qt6_package(REQUIRED COMPONENTS ${STARPP_QT_COMPONENTS})

# Fetch Qx (build and import from source)
set(STARPP_QX_COMPONENTS
    Core
    Io
)

include(OB/FetchQx)
ob_fetch_qx(
    REF "72e93eb5986b62ebd4ac26971572b4860724b5d2"
    COMPONENTS
        ${STARPP_QX_COMPONENTS}
)

# Fetch Neargye's Magic Enum
include(OB/FetchMagicEnum)
ob_fetch_magicenum("v0.8.0")

# Process Targets
set(LIB_TARGET_NAME ${PROJECT_NAMESPACE_LC}_base)
set(LIB_ALIAS_NAME Base)
string(TOLOWER "${LIB_ALIAS_NAME}" LIB_ALIAS_NAME_LC)
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib")
add_subdirectory("${LIB_PATH}")

set(APP_TARGET_NAME ${PROJECT_NAMESPACE_LC}_frontend)
set(APP_ALIAS_NAME Frontend)
add_subdirectory(app)

if(STARPP_DOCS)
    set(DOC_TARGET_NAME ${PROJECT_NAMESPACE_LC}_docs)
    add_subdirectory(doc)
endif()

if(STARPP_TESTS)
    enable_testing()
    set(TESTS_TARGET_PREFIX ${PROJECT_NAMESPACE_LC})
    set(TESTS_COMMON_ALIAS_NAME ${PROJECT_NAMESPACE}::TestCommon)
    add_subdirectory(tests)
endif()

#--------------------Package Config-----------------------

ob_standard_project_package_config(
    COMPATIBILITY "SameMinorVersion"
    CONFIG STANDARD
        TARGET_CONFIGS
            TARGET "${PROJECT_NAMESPACE}::${LIB_ALIAS_NAME}" COMPONENT "${LIB_ALIAS_NAME}" DEFAULT
            TARGET "${PROJECT_NAMESPACE}::${APP_ALIAS_NAME}" COMPONENT "${APP_ALIAS_NAME}"
        DEPENDS
            PACKAGE "Qt6" COMPONENTS ${STARPP_QT_COMPONENTS}
            PACKAGE "Qx" VERSION ${Qx_VERSION} COMPONENTS ${STARPP_QX_COMPONENTS}
)

#================= Install ==========================

ob_standard_project_misc_install()

#====================== CPack ==============================

include(OB/Packaging)
ob_standard_project_package(VENDOR "oblivioncth")

