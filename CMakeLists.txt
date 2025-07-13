cmake_minimum_required(VERSION 3.16)
project(tc)
add_compile_options(-fdiagnostics-color=always)
add_compile_options(-fexec-charset=cp932)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# ソースファイル
set(SOURCES
	OCR.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME}
	# OCR
	tesseract
	leptonica
	# Windows
	gdiplus
	gdi32
	ole32
)
