cmake_minimum_required (VERSION 3.8)

project (Iso C CXX)

set(CMAKE_CXX_STANDARD 17)

#link_directories("D:/dev/raylib/projects/VS2019/build/raylib/bin/x64/Release.DLL/")
add_subdirectory(lib/raylib)

# Add source to this project's executable.
add_executable (Iso "src/main.cpp" "src/tileset/tileset.h" "src/control/control.h" "src/debug.h" "src/settings.h" "src/tileset/tileset.cpp" "src/gen/generate.h" "src/gen/generate.cpp"  "src/control/control.cpp")
target_link_libraries(Iso raylib)

set_target_properties(Iso PROPERTIES DEBUG_POSTFIX "_d")