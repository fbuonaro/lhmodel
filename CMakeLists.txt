cmake_minimum_required( VERSION 3.10 )

project( lhmodel 
         DESCRIPTION "lhmodel libraries"
         HOMEPAGE_URL "https://github.com/fbuonaro/lhmodel"
         LANGUAGES CXX
         VERSION 1.0.0 )

enable_testing()

add_subdirectory( liblhmodel )
add_subdirectory( liblhmodelutil )