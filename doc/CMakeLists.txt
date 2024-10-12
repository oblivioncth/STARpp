# Include standard documentation target creator
include(OB/Documentation)

# See https://github.com/doxygen/doxygen/issues/9892
list(APPEND DOXYGEN_PREDEFINED
    STAR_BASE_EXPORT=
)

ob_standard_documentation(${DOC_TARGET_NAME}
    DOXY_VER 1.9.4
    PROJ_NAME "${PROJECT_NAME_FORMAL}"
    QT_MODULES
        qtcore
    INPUT_LIST
        "${LIB_PATH}/src"
        "${LIB_PATH}/include"
    THEME_COLORS
        PRIMARY "#28B3CC"
        NIGHT_PRIMARY "#2BAAC6"
)
