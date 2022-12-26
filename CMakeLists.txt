#================= Project Setup ==========================

# CMake
cmake_minimum_required(VERSION 3.24.0)

# Configuration options
option(STARCALC_TESTS "Build StarCalc tests" OFF)
option(STARCALC_DOCS "Build StarCalc documentation" OFF)

# Project
# NOTE: DON'T USE TRAILING ZEROS IN VERSIONS
project(StarCalc
    VERSION 0.1.0.1
    LANGUAGES CXX
    DESCRIPTION "C++ implementation of the STAR voting system"
)
set(FORMAL_NAME "STAR Calculator")
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
set(FILE_TEMPLATES_PATH "${PROJ_SCRIPTS_PATH}/file_templates")

#------------Set Global Build Rules----------------

# Versioning
include(OB/VerboseVersioning)
setup_verbose_versioning(PROJECT_VERSION_VERBOSE)

#================= Build =========================

# Import Qt
set(QT_COMPONENTS
    Core
)
if(STARCALC_TESTS)
    list(APPEND QT_COMPONENTS Test)
endif()

# Find Qt package
include(OB/BetterFindQt6)
find_qt6_package(REQUIRED COMPONENTS ${QT_COMPONENTS})

# Disable deprecated code
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060000)

# Fetch Qx (build and import from source)
include(OB/FetchQx)
fetch_qx("285b863ec9739d6eb7c803d9b6554d71afed75ee")

# Fetch Neargye's Magic Enum
include(OB/FetchMagicEnum)
fetch_magicenum("v0.8.0")

# Process Targets
set(LIB_TARGET_NAME ${PROJECT_NAME_LC}_base)
set(LIB_ALIAS_NAME ${PROJECT_NAME}::Base)
set(LIB_EXPORT_NAME Base)
set(LIB_INCLUDE_ROOT_NAME star)
set(LIB_TARGET_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(lib)

set(APP_TARGET_NAME ${PROJECT_NAME_LC}_frontend)
set(APP_ALIAS_NAME ${PROJECT_NAME}::Frontend)
add_subdirectory(app)

if(STARCALC_DOCS)
    set(DOC_TARGET_NAME ${PROJECT_NAME_LC}_docs)
    add_subdirectory(doc)
endif()

if(STARCALC_TESTS)
    enable_testing()
    set(TESTS_TARGET_PREFIX ${PROJECT_NAME_LC})
    set(TESTS_COMMON_ALIAS_NAME ${PROJECT_NAME}::TestCommon)
    add_subdirectory(tests)
endif()

#--------------------Package Config-----------------------

# Create config file
configure_file("${FILE_TEMPLATES_PATH}/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
    @ONLY
)

# Create version config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY ExactVersion
)

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

# Install Package Config
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake/${PROJECT_NAME}ConfigVersion.cmake"
    COMPONENT ${TOP_LEVEL_INSTALL_COMPONENT}
    DESTINATION cmake
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
)

#====================== CPack ==============================

set(CPACK_PACKAGE_VENDOR "oblivioncth")
set(CPACK_PACKAGE_DIRECTORY "${PACKAGE_PREFIX}")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION_VERBOSE}_(Qt${Qt6_VERSION}-${Qt6_LINKAGE})_${CMAKE_SYSTEM_NAME}_${TARGET_ARCH}")
set(CPACK_GENERATOR "ZIP")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
include(CPack)

