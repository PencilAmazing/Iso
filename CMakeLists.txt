cmake_minimum_required (VERSION 3.8)

project (Iso C CXX)

set(CMAKE_CXX_STANDARD 17)

if (EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY --preload-file D:/dev/Iso/bin/assets@/assets")
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
  message(WARNING "Emscripten active")
else()
  message(WARNING "NOt active")
endif ()

set(raylib_INSTALL_DIR "D:/dev/raylib-install")
#set(raylib_LIBRARY "${raylib_INSTALL_DIR}/bin/raylib.dll")
if (EMSCRIPTEN)
set(raylib_LIBRARY "${raylib_INSTALL_DIR}/lib/libraylib.a")
else()
set(raylib_LIBRARY "${raylib_INSTALL_DIR}/lib/raylib.lib")
endif()
set(raylib_INCLUDE_DIR "${raylib_INSTALL_DIR}/include")
set(raylib_DIR "D:/dev/raylib-install/lib/cmake/raylib/")
find_package(raylib 4.2.0 REQUIRED)

#link_directories("D:/dev/raylib/projects/VS2019/build/raylib/bin/x64/Release.DLL/")
#add_subdirectory(lib/raylib)

# Add source to this project's executable.
add_executable (Iso "src/main.cpp" "src/tileset/tileset.h" "src/control/control.h" "src/debug.h" "src/settings.h" "src/tileset/tileset.cpp" "src/gen/generate.h" "src/gen/generate.cpp"  "src/control/control.cpp" "src/tileset/tilemap.h" "src/tileset/tilemap.cpp")
target_link_libraries(Iso raylib)

set_target_properties(Iso PROPERTIES DEBUG_POSTFIX "_d")
if (EMSCRIPTEN)
file(COPY "${raylib_INSTALL_DIR}/lib/libraylib.a" DESTINATION ${CMAKE_BINARY_DIR})
else()
file(COPY "${raylib_INSTALL_DIR}/bin/raylib.dll" DESTINATION ${CMAKE_BINARY_DIR})
endif()