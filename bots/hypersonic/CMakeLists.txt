cmake_minimum_required (VERSION 2.8)
project (HYPERSONIC)

set (PRJ_OUTPUT "hypersonic")
set (MTP_FLAGS "-Wall -Wpedantic -ggdb -std=c++11 -I\"${CMAKE_SOURCE_DIR}/../../common/\"")
set (CMAKE_CXX_FLAGS ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_DEBUG ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_MINSIZEREL ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_RELEASE ${MTP_FLAGS})

set (BIGFILE ".hypersonic.cc")
set (SRCS "../../common/cgame-common.cc" hypersonic.cc bomb.cc world.cc)
set (HEADERS "../../common/cgame-common.h" params.h utils.h world.h bomb.h tests.h)
set (TEST_SRCS "../../common/cgame-common-tests.cc" hypersonic-tests.cc)
set (ALLFILES ${HEADERS} ${SRCS})

include_directories (${HYPERSONIC_SOURCE_DIR}/common)

add_executable ("${PRJ_OUTPUT}-tests.exe" ${SRCS} ${TEST_SRCS})
target_compile_definitions ("${PRJ_OUTPUT}-tests.exe" PUBLIC "-DUSE_TESTS")

add_executable ("${PRJ_OUTPUT}-final.exe" ${BIGFILE})
add_custom_target (big_file
                   COMMAND cd "${CMAKE_SOURCE_DIR}" && cat ${ALLFILES} | grep -v ":grep-out:" > "${BIGFILE}")
add_dependencies ("${PRJ_OUTPUT}-final.exe" big_file)
