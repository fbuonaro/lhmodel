cmake_minimum_required( VERSION 3.10 )

project( liblhmodel )

# general flags for building successfully
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

# for debugging
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -Wall -Wextra -W" )
set( LLHC_CXX_FLAGS_DBG "${LLHC_CXX_FLAGS_DBG} -DDEBUG" )
set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${LLHC_CXX_FLAGS_DBG}" )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${LLHC_CXX_FLAGS_DBG}" )

##############################################################
# library
##############################################################

set( LIB_SRC_DIR "src" )
set( LIB_INC_DIR "include" )

# pull in gtest
find_package( GTest REQUIRED )
if( NOT GTEST_FOUND )
    message( FATAL_ERROR "gtest could not be found" )
endif( NOT GTEST_FOUND )

add_library( lhmodel SHARED
    "${LIB_SRC_DIR}/lhmodel.cxx" )

# target_link_libraries( lhmodel
#     PUBLIC
#     PRIVATE
#     )

target_include_directories( lhmodel
    PUBLIC
        "${LIB_INC_DIR}/lhmodel"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}" )

##############################################################
# unit tests
##############################################################

set( TST_SRC_DIR "./test" )

enable_testing()

add_executable( testlhmodel
    "${TST_SRC_DIR}/testlhmodel.cxx"
    "${TST_SRC_DIR}/ExampleModelA.cxx"
    "${TST_SRC_DIR}/ExampleModelB.cxx"
    "${TST_SRC_DIR}/ExampleModelC.cxx"
    "${TST_SRC_DIR}/ExampleModelD.cxx" )

target_link_libraries( testlhmodel
    PRIVATE
        "${GTEST_BOTH_LIBRARIES}"
        lhmodel )

target_include_directories( testlhmodel
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${GTEST_INCLUDE_DIRS}"
        "${LIB_INC_DIR}/lhmodel"
        "${LIB_INC_DIR}/lhmodel_test" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodel )

##############################################################
# installation
##############################################################

# library
install( TARGETS lhmodel
         LIBRARY
            DESTINATION lib
            COMPONENT lhmodel )

# include directory
install( DIRECTORY "${LIB_INC_DIR}/lhmodel/lhmodel"
         DESTINATION include
         COMPONENT lhmodel
         FILES_MATCHING PATTERN "*.h" )

ADD_CUSTOM_TARGET( install-lhmodel
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=lhmodel"
                   -P "cmake_install.cmake" )
