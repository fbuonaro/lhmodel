cmake_minimum_required( VERSION 3.10 )

project( liblhmodel 
         DESCRIPTION "lhmodel library"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhmodel/liblhmodel"
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
set( LH_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhmodel_${PROJECT_VERSION}" )
set( LH_INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhmodel_${PROJECT_VERSION}" )

# library name
set( LH_LIB_NAME lhmodel )

# source files
set( LH_LIB_SRC_FILES 
    "src/lhmodel.cxx" )
# library dependencies
set( LH_LIB_PUBLIC_LINKLIBS )
set( LH_LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LH_LIB_PUBLIC_INCLUDES 
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodel>        # build
     $<INSTALL_INTERFACE:${INSTALL_INCDIR}> )                                # export
set( LH_LIB_PRIVATE_INCLUDES 
     "${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodel_impl"
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

add_executable( testlhmodel
                "test/testlhmodel.cxx"
                "test/ExampleModelA.cxx"
                "test/ExampleModelB.cxx"
                "test/ExampleModelC.cxx"
                "test/ExampleModelD.cxx" )

target_link_libraries( testlhmodel
                       PRIVATE
                           "${GTEST_BOTH_LIBRARIES}"
                           lhmodel )

target_include_directories( testlhmodel
                            PRIVATE
                                "${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodel_test"
                                "${LH_LIB_PRIVATE_INCLUDES}"
                                "${CMAKE_CURRENT_BINARY_DIR}"
                                "${GTEST_INCLUDE_DIRS}" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodel )

##############################################################
# installation
##############################################################

# name of component to install
set(LH_COMPONENT_NAME liblhmodel )
# directories to install
set(LH_LIB_PUBLIC_INCLUDE_DIRS "include/lhmodel/lhmodel")

lh_add_install_library()

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${LH_INSTALL_INCDIR}")
set( PC_LIBDIR "${LH_INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS )
set( PC_PUBLIBS "-llhmodel" )
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
set( CPACK_RPM_LIBLHMODEL_PACKAGE_REQUIRES_POST "sed" PARENT_SCOPE) 
set( CPACK_RPM_LIBLHMODEL_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" PARENT_SCOPE)

set(LH_PACKAGE_DESC "liblhmodel libraries package")

lh_add_cpack_component()
