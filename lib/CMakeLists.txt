#================= Common Build =========================

# Add executable target
qt_add_library(${LIB_TARGET_NAME})

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
    dummy_p.h
    reference/ballotbox_p.h
    reference/categoryconfig_p.h
    reference/resultset_p.h
)

set(CXX_IMPLEMENTATION
    calculator.cpp
    election.cpp
    electionresult.cpp
    expectedelectionresult.cpp
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
    $<INSTALL_INTERFACE:NOT_SETUP>
)

# Link target to libraries
target_link_libraries(${LIB_TARGET_NAME}
    PRIVATE
        Qx::Io
    PUBLIC
        Qt6::Core
        Qx::Core
)

################# Windows Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Windows)

endif()


################# Linux Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Linux)

endif()

#================= Install ==========================

# NOT SETUP
