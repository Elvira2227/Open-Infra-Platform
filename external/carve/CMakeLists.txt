cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0048 NEW)

project(carve)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(carve_VERSION_MAJOR 2)
set(carve_VERSION_MINOR 0)
set(carve_VERSION_PATCH 0a)

set(CARVE_VERSION ${carve_VERSION_MAJOR}.${carve_VERSION_MINOR}.${carve_VERSION_PATCH})

option(BUILD_COVERAGE                    "Compile with gcov"                                 OFF)
option(BUILD_SHARED_LIBS                 "Compile libcarve as shared"                        ON)
option(CARVE_WITH_GUI                    "Compile gui code"                                  OFF)
option(CARVE_DEBUG                       "Compile in debug code"                             OFF)
option(CARVE_DEBUG_WRITE_PLY_DATA        "Write geometry output during debug"                OFF)
option(CARVE_USE_EXACT_PREDICATES        "Use Shewchuk's exact predicates, where possible"   OFF)
option(CARVE_INTERSECT_GLU_TRIANGULATOR  "Include support for GLU triangulator in intersect" OFF)
option(CARVE_GTEST_TESTS                 "Complie gtest, and dependent tests"                OFF)

if (MSVC)
  # For MSVC, CMake sets certain flags to defaults we want to override.
  # This replacement code is taken from sample in the CMake Wiki at
  # http://www.cmake.org/Wiki/CMake_FAQ#Dynamic_Replace.
  foreach (flag_var
           CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
           CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
    # In hermetic build environments, tests may not have access to MS runtime
    # DLLs, so this replaces /MD (CRT libraries in DLLs) with /MT (static CRT
    # libraries).
    #string(REPLACE "/MD" "-MT" ${flag_var} "${${flag_var}}")
    # We prefer more strict warning checking for building Google Test.
    # Replaces /W3 with /W4 in defaults.
    string(REPLACE "/W3" "-W4" ${flag_var} "${${flag_var}}")
  endforeach()
endif()

if (BUILD_COVERAGE)
  set(BUILD_SHARED_LIBS OFF)
  set(CMAKE_BUILD_TYPE DEBUG)
  IF (CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
    SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
  ENDIF()
endif(BUILD_COVERAGE)

if(WIN32)
  set(BUILD_SHARED_LIBS OFF) # until everything is exported
  add_definitions(-D_USE_MATH_DEFINES)
  add_definitions(-DNOMINMAX)
endif(WIN32)

find_package(Boost 1.57)
include_directories(${Boost_INCLUDE_DIRS})
message(STATUS "Using system boost")

if(CARVE_WITH_GUI)
  find_package(OpenGL)
  find_package(GLUT)

  if(NOT OPENGL_FOUND)
    message(WARNING "Unable to locate OpenGL")
    set(CARVE_WITH_GUI OFF)

  elseif(NOT GLUT_FOUND)
    message(WARNING "Unable to locate GLUT")
    set(CARVE_WITH_GUI OFF)

  else(OPENGL_FOUND AND GLUT_FOUND)
    message(STATUS "Found OpenGL and GLUT")
    include_directories(${OPENGL_INCLUDE_DIR})
    include_directories(${GLUT_INCLUDE_DIR})
    if(WIN32)
      add_definitions(-DGLUI_NO_LIB_PRAGMA)
      add_definitions(-DGLUI_USE_STATIC_LIB)
      add_definitions(-DGLEW_STATIC)
    endif(WIN32)
    add_subdirectory(external/GLEW) 
    add_subdirectory(external/GLUI) 

  endif(NOT OPENGL_FOUND)

endif(CARVE_WITH_GUI)

add_subdirectory(external/GLOOP) 

if (CARVE_GTEST_TESTS)
  add_subdirectory(external/gtest-1.5.0)
endif(CARVE_GTEST_TESTS)

configure_file (
  "${carve_SOURCE_DIR}/include/carve/cmake-config.h.in"
  "${carve_BINARY_DIR}/include/carve/config.h"
)
add_definitions(-DCMAKE_BUILD)

include_directories(${carve_BINARY_DIR}/include)

add_subdirectory(lib) 
add_subdirectory(include) 
add_subdirectory(common) 
add_subdirectory(src) 
add_subdirectory(examples) 
add_subdirectory(tests) 


# include(CTest)

if(BUILD_TESTING)
  ENABLE_TESTING()
endif(BUILD_TESTING)
