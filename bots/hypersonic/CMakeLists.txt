cmake_minimum_required (VERSION 2.8)
project (HYPERSONIC)

set (BIGFILE ".hypersonic.cc")
set (SRCS "${HYPERSONIC_SOURCE_DIR}/common/cgame-common.cc" hypersonic.cc bomb.cc world.cc)
set (HEADERS "${HYPERSONIC_SOURCE_DIR}/common/cgame-common.h" params.h utils.h world.h bomb.h)
set (ALLFILES ${HEADERS} ${SRCS})

include_directories (${HYPERSONIC_SOURCE_DIR}/common)

add_custom_command (OUTPUT "${BIG_FILENAME}"
                   cat ${ALLFILES} | grep -v ":grep-out:" > "${BIGFILE}")

#add_executable (tests.exe )

add_executable (final.exe ${BIGFILE})
