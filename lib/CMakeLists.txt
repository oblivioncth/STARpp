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

# Add via ob standard library
include(OB/Library)
ob_add_standard_library(${LIB_TARGET_NAME}
    NAMESPACE "${PROJECT_NAMESPACE}"
    ALIAS "${LIB_ALIAS_NAME}"
    EXPORT_HEADER
        PATH "${PROJECT_NAMESPACE_LC}/${PROJECT_NAMESPACE_LC}_${LIB_ALIAS_NAME_LC}_export.h"  
    HEADERS_API
        COMMON "${PROJECT_NAMESPACE_LC}"
        FILES
            calculator.h
            election.h
            electionresult.h
            expectedelectionresult.h
            qualifierresult.h
            rank.h
            reference.h
            seat.h
    IMPLEMENTATION
        calculator.cpp
        election.cpp
        electionresult.cpp
        expectedelectionresult.cpp
        headtoheadresults.h
        headtoheadresults.cpp
        qualifierresult.cpp
        rank.cpp
        reference.cpp
        reference/ballotbox_p.h
        reference/ballotbox_p.cpp
        reference/calculatoroptions_p.h
        reference/calculatoroptions_p.cpp
        reference/categoryconfig_p.h
        reference/categoryconfig_p.cpp
        reference/resultset_p.h
        reference/resultset_p.cpp
        seat.cpp
    LINKS
        PRIVATE
            Qx::Core
            Qx::Io
            $<BUILD_INTERFACE:magic_enum::magic_enum>
        PUBLIC
            Qt6::Core
    CONFIG STANDARD
 )
