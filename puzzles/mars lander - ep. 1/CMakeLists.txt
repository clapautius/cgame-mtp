cmake_minimum_required (VERSION 2.8)

# change the lines below
project (MARS_LANDER_1)
set (PRJ_OUTPUT "mars-lander-1")
set (MTP_FLAGS "-Wall -Wpedantic -ggdb -std=c++11")
set (SRCS "../../common/cgame-common.cc"
  mars-lander-1.cc)
set (HEADERS "../../common/cgame-common.h")
set (TEST_SRCS "../../common/cgame-common-tests.cc"
  mars-lander-1-tests.cc)


set (CMAKE_CXX_FLAGS ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_DEBUG ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_MINSIZEREL ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_RELWITHDEBINFO ${MTP_FLAGS})
set (CMAKE_CXX_FLAGS_RELEASE ${MTP_FLAGS})

set (BIGFILE ".${PRJ_OUTPUT}.cc")

set (ALLFILES ${HEADERS} ${SRCS})

include_directories (${CMAKE_SOURCE_DIR}/../../common)

add_executable ("${PRJ_OUTPUT}-tests.exe" ${SRCS} ${TEST_SRCS})
target_compile_definitions ("${PRJ_OUTPUT}-tests.exe" PUBLIC "-DUSE_TESTS")

add_executable ("${PRJ_OUTPUT}-final.exe" ${SRCS})

add_custom_target (big_file
                   COMMAND cd "${CMAKE_SOURCE_DIR}" && cat ${ALLFILES} | grep -v ":grep-out:" > "${BIGFILE}")
add_dependencies ("${PRJ_OUTPUT}-final.exe" big_file)

if (WIN32)
   add_custom_target (clipboard ALL
                     COMMAND cat "${BIGFILE}" | clip
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
else ()
   add_custom_target (clipboard ALL
                     COMMAND cat "${BIGFILE}" | xclip -selection clipboard -display ":0"
                     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif ()
add_dependencies (clipboard big_file)
