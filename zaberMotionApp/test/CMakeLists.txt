cmake_minimum_required( VERSION 3.14 )
project( zaber_test LANGUAGES CXX )
set( CMAKE_CXX_STANDARD 17 )

file(GLOB ZML_TEST_SRC
  tests/*.cpp
  test_main.cpp
  ../src/zaberConnectionManager.cpp
)
file(GLOB_RECURSE ZML_MOCK_SRC zml_mock/**/*.cpp)

add_executable( zaber_test ${ZML_TEST_SRC} ${ZML_MOCK_SRC})

target_include_directories( zaber_test PRIVATE zml_mock ../src thirdparty/catch)

if( MSVC )
  target_compile_options( zaber_test PRIVATE $<$<CONFIG:DEBUG>:/${MSVC_RUNTIME_FLAG}d> $<$<CONFIG:RELEASE>:/${MSVC_RUNTIME_FLAG}> /W4 /WX /MP )
else()
  target_compile_options( zaber_test PRIVATE -g -Wall -Wextra -pedantic -Werror )
  target_link_libraries( zaber_test pthread )

  if( ZML_ARCH STREQUAL "arm" AND CMAKE_SYSTEM_NAME STREQUAL "Linux" )
    target_compile_options( zaber_test PRIVATE -Wno-psabi )
  endif()
endif()

enable_testing()
add_test(NAME zaber_test COMMAND zaber_test)
