cmake_minimum_required( VERSION 3.1.0 )

project ( Polynomial-Regression VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fsanitize=address" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu++0x -D__STDC_INT64__" )
set( Boost_USE_MULTITHREADED ON )
set( CMAKE_THREAD_PREFER_PTHREAD TRUE )
set( THREADS_PREFER_PTHREAD_FLAG TRUE )

################ VARIABLES ###########################
set( KERNEL_FILES src/kernel )
set( COMS_FILES ${KERNEL_FILES}/coms )
set( EXCEPTIONS_FILES ${KERNEL_FILES}/exceptions )
set( REGRESSIONS_FILES ${KERNEL_FILES}/regressions )
############# END VARIABLES ############################

############### SOURCE FILES ##############################
set(
    HEADER_FILES 
    src/main.hpp
    src/common.hpp
    ${KERNEL_FILES}/common.hpp
    ${COMS_FILES}/inc/coms.hpp
    ${COMS_FILES}/inc/abstracts.hpp
    ${EXCEPTIONS_FILES}/inc/exceptions.hpp
    ${REGRESSIONS_FILES}/inc/regressions.hpp
)
set(
    IMPLEMENTED_FILES
    src/main.cpp
    ${COMS_FILES}/src/coms.cpp
    ${COMS_FILES}/src/abstracts.cpp
    ${EXCEPTIONS_FILES}/src/exceptions.cpp
    ${REGRESSIONS_FILES}/src/regressions.cpp
)
############## END SOURCE FILES ###########################

#################### LINKING ##########################
include_directories(
    /usr/local/include
)

add_definitions(
    -UNDEBUG
    -DDTF_HEADER_ONLY
    -D__FLATJSON__CHILDS_TYPE=std::uint32_t
    -D__FLATJSON__VLEN_TYPE=std::uint32_t
)

add_executable(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${IMPLEMENTED_FILES}
)
#################### END LINKING ####################################



