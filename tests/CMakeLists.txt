include_directories("${CMAKE_SOURCE_DIR}/include")
file(GLOB_RECURSE SOURCES "Lums/*.cpp")
add_executable(TestSuiteLums ${SOURCES})
target_link_libraries(TestSuiteLums LumsTest)

add_custom_target(
  Tests_Lums TestSuiteLums
  DEPENDS
    TestSuiteLums
  VERBATIM
  USES_TERMINAL
)
