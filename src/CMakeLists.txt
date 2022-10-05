#================= Common Build =========================

# Allow includes relative to source root
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Get path to resources
set(RESOURCE_PATH "${PROJECT_SOURCE_DIR}/res")

# Add executable target
qt_add_executable(${MAIN_TARGET_NAME})

# Make alias target so target can be referred to with its friendly
# export name both internally and when part of another build tree
add_executable(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${MAIN_TARGET_NAME})

# Set app sources
set(CXX_SOURCES
    main.cpp
)

# Add sources to target
target_sources(${MAIN_TARGET_NAME}
    PRIVATE
        ${CXX_SOURCES}
)

# Link target to libraries
target_link_libraries(${MAIN_TARGET_NAME}
    PRIVATE
        Qt6::Core
        Qx::Core
)

# Configure target properties
set_target_properties(${MAIN_TARGET_NAME} PROPERTIES
    EXPORT_NAME "${PROJECT_NAME}"
)

################# Windows Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Windows)

endif()


################# Linux Build #################
if(CMAKE_SYSTEM_NAME STREQUAL Linux)

endif()

#================= Install ==========================

# Install executable
install(TARGETS ${MAIN_TARGET_NAME}
    CONFIGURATIONS Release
    COMPONENT ${MAIN_TARGET_NAME}
    ${SUB_PROJ_EXCLUDE_FROM_ALL} # "EXCLUDE_FROM_ALL" if project is not top-level
    RUNTIME DESTINATION bin
)
