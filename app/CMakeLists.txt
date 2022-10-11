#================= Common Build =========================

# Allow includes relative to source root
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Get path to resources
set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/res")

# Add executable target
qt_add_executable(${APP_TARGET_NAME})

# Make alias target so target can be referred to with its friendly
# export name both internally and when part of another build tree
add_executable(${APP_ALIAS_NAME} ALIAS ${APP_TARGET_NAME})

# Set app sources
set(CXX_SOURCES
    src/core.h
    src/core.cpp
    src/errorcode.h
    src/referenceelectionconfig.h
    src/resultspresenter.h
    src/resultspresenter.cpp
    src/main.cpp
)

# Add sources to target
target_sources(${APP_TARGET_NAME}
    PRIVATE
        ${CXX_SOURCES}
)

# Link target to libraries
target_link_libraries(${APP_TARGET_NAME}
    PRIVATE
        StarCalc::Base
        Qx::Core
        Qx::Io
)

## Forward select project variables to C++ code
include(OB/ProjectVars)
set_cxx_project_vars(${APP_TARGET_NAME}
    VERSION_STR "\"${PROJECT_VERSION}\""
    SHORT_NAME "\"${PROJECT_NAME}\""
    APP_NAME "\"${FORMAL_NAME}\""
)

################# Windows Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Windows)

endif()


################# Linux Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Linux)

endif()

#================= Install ==========================

# Install executable
install(TARGETS ${APP_TARGET_NAME}
    CONFIGURATIONS Release
    COMPONENT ${APP_TARGET_NAME}
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
    RUNTIME DESTINATION bin
)
