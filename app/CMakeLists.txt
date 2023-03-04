# Creates an executable target in the "OB Standard Fashion"
# via the provided arguments. Also produces an install
# component that matches the target name.
#
# This function will handle setup for installation of the component's
# dependencies, including if it uses shared Qt libraries.
#
# Adds CMAKE_CURRENT_SOURCE_DIR as an include directory to the target.
#
# Argument Notes:
# ---------------
# NAMESPACE:
#   Namespace to use for file generation, export configuration, and installation pathing.
# ALIAS:
#   Do not use "::" as part of the libraries alias, they will be
#   added automatically after first prepending the provided namespace.
#
#   Used for file generation, export configuration, and installation pathing.
# OUTPUT_NAME:
#   Name to use for the built executable. Will always be lowercase on Linux platforms.
#   Set to ALIAS if not defined.
# SOURCE:
#   Files are assumed to be under "${CMAKE_CURRENT_SOURCE_DIR}/src"
# SOURCE_GEN:
#   Files are assumed to be under "${CMAKE_CURRENT_BINARY_DIR}/src"
# LINKS:
#   Same contents/arguments as with target_link_libraries().
# CONFIG:
#   This optional argument can take two forms.
#
#   First:
#       CONFIG
#           STANDARD
#           DEPENDS
#
#   This form configures and installs a package configuration file set to include
#   the library target and find the optional dependencies if provided. The dependencies
#   are passed via the same form as in the CONFIG argument from the
#   ob_standard_project_package_config() command.
#
#   Second:
#       CONFIG
#           CUSTOM "path"
#
#   This form uses configure_file() to configure the custom configuration file input
#   template (presumably associated with the target) from the provided path. It also
#   handles its installation.
#
#   The config file is installed as:
#   ${CMAKE_INSTALL_PREFIX)/cmake/${NAMESPACE}/${NAMESPACE}${ALIAS}Config.cmake
function(ob_add_standard_executable target)

    #------------ Argument Handling ---------------

    # Function inputs
    set(oneValueArgs
        NAMESPACE
        ALIAS
        OUTPUT_NAME
    )

    set(multiValueArgs
        SOURCE
        SOOURCE_GEN
        LINKS
        CONFIG
    )

    # Required Arguments (All Types)
    set(requiredArgs
        NAMESPACE
        ALIAS
        SOURCE
    )

    # Parse arguments
    include(OB/Utility)
    ob_parse_arguments(STD_EXECUTABLE "" "${oneValueArgs}" "${multiValueArgs}" "${requiredArgs}" ${ARGN})

    # Standardized set and defaulted values
    set(_TARGET_NAME "${target}")
    set(_NAMESPACE "${STD_EXECUTABLE_NAMESPACE}")
    set(_ALIAS "${STD_EXECUTABLE_ALIAS}")

    if(STD_EXECUTABLE_OUTPUT_NAME)
        set(_OUTPUT_NAME "${OUTPUT_NAME}")
    else()
        set(_OUTPUT_NAME "${ALIAS}")
    endif()

    set(_SOURCE "${STD_EXECUTABLE_SOURCE}")
    set(_SOURCE_GEN "${STD_EXECUTABLE_SOURCE_GEN}")
    set(_LINKS "${STD_EXECUTABLE_LINKS}")
    set(_CONFIG "${STD_EXECUTABLE_CONFIG}")

    # Compute Intermediate Values
    if(_LINKS MATCHES "Qt[0-9]*::")
        set(_USE_QT TRUE)
    else()
        set(_USE_QT FALSE)
    endif()

    string(TOLOWER "${_NAMESPACE}" _NAMESPACE_LC)
    string(TOLOWER "${_ALIAS}" _ALIAS_LC)
    string(TOLOWER "${_OUTPUT_NAME}" _OUTPUT_NAME_LC)

    #---------------- Executable Setup -------------------

    # Create lib
    if(_USE_QT)
        qt_add_executable(${_TARGET_NAME})
    else()
        add_executable(${_TARGET_NAME})
    endif()

    add_executable("${_NAMESPACE}::${_ALIAS}" ALIAS ${_TARGET_NAME})

    # Add implementation
    foreach(impl ${_SOURCE})
        # Ignore non-relevant system specific implementation
        string(REGEX MATCH [[_win\.cpp$]] IS_WIN_IMPL "${impl}")
        string(REGEX MATCH [[_linux\.cpp$]] IS_LINUX_IMPL "${impl}")
        if((IS_WIN_IMPL AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows") OR
           (IS_LINUX_IMPL AND NOT CMAKE_SYSTEM_NAME STREQUAL "Linux"))
            continue()
        endif()

        list(APPEND full_impl_paths "${CMAKE_CURRENT_SOURCE_DIR}/src/${impl}")
    endforeach()

    target_sources(${_TARGET_NAME} PRIVATE ${full_impl_paths})

    # Add generated implementation
    if(_SOURCE_GEN)
        foreach(impl_gen ${_SOURCE})
            # Ignore non-relevant system specific implementation
            string(REGEX MATCH [[_win\.cpp$]] IS_WIN_IMPL "${impl_gen}")
            string(REGEX MATCH [[_linux\.cpp$]] IS_LINUX_IMPL "${impl_gen}")
            if((IS_WIN_IMPL AND NOT CMAKE_SYSTEM_NAME STREQUAL "Windows") OR
               (IS_LINUX_IMPL AND NOT CMAKE_SYSTEM_NAME STREQUAL "Linux"))
                continue()
            endif()

            list(APPEND full_impl_gen_paths "${CMAKE_CURRENT_SOURCE_DIR}/src/${impl_gen}")
        endforeach()

        target_sources(${_TARGET_NAME} PRIVATE ${full_impl_gen_paths})
    endif()

    # Include current soure and generated source directories for easy includes from the top
    # level of the target hiearchy
    target_include_directories(${_TARGET_NAME}
        PRIVATE
            "${CMAKE_CURRENT_SOURCE_DIR}/src"
            "${CMAKE_CURRENT_BINARY_DIR}/src"
    )

    # Link to libraries
    if(_LINKS)
        target_link_libraries(${_TARGET_NAME} ${_LINKS})
    endif()

    # Configure target properties
    set_target_properties(${_TARGET_NAME} PROPERTIES
        EXPORT_NAME "${_ALIAS}"
    )

    if(CMAKE_SYSTEM_NAME STREQUAL Windows)
        set_target_properties(${_TARGET_NAME} PROPERTIES
            OUTPUT_NAME "${_OUTPUT_NAME}"
        )
    endif()

    if(CMAKE_SYSTEM_NAME STREQUAL Linux)
        set_target_properties(${_TARGET_NAME} PROPERTIES
            OUTPUT_NAME "${_OUTPUT_NAME_LC}"
        )
    endif()

    # Install target and export
    install(TARGETS ${_TARGET_NAME}
        COMPONENT ${_TARGET_NAME}
        EXPORT ${_NAMESPACE}${_ALIAS}Targets
        ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
        RUNTIME DESTINATION bin
    )

    install(EXPORT ${_NAMESPACE}${_ALIAS}Targets
        COMPONENT ${_TARGET_NAME}
        FILE "${_NAMESPACE}${_ALIAS}Targets.cmake"
        NAMESPACE ${_NAMESPACE}::
        DESTINATION "cmake/${_ALIAS}"
        ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
    )

    # Install Dependencies
    if(_LINKS)

    endif()

    # Package Config
    if(_CONFIG)
        # Parse config parameters directly in this function to avoid awkward variable passing
        set(cfg_gen_include "${_NAMESPACE}${_ALIAS}Targets.cmake")
        set(cfg_gen_name "${_NAMESPACE}${_ALIAS}Config.cmake")
        set(cfg_gen_path "${CMAKE_CURRENT_BINARY_DIR}/cmake/${cfg_gen_name}")

        set(op
            STANDARD
        )

        set(ova
            CUSTOM
        )

        set(mva
            DEPENDS
        )

        # Parse arguments
        ob_parse_arguments(CONFIG "${op}" "${ova}" "${mva}" "" ${_CONFIG})

        # Must have one, and only one form
        if(DEFINED CONFIG_CUSTOM AND (CONFIG_STANDARD OR DEFINED CONFIG_DEPENDS))
            message(FATAL_ERROR "CUSTOM and STANDARD mode are mutually exclusive!")
        elseif(NOT DEFINED CONFIG_CUSTOM AND NOT CONFIG_STANDARD)
            message(FATAL_ERROR "Either CUSTOM or STANDARD must be used!")
        endif()

        # Standard Form
        if(CONFIG_STANDARD)
            # Generate config
            include("${__OB_CMAKE_PRIVATE}/common.cmake")
            __ob_generate_std_target_package_config_file(
                OUTPUT "${cfg_gen_path}"
                INCLUDES "${cfg_gen_include}"
                DEPENDS ${CONFIG_DEPENDS}
            )
        else() # Custom Form
            configure_file(
                "${CONFIG_CUSTOM}"
                "${cfg_gen_path}"
                @ONLY
            )
        endif()

        # Install config
        install(FILES
            "${cfg_gen_path}"
            COMPONENT ${_TARGET_NAME}
            DESTINATION "cmake/${_ALIAS}"
            ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
        )
    endif()
endfunction()

#================= Common Build =========================

# Add via ob standard executable
ob_add_standard_executable(${APP_TARGET_NAME}
    NAMESPACE "${PROJECT_NAMESPACE}"
    ALIAS "${APP_ALIAS_NAME}"
    OUTPUT_NAME "${PROJECT_NAMESPACE}"
    SOURCE
        core.h
        core.cpp
        errorcode.h
        referenceelectionconfig.h
        resultspresenter.h
        resultspresenter.cpp
        main.cpp
    LINKS
        PRIVATE
            STAR::Base
            Qx::Core
            Qx::Io
            magic_enum::magic_enum
    CONFIG STANDARD
)

## Forward select project variables to C++ code
include(OB/CppVars)
ob_add_cpp_vars(${APP_TARGET_NAME}
    NAME "project_vars"
    PREFIX "PROJECT_"
    VARS
        VERSION_STR "\"${PROJECT_VERSION}\""
        SHORT_NAME "\"${PROJECT_NAME}\""
        APP_NAME "\"${APP_NAME}\""
)
