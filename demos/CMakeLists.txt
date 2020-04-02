cmake_minimum_required(VERSION 3.5)

project(pirate_demos)

# Common options
option(GAPS_DISABLE "Disable compilation with GAPS annotations" OFF)
option(GAPS_DEMOS "Enable compilation of all GAPS demo applications" ON)
option(CHANNEL_DEMO "Enable compilation of GAPS channel application" OFF)
option(PIRATE_SHMEM_FEATURE "support shared memory channels" OFF)
option(PNT_DEMO "Enable compilation of pnt application" OFF)
option(BUILD_ALL "Enable PIRATE_SHMEM_FEATURE, PIRATE_UNIT_TEST, GAPS_DEMOS, and GAPS_BENCH" OFF)

if(BUILD_ALL)
    SET(PIRATE_SHMEM_FEATURE ON BOOL)
    SET(PIRATE_UNIT_TEST ON BOOL)
    SET(GAPS_DEMOS ON BOOL)
    SET(GAPS_BENCH ON BOOL)
endif(BUILD_ALL)

if (NOT GAPS_DISABLE)
  # Use lld for demos if a linker is not already set.
  if (CMAKE_EXE_LINKER_FLAGS)
    string(FIND ${CMAKE_EXE_LINKER_FLAGS} -fuse-ld USE_LD_IDX)
  else()
    set(USE_LD_IDX -1)
  endif()

  if (USE_LD_IDX EQUAL -1)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
  endif()
endif()

# Common build options
set(PIRATE_C_FLAGS -Werror -Wall -Wextra -Wpedantic -fPIC)

set(PIRATE_STATIC_LIB libpirate_static)
set(PIRATE_SHARED_LIB libpirate_shared)
set(PIRATE_APP_LIBS pirate)

if (PIRATE_SHMEM_FEATURE)
    set(PIRATE_APP_LIBS ${PIRATE_APP_LIBS} pthread rt)
endif(PIRATE_SHMEM_FEATURE)

if (GAPS_DEMOS)
    add_subdirectory(simple_demo)
    add_subdirectory(time_demo)
    add_subdirectory(channel_demo)
    add_subdirectory(cxx_demo)
    add_subdirectory(pnt_demo/4.pirate)
    if (NOT GAPS_DISABLE)
        add_subdirectory(enclave_demo)
    endif(NOT GAPS_DISABLE)
endif(GAPS_DEMOS)

if (CHANNEL_DEMO)
    add_subdirectory(channel_demo)
endif(CHANNEL_DEMO)

if (PNT_DEMO)
    add_subdirectory(pnt_demo/4.pirate)
endif(PNT_DEMO)