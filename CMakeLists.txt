cmake_minimum_required(VERSION 3.16)
project(tc)
add_compile_options(-fdiagnostics-color=always)
add_compile_options(-fexec-charset=cp932)
add_compile_options(-g)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# ソースファイル
set(SOURCES
	OCR.cpp
	capture.cpp
	selectRect.cpp
	main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "C:/msys64/mingw64/include/qt6")
target_link_libraries(${PROJECT_NAME}
	# Qt6
	Qt6::Widgets
	# OCR
	tesseract
	leptonica
	# Windows
	gdiplus
	gdi32
	ole32
)
