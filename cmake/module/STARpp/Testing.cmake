# Adds a test and automatically adds the supplied Qt bin dir to its env when run
macro(starpp_add_test)

    # Parse arguments (just NAME for now)
    cmake_parse_arguments(__CUS_ADD_TEST "" "NAME" "" ${ARGN})

    # Validate input
    if(__CUS_ADD_TEST_KEYWORDS_MISSING_VALUES)
        foreach(missing_val ${__CUS_ADD_TEST_KEYWORDS_MISSING_VALUES})
            message(WARNING "A value for '${missing_val}' must be provided")
        endforeach()
        message(FATAL_ERROR "Not all required values were present!")
    endif()

    add_test(NAME ${__CUS_ADD_TEST_NAME} ${__CUS_ADD_TEST_UNPARSED_ARGUMENTS})
    set_property(TEST ${__CUS_ADD_TEST_NAME} PROPERTY ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${Qt6_PREFIX_PATH}/bin")
endmacro()