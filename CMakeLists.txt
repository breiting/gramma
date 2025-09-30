cmake_minimum_required(VERSION 3.16)
project(gramma LANGUAGES CXX)

# Produce json file for neovim clangd completion
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Load compiler settings target
include(cmake/CompilerSettings.cmake)

# Load embed text functionality and create assets folder
include(cmake/EmbedText.cmake)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/generated/assets")


# Options
option(GRAMMA_BUILD_HELLO "Build hello app" ON)
option(GRAMMA_BUILD_BOUNCE "Build bouncing app" ON)


# -------- Dependencies (fetch once, available everywhere) --------
include(FetchContent)

# GLM
FetchContent_Declare(
		glm
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
		glfw
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		GIT_TAG 3.4
)
FetchContent_MakeAvailable(glfw)

# glad (local)
add_library(glad STATIC extern/glad/src/gl.cpp)
target_include_directories(glad PUBLIC extern/glad/include)

# Common include dir
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Lib: gramma
add_library(gramma
		src/core/Runner.cpp
		src/core/Time.cpp
		src/core/Window.cpp
		src/model/World2D.cpp
		src/view/Camera2D.cpp
		src/view/CircleBatch.cpp
										# src/view/Font.cpp
		src/view/Hud.cpp
		src/view/LineBatch.cpp
		src/view/Mesh.cpp
		src/view/Panel.cpp
		src/view/QuadBatch.cpp
		src/view/Shader.cpp
										# src/view/TextElement.cpp

)


# -------------------------
# Shaders (embed to headers)
# -------------------------
set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/assets")
set(GEN_INC_DIR    "${CMAKE_BINARY_DIR}/generated")

embed_text_to_header("${SHADER_SRC_DIR}/phong.vert"  "${GEN_INC_DIR}/assets/phong_vert.h"  phong_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/phong.frag"  "${GEN_INC_DIR}/assets/phong_frag.h"  phong_frag_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/unlit.vert"  "${GEN_INC_DIR}/assets/unlit_vert.h"  unlit_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/unlit.frag"  "${GEN_INC_DIR}/assets/unlit_frag.h"  unlit_frag_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/circle.vert" "${GEN_INC_DIR}/assets/circle_vert.h" circle_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/circle.frag" "${GEN_INC_DIR}/assets/circle_frag.h" circle_frag_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/line.vert"   "${GEN_INC_DIR}/assets/line_vert.h"   line_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/line.frag"   "${GEN_INC_DIR}/assets/line_frag.h"   line_frag_glsl)


target_link_libraries(gramma
PUBLIC
	glfw glad glm::glm
)
target_compile_features(gramma PUBLIC cxx_std_17)

target_include_directories(gramma
PUBLIC
	inc
	${GEN_INC_DIR}
)

# Apps
if(GRAMMA_BUILD_HELLO)
	add_executable(hello
				apps/hello/main.cpp
				apps/hello/HelloApp.cpp
		)
	target_link_libraries(hello
				PRIVATE gramma glm::glm
		)
	target_include_directories(hello PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(hello PRIVATE cxx_std_17)
endif()

if(GRAMMA_BUILD_BOUNCE)
	add_executable(bounce
				apps/bounce/main.cpp
				apps/bounce/BounceApp.cpp
		)
	target_link_libraries(bounce
				PRIVATE gramma glm::glm
		)
	target_include_directories(bounce PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(bounce PRIVATE cxx_std_17)
endif()
