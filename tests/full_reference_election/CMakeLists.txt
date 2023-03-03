#================= Common Build =========================

# Allow includes relative to source root
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Setup test target
set(test_name "tst_full_reference_election")
set(target_name "${TESTS_TARGET_PREFIX}_${test_name}")
add_executable(${target_name} ${test_name}.cpp)
starpp_add_test(NAME ${target_name} COMMAND ${target_name})
target_link_libraries(${target_name}
    PRIVATE
            Qt6::Test
            STAR::Base
            STAR::TestCommon
)

# Bundle test data
file(GLOB test_data
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "data/*.*"
)

qt_add_resources(${target_name} "${test_name}_data"
    PREFIX "/"
    FILES
        ${test_data}
)
