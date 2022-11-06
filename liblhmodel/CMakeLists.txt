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
set( INSTALL_BINDIR "${CMAKE_INSTALL_BINDIR}" )
set( INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}/liblhmodel_${PROJECT_VERSION}" )
set( INSTALL_INCDIR "${CMAKE_INSTALL_INCLUDEDIR}/lhmodel_${PROJECT_VERSION}" )

# source files
set( LIB_SRC_FILES 
    "src/lhmodel.cxx" )
# library dependencies
set( LIB_PUBLIC_LINKLIBS )
set( LIB_PRIVATE_LINKLIBS )
# header dependencies
set( LIB_PUBLIC_INCLUDES 
     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/lhmodel>        # build
     $<INSTALL_INTERFACE:${INSTALL_INCDIR}> )                                # export
set( LIB_PRIVATE_INCLUDES 
     "include/lhmodel_impl"
     "${CMAKE_CURRENT_BINARY_DIR}" )

# boilerplate ..
# libs
add_library( lhmodel SHARED ${LIB_SRC_FILES} )
add_library( lhmodel_static STATIC ${LIB_SRC_FILES} )
# lib deps
target_link_libraries( lhmodel
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
target_link_libraries( lhmodel_static
                       PUBLIC "${LIB_PUBLIC_LINKLIBS}"
                       PRIVATE "${LIB_PRIVATE_LINKLIBS}" )
# header deps
target_include_directories( lhmodel
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
target_include_directories( lhmodel_static
                            PUBLIC "${LIB_PUBLIC_INCLUDES}"
                            PRIVATE "${LIB_PRIVATE_INCLUDES}" )
# properties
set_target_properties( lhmodel
                       PROPERTIES 
                           LIBRARY_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           SOVERSION ${PROJECT_VERSION}
                           VERSION ${PROJECT_VERSION}
                           CLEAN_DIRECT_OUTPUT 1)
set_target_properties( lhmodel_static 
                       PROPERTIES
                           ARCHIVE_OUTPUT_DIRECTORY "${INSTALL_LIBDIR}"
                           OUTPUT_NAME lhmodel )
# .. boilerplate

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
                                "include/lhmodel_test"
                                "${LIB_PRIVATE_INCLUDES}"
                                "${CMAKE_CURRENT_BINARY_DIR}"
                                "${GTEST_INCLUDE_DIRS}" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodel )

##############################################################
# installation
##############################################################

# boilerplate ..
# install lib/bin
install( TARGETS lhmodel lhmodel_static
         EXPORT liblhmodel-targets 
         COMPONENT liblhmodel
         RUNTIME DESTINATION "${INSTALL_BINDIR}"
         COMPONENT liblhmodel
         LIBRARY DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhmodel
         ARCHIVE DESTINATION "${INSTALL_LIBDIR}"
         COMPONENT liblhmodel
         INCLUDES DESTINATION "${LIB_PUBLIC_INCLUDES}" )
# install header dir
install( DIRECTORY "include/lhmodel/lhmodel"
         DESTINATION "${INSTALL_INCDIR}"
         COMPONENT liblhmodel
         FILES_MATCHING PATTERN "*.h" )
# install export for things outside of build
install( EXPORT liblhmodel-targets 
         FILE liblhmodelTargets.cmake 
         NAMESPACE liblhmodel:: 
         DESTINATION "${INSTALL_LIBDIR}/cmake" 
         COMPONENT liblhmodel )
# export export for things in build
export( EXPORT liblhmodel-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/liblhmodelTargets.cmake 
    NAMESPACE liblhmodel:: )
ADD_CUSTOM_TARGET( install-lhmodel
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=liblhmodel"
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
set( PC_PUBLIBS "-llhmodel" )
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
         COMPONENT liblhmodel )
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
         COMPONENT liblhmodel )
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
# CPACK_RPM_liblhmodel_REQUIRES # component requires

set( CPACK_RPM_PACKAGE_RELOCATABLE ON)
set( CPACK_RPM_COMPONENT_INSTALL ON )

# to relocate the pc file prefix to /somewhere if 'rpm --install --prefix /somewhere'
set( CPACK_RPM_PACKAGE_REQUIRES_POST "sed" ) 
set( CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${PC_RELOCATE_SCRIPT}" )

include( CPack )

# component, group can be different; group will appended to package
cpack_add_component(liblhmodel
    DISPLAY_NAME  "lhmodel library"
    DESCRIPTION   "lhmodel library"
    GROUP liblhmodel)
cpack_add_component_group(liblhmodel)
# .. boilerplate