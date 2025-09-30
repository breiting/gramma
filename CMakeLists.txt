cmake_minimum_required(VERSION 3.16)
project(gramma LANGUAGES CXX)

# Produce json file for neovim clangd completion
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# Dear ImGui
FetchContent_Declare(imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG        v1.92.3
)
FetchContent_MakeAvailable(imgui)



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
		src/view/Gui.cpp
		src/view/LineBatch.cpp
		src/view/Mesh.cpp
		src/view/Shader.cpp

	${imgui_SOURCE_DIR}/imgui.cpp
	${imgui_SOURCE_DIR}/imgui_draw.cpp
	${imgui_SOURCE_DIR}/imgui_tables.cpp
	${imgui_SOURCE_DIR}/imgui_widgets.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
	${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp

)
target_link_libraries(gramma
PUBLIC
	glfw glad glm::glm
)
target_compile_features(gramma PUBLIC cxx_std_17)

target_include_directories(gramma
PUBLIC
	inc
	${imgui_SOURCE_DIR}
	${imgui_SOURCE_DIR}/backends
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
endif()

if(GRAMMA_BUILD_BOUNCE)
	add_executable(bounce
				apps/bounce/main.cpp
				apps/bounce/BounceApp.cpp
		)
	target_link_libraries(bounce
				PRIVATE gramma glm::glm
		)
endif()
