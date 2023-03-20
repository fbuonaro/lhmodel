cmake_minimum_required( VERSION 3.10 )

project( liblhmodelutil
         DESCRIPTION "lhmodelutil library"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhmodel/liblhmodelutil"
         LANGUAGES CXX
         VERSION 1.0.0 )

##############################################################
# general
##############################################################

include( GNUInstallDirs )

set( CMAKE_CXX_STANDARD 11 )

add_compile_options( -Wall -Wextra -pedantic -Werror )

##############################################################
# library
##############################################################

# install directories
set( LH_INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( LH_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhmodelutil_${PROJECT_VERSION}" )
set( LH_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhmodelutil_${PROJECT_VERSION}" )

# library name
set( LH_LIB_NAME lhmodelutil )

# pull in rapidjson
find_package( RapidJSON REQUIRED )
# pull in liblhmodel
# TODO still trying to get this to work, the files generated at install time cannot be found ..
# find_package( liblhmodel REQUIRED PATHS "${CMAKE_BINARY_DIR}")

# source files
set( LH_LIB_SRC_FILES 
     "src/lhmodel_rapidjson.cxx" )
# library dependencies
# TODO still trying to get this to work, not yet sure if this a compilation order issue 
# that makes find_package fail or something else, using the target normally like in any intraproject build fixes it
# set( LIB_PUBLIC_LINKLIBS liblhmodel::lhmodel)
set( LH_LIB_PUBLIC_LINKLIBS lhmodel)
set( LH_LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LH_LIB_PUBLIC_INCLUDES 
     "${RAPIDJSON_INCLUDE_DIRS}"
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodelutil>        # build
     $<INSTALL_INTERFACE:${LH_INSTALL_INCDIR}> )                                # export
set( LH_LIB_PRIVATE_INCLUDES 
     "include/lhmodelutil_impl"
     "${CMAKE_CURRENT_BINARY_DIR}" )

lh_add_library()

##############################################################
# unit tests
##############################################################

# pull in gtest
find_package( GTest REQUIRED )
if( NOT GTEST_FOUND )
    message( FATAL_ERROR "gtest could not be found" )
endif( NOT GTEST_FOUND )

add_executable( testlhmodelutil
                "test/testlhmodelutil.cxx"
                "test/ExampleModelA.cxx"
                "test/ExampleModelB.cxx"
                "test/ExampleModelC.cxx"
                "test/ExampleModelD.cxx" )

target_link_libraries( testlhmodelutil
                       PRIVATE
                           "${GTEST_BOTH_LIBRARIES}"
                           lhmodelutil )

target_include_directories( testlhmodelutil
                            PRIVATE
                                "${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodelutil_test"
                                "${LH_LIB_PRIVATE_INCLUDES}"
                                "${CMAKE_CURRENT_BINARY_DIR}"
                                "${GTEST_INCLUDE_DIRS}" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodelutil )

##############################################################
# installation
##############################################################

# name of component to install
set(LH_COMPONENT_NAME liblhmodelutil )
# directories to install
set(LH_LIB_PUBLIC_INCLUDE_DIRS "include/lhmodelutil/lhmodelutil")

lh_add_install_library()

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${LH_INSTALL_INCDIR}")
set( PC_LIBDIR "${LH_INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS )
set( PC_PUBLIBS "-llhmodelutil" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

lh_add_pkgconfig()

##############################################################
# cmake packaging
##############################################################

lh_add_install_cmake_config()

##############################################################
# rpm packaging
##############################################################
 
# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_LIBLHMODELUTIL_PACKAGE_REQUIRES_POST "sed" PARENT_SCOPE) 
set( CPACK_RPM_LIBLHMODELUTIL_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" PARENT_SCOPE)

set(LH_PACKAGE_DESC "liblhmodelutil libraries package")

lh_add_cpack_component()
