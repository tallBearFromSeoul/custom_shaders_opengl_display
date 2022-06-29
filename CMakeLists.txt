cmake_minimum_required(VERSION 3.0)
project(display_opengl)
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(EXE_NAME disp_opengl.exe)
set(EXECUTABLE_OUTPUT_PATH "../")
set(SOURCES
	./src/main.cpp
)

include_directories(./include)
include_directories(/opt/homebrew/include)
include_directories(/usr/local/include)

add_executable(${EXE_NAME} ${SOURCES})

target_link_libraries(
	${EXE_NAME}
	/usr/local/lib/libglad.a
	/opt/homebrew/lib/libglfw.3.3.dylib
	/usr/local/lib/libassimp.5.2.0.dylib
	)

