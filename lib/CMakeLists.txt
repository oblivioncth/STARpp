#================= Common Build =========================

# NOTE: This file involves complications with cmake library linking that are explained here.
#
# Because static libraries don't embed the symbols for their dependencies, they need access to
# them (whether a static .lib/.a or dynamic .dll/.so) when linking actually occures (i.e. when
# making an executable or different dynamic lib). Because of this, when using target_link_libraries()
# on a static library, the listed dependencies will all be expected to have an export/install(EXPORT...)
# setup for them if the target library also has one setup, even if those dependencies are under the
# PRIVATE section (this is the major sticking point, as normally you'd think that the PRIVATE section
# would limit all inclusion of the dependecy to the build only). This is so that when the target library is
# exported/installed, the object files of its dependencies are also exported/installed since they will
# be needed when the consumer of the target goes to build their project as described above.
#
# However, there is one exception to this. If the dependency is an INTERFACE target that itself has no
# non-interface dependencies (and therefore brings in no symbols whatsoever), then it should not need
# to be exported/install at all since there are no symbols from it that consumers further up the chain
# will need. Unfortunately, CMake does not currently detect this situation, and errors out anyway.
#
# A somewhat hacky workaround is to use $<BUILD_INTERFACE:dependency_name> within target_include_libraries()
# to force scope the inclusion to the build portion only. The caveat though is that this only prevents an
# error with install(EXPORT...) and not export(), though the latter is mainly useful for things like
# cross-compiling, and therefore is not of concern here.
#
# This workaround is used with the dependency "magic_enum" below.
#
# See the following for more info:
#
# https://gitlab.kitware.com/cmake/cmake/-/issues/15415
# https://stackoverflow.com/questions/45777734/install-export-requires-target-from-subproject

# Add executable target
qt_add_library(${LIB_TARGET_NAME} STATIC)

# Make alias target so target can be referred to with its friendly
# export name both internally and when part of another build tree
add_library(${LIB_ALIAS_NAME} ALIAS ${LIB_TARGET_NAME})

# Set app sources
set(CXX_PUBLIC_HEADERS
    calculator.h
    election.h
    electionresult.h
    expectedelectionresult.h
    rank.h
    reference.h
)

set(CXX_PRIVATE_HEADERS
    headtoheadresults.h
    reference/ballotbox_p.h
    reference/categoryconfig_p.h
    reference/resultset_p.h
)

set(CXX_IMPLEMENTATION
    calculator.cpp
    election.cpp
    electionresult.cpp
    expectedelectionresult.cpp
    headtoheadresults.cpp
    rank.cpp
    reference.cpp
    reference/ballotbox_p.cpp
    reference/categoryconfig_p.cpp
    reference/resultset_p.cpp
)

# Build pathed implementation file list
foreach(impl_file ${CXX_IMPLEMENTATION})
    list(APPEND pathed_impl_files "src/${impl_file}")
endforeach()

# Build pathed private headers file list
foreach(private_header ${CXX_PRIVATE_HEADERS})
    list(APPEND pathed_private_headers "src/${private_header}")
endforeach()

# Build pathed include file list
foreach(api_header ${CXX_PUBLIC_HEADERS})
    list(APPEND pathed_api_headers "include/${LIB_INCLUDE_ROOT_NAME}/${api_header}")
endforeach()

# Group include files with their parent directories stripped
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/${LIB_INCLUDE_ROOT_NAME}"
    PREFIX "Include Files"
    FILES ${pathed_api_headers}
)

# Add sources to target. Add include files as private target source so that they aren't
# built nor marked as a dependency, but are shown with the target in the IDE
target_sources(${LIB_TARGET_NAME}
    PRIVATE
        ${pathed_impl_files}
        ${pathed_private_headers}
        ${pathed_api_headers}
)

# Set target include directories so that public headers are propegated to consumers
target_include_directories(${LIB_TARGET_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/${LIB_INCLUDE_ROOT_NAME}>
)

# Link target to libraries
target_link_libraries(${LIB_TARGET_NAME}
    PRIVATE
        Qx::Core
        Qx::Io
        $<BUILD_INTERFACE:magic_enum::magic_enum>
    PUBLIC
        Qt6::Core
)

#---------- Configure Target Properties------------------
set_target_properties(${LIB_TARGET_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    DEBUG_POSTFIX "d"
    EXPORT_NAME "${LIB_EXPORT_NAME}"
)

################# Windows Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set_target_properties(${LIB_TARGET_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME}"
    )
endif()


################# Linux Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set_target_properties(${LIB_TARGET_NAME} PROPERTIES
        OUTPUT_NAME "${PROJECT_NAME_LC}"
    )
endif()

#================= Install ==========================

# Install lib
install(TARGETS ${LIB_TARGET_NAME}
    COMPONENT ${LIB_TARGET_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin # For potential future shared version
)

# Install public headers
install(DIRECTORY include/${LIB_INCLUDE_ROOT_NAME}
    COMPONENT ${LIB_TARGET_NAME}
    DESTINATION "include"
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
)

# Install package target export
install(EXPORT ${PROJECT_NAME}Targets
    COMPONENT ${LIB_TARGET_NAME}
    FILE "${PROJECT_NAME}Targets.cmake"
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION cmake
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
)
