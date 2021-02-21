cmake_minimum_required( VERSION 3.10 )

project( liblhmodelutil )

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

find_package( RapidJSON REQUIRED )
if( NOT RapidJSON_FOUND )
    message( FATAL_ERROR "rapidjson could not be found" )
endif( NOT RapidJSON_FOUND )

add_library( lhmodelutil SHARED
    "${LIB_SRC_DIR}/lhmodel_rapidjson.cxx" )

target_link_libraries( lhmodelutil
    PUBLIC
        lhmodel )

target_include_directories( lhmodelutil
    PUBLIC
        lhmodel
        "${LIB_INC_DIR}/lhmodelutil"
        "${RAPIDJSON_INCLUDE_DIRS}"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}" )

##############################################################
# unit tests
##############################################################

set( TST_SRC_DIR "./test" )

enable_testing()

add_executable( testlhmodelutil
    "${TST_SRC_DIR}/testlhmodelutil.cxx"
    "${TST_SRC_DIR}/ExampleModelA.cxx"
    "${TST_SRC_DIR}/ExampleModelB.cxx"
    "${TST_SRC_DIR}/ExampleModelC.cxx"
    "${TST_SRC_DIR}/ExampleModelD.cxx" )

target_link_libraries( testlhmodelutil
    PRIVATE
        "${GTEST_BOTH_LIBRARIES}"
        lhmodelutil )

target_include_directories( testlhmodelutil
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${GTEST_INCLUDE_DIRS}"
        "${LIB_INC_DIR}/lhmodelutil"
        "${LIB_INC_DIR}/lhmodelutil_test" )

include( GoogleTest )
gtest_add_tests( TARGET testlhmodelutil )

##############################################################
# installation
##############################################################

# library
install( TARGETS lhmodelutil
         LIBRARY
            DESTINATION lib
            COMPONENT lhmodelutil )

# include directory
install( DIRECTORY "${LIB_INC_DIR}/lhmodelutil/lhmodelutil"
         DESTINATION include
         COMPONENT lhmodelutil
         FILES_MATCHING PATTERN "*.h" )

ADD_CUSTOM_TARGET( install-lhmodelutil
                   ${CMAKE_COMMAND}
                   -D "CMAKE_INSTALL_COMPONENT=lhmodelutil"
                   -P "cmake_install.cmake" )
