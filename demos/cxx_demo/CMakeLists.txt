cmake_minimum_required(VERSION 3.5)

project(cxx_demo)
set(CMAKE_BUILD_TYPE RelwithDebInfo)

# Options
if (GAPS_DISABLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGAPS_DISABLE")
endif(GAPS_DISABLE)

# Dependencies
include_directories(../../libpirate)

# Build flags
set(BUILD_FLAGS "-Werror" "-Wall" "-Wextra" "-Wpedantic" "-O3")
if (NOT GAPS_DISABLE)
    set(BUILD_FLAGS ${BUILD_FLAGS} "-ffunction-sections" "-fdata-sections" "--target=x86_64-pc-linux-elf")
endif(NOT GAPS_DISABLE)

SET(READER_TGT reader_cxx)
SET(WRITER_TGT writer_cxx)

ADD_EXECUTABLE(${READER_TGT} reader.cpp)
ADD_EXECUTABLE(${WRITER_TGT} writer.cpp)

TARGET_LINK_LIBRARIES(${READER_TGT} LINK_PUBLIC ${PIRATE_APP_LIBS})
TARGET_LINK_LIBRARIES(${WRITER_TGT} LINK_PUBLIC ${PIRATE_APP_LIBS})
