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
set( INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhmodelutil_${PROJECT_VERSION}" )
set( INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhmodelutil_${PROJECT_VERSION}" )

# pull in rapidjson
find_package( RapidJSON REQUIRED )
# pull in liblhmodel
# TODO still trying to get this to work, the files generated at install time cannot be found ..
# find_package( liblhmodel REQUIRED PATHS "${CMAKE_BINARY_DIR}")

# source files
set( LIB_SRC_FILES 
     "src/lhmodel_rapidjson.cxx" )
# library dependencies
# TODO still trying to get this to work, not yet sure if this a compilation order issue 
# that makes find_package fail or something else, using the target normally like in any intraproject build fixes it
# set( LIB_PUBLIC_LINKLIBS liblhmodel::lhmodel)
set( LIB_PUBLIC_LINKLIBS lhmodel)
set( LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LIB_PUBLIC_INCLUDES 
     "${RAPIDJSON_INCLUDE_DIRS}"
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodelutil>        # build
     $<INSTALL_INTERFACE:${INSTALL_INCDIR}> )                                # export
set( LIB_PRIVATE_INCLUDES 
     "include/lhmodelutil_impl"
     "${CMAKE_CURRENT_BINARY_DIR}" )

# boilerplate ..
# libs
add_library( lhmodelutil SHARED ${LIB_SRC_FILES} )
add_library( lhmodelutil_static STATIC ${LIB_SRC_FILES} )
# lib deps
target_link_libraries( lhmodelutil
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
target_link_libraries( lhmodelutil_static
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
# header deps
target_include_directories( lhmodelutil
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
target_include_directories( lhmodelutil_static
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
# properties
set_target_properties( lhmodelutil
                       PROPERTIES 
                           LIBRARY_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           SOVERSION ${PROJECT_VERSION}
                           VERSION ${PROJECT_VERSION}
                           CLEAN_DIRECT_OUTPUT 1)
set_target_properties( lhmodelutil_static 
                       PROPERTIES
                           ARCHIVE_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           OUTPUT_NAME lhmodelutil )
# .. boilerplate

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
                                "include/lhmodelutil_test"
                                "${LIB_PRIVATE_INCLUDES}"
                                "${CMAKE_CURRENT_BINARY_DIR}"
                                "${GTEST_INCLUDE_DIRS}" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodelutil )

##############################################################
# installation
##############################################################

# boilerplate ..
# install lib/bin
install( TARGETS lhmodelutil lhmodelutil_static
         EXPORT liblhmodelutil-targets 
         COMPONENT liblhmodelutil
         RUNTIME DESTINATION "${INSTALL_BINDIR}"
         COMPONENT liblhmodelutil
         LIBRARY DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhmodelutil
         ARCHIVE DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhmodelutil
         INCLUDES DESTINATION "${LIB_PUBLIC_INCLUDES}" )
# install header dir
install( DIRECTORY "include/lhmodelutil/lhmodelutil"
         DESTINATION "${INSTALL_INCDIR}"
         COMPONENT liblhmodelutil
         FILES_MATCHING PATTERN "*.h" )
# install export for things outside of build
install( EXPORT liblhmodelutil-targets 
         FILE liblhmodelutilTargets.cmake 
         NAMESPACE liblhmodelutil:: 
         DESTINATION "${INSTALL_LIBDIR}/cmake" 
         COMPONENT liblhmodelutil )
# export export for things in build
export( EXPORT liblhmodelutil-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/liblhmodelutilTargets.cmake 
    NAMESPACE liblhmodelutil:: )
ADD_CUSTOM_TARGET( install-lhmodelutil
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=liblhmodelutil"
                   -P "cmake_install.cmake" )
# .. boilerplate

##############################################################
# pkg-config
##############################################################
set( PC_INCDIR "${INSTALL_INCDIR}")
set( PC_LIBDIR "${INSTALL_LIBDIR}")
set( PC_PUBREQS )
set( PC_PRIVREQS )
set( PC_CFLAGS )
set( PC_PUBLIBS "-llhmodelutil" )
set( PC_PRIVLIBS )
set( PC_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig" )
set( PC_INSTALL_FILENAME "${PROJECT_NAME}-${CMAKE_PROJECT_VERSION_MAJOR}.pc" )
set( PC_INSTALL_FILE "${PC_INSTALL_DIR}/${PC_INSTALL_FILENAME}" )
set( PC_RELOCATE_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh" )

# boilerplate ..
# configure pc file
configure_file( "${CMAKE_SOURCE_DIR}/modules/lhscriptutil/cmake/pkgconfig/component.pc.in"
                "${CMAKE_CURRENT_BINARY_DIR}/${PC_INSTALL_FILENAME}"
                @ONLY )
# install pc file
install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PC_INSTALL_FILENAME}"
         DESTINATION "${PC_INSTALL_DIR}"
         COMPONENT liblhmodelutil )
# configure pc relocation script
configure_file( "${CMAKE_SOURCE_DIR}/modules/lhscriptutil/cmake/pkgconfig/rpm_relocate_pc.sh.in"
                "${CMAKE_CURRENT_BINARY_DIR}/rpm_relocate_pc.sh"
                @ONLY )
# .. boilerplate

##############################################################
# cmake packaging
##############################################################

# boilerplate ..
include(CMakePackageConfigHelpers)

configure_package_config_file( "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                               "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                               INSTALL_DESTINATION "${INSTALL_LIBDIR}/cmake" )

write_basic_package_version_file( "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                                  VERSION "${PROJECT_VERSION}"
                                  COMPATIBILITY SameMajorVersion )

install( FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
         DESTINATION "${INSTALL_LIBDIR}/cmake"
         COMPONENT liblhmodelutil )
# .. boilerplate

##############################################################
# rpm packaging
##############################################################
 
# boilerplate ..
set( CPACK_PROJECT_NAME ${PROJECT_NAME} )
set( CPACK_PROJECT_VERSION ${PROJECT_VERSION} )
set( CPACK_COMPONENTS_GROUPING ONE_PER_GROUP )
set( CPACK_GENERATOR "RPM" )

# set( CPACK_GENERATOR "RPM" "TGZ" )
# CPACK_RPM_BUILD_REQUIRES
set( CPACK_RPM_PACKAGE_REQUIRES "rapidjson-devel >= 1.1.0" )

set( CPACK_RPM_PACKAGE_RELOCATABLE ON)
set( CPACK_RPM_COMPONENT_INSTALL ON )

# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_PACKAGE_REQUIRES_POST "sed" ) 
set( CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" )

include( CPack )

# component, group can be different; group will appended to package
cpack_add_component(liblhmodelutil
    DISPLAY_NAME  "lhmodelutil library"
    DESCRIPTION   "lhmodelutil library"
    GROUP liblhmodelutil)
cpack_add_component_group(liblhmodelutil)
# .. boilerplate