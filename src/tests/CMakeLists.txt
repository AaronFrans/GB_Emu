include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})
include_directories("/usr/local/include")
link_directories(${CHECK_LIBRARY_DIRS})

set(TEST_SOURCES
  check_gbe.c
)

add_executable(check_gbe ${TEST_SOURCES})
target_link_libraries(check_gbe emu ${CHECK_LIBRARIES})
target_include_directories(check_gbe PRIVATE ${PROJECT_SOURCE_DIR}/include )


find_program(DEBIAN "dpkg")
if(DEBIAN)
    target_link_libraries(check_gbe check m rt pthread subunit)
endif()

if (WIN32)
target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/../windows_deps/check )

# check_gbe links emu, which needs the SDL2/SDL2_ttf DLLs at runtime.
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(RUNTIME_DLL_ARCH_DIR x64)
else ()
  set(RUNTIME_DLL_ARCH_DIR x86)
endif ()

add_custom_command(
  TARGET check_gbe POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2/lib/${RUNTIME_DLL_ARCH_DIR}/SDL2.dll
  $<TARGET_FILE_DIR:check_gbe>
  VERBATIM)
foreach(dll_name SDL2_ttf.dll zlib1.dll libfreetype-6.dll)
  add_custom_command(
    TARGET check_gbe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PROJECT_SOURCE_DIR}/../windows_deps/sdl2_ttf/lib/${RUNTIME_DLL_ARCH_DIR}/${dll_name}
    $<TARGET_FILE_DIR:check_gbe>
    VERBATIM)
endforeach()
endif()
