if (EMSCRIPTEN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY")
  set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so taht you can directly open it.
endif ()

cmake_minimum_required (VERSION 3.8)

project (Iso C CXX)

set(CMAKE_CXX_STANDARD 17)

set(raylib_INSTALL_DIR "D:/dev/raylib-install")
#set(raylib_LIBRARY "${raylib_INSTALL_DIR}/bin/raylib.dll")
set(raylib_LIBRARY "${raylib_INSTALL_DIR}/lib/raylib.lib")
set(raylib_INCLUDE_DIR "${raylib_INSTALL_DIR}/include")
set(raylib_DIR "D:/dev/raylib-install/lib/cmake/raylib/")
find_package(raylib REQUIRED)

#link_directories("D:/dev/raylib/projects/VS2019/build/raylib/bin/x64/Release.DLL/")
#add_subdirectory(lib/raylib)

# Add source to this project's executable.
add_executable (Iso "src/main.cpp" "src/tileset/tileset.h" "src/control/control.h" "src/debug.h" "src/settings.h" "src/tileset/tileset.cpp" "src/gen/generate.h" "src/gen/generate.cpp"  "src/control/control.cpp" "src/tileset/tilemap.h" "src/tileset/tilemap.cpp")
target_link_libraries(Iso raylib)

set_target_properties(Iso PROPERTIES DEBUG_POSTFIX "_d")
file(COPY "${raylib_INSTALL_DIR}/bin/raylib.dll" DESTINATION ${CMAKE_BINARY_DIR})