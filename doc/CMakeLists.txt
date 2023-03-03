# Setup documentation
include(OB/Documentation)
ob_standard_documentation(
    TARGET_NAME "${DOC_TARGET_NAME}"
    DOXY_VER 1.9.4
    PROJ_NAME "${PROJECT_NAME_FORMAL}"
    QT_PREFIX "${Qt6_PREFIX_PATH}"
    QT_MODULES
        qtcore
    INPUT_LIST
        "${LIB_PATH}/src"
        "${LIB_PATH}/include"
)
