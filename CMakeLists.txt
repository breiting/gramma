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
option(GRAMMA_BUILD_SIM "Build sim app" ON)
option(GRAMMA_BUILD_RESCUE "Build rescue app" OFF)

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

		src/model/Agent.cpp
		src/model/AgentFactory.cpp
		src/model/BruteForceCollisionHandler.cpp
		src/model/Environment.cpp
		src/model/ExerciseNeed.cpp
		src/model/FoodSource.cpp
		src/model/HungerNeed.cpp
		src/model/KDTreeCollisionHandler.cpp
		src/model/RandomWalkTask.cpp
		src/model/Room.cpp
		src/model/SeekFoodTask.cpp
		src/model/VisionSensor.cpp

		src/view/Camera2D.cpp
		src/view/AgentView.cpp
		src/view/AgentBatchView.cpp
		src/view/FoodView.cpp
		src/view/EnvironmentView.cpp
		src/view/RoomView.cpp
		src/view/LineShape.cpp
		src/view/Shader.cpp
		src/view/Texture.cpp
		src/view/TriangularMesh.cpp
)


# -------------------------
# Shaders (embed to headers)
# -------------------------
set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/assets")
set(GEN_INC_DIR    "${CMAKE_BINARY_DIR}/generated")

embed_text_to_header("${SHADER_SRC_DIR}/agent.vert"  "${GEN_INC_DIR}/assets/agent_vert.h"  agent_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/agent.frag"  "${GEN_INC_DIR}/assets/agent_frag.h"  agent_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/agentbatch.vert"  "${GEN_INC_DIR}/assets/agentbatch_vert.h"  agentbatch_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/agentbatch.frag"  "${GEN_INC_DIR}/assets/agentbatch_frag.h"  agentbatch_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/food.vert"  "${GEN_INC_DIR}/assets/food_vert.h"  food_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/food.frag"  "${GEN_INC_DIR}/assets/food_frag.h"  food_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/unlit.vert"  "${GEN_INC_DIR}/assets/unlit_vert.h"  unlit_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/unlit.frag"  "${GEN_INC_DIR}/assets/unlit_frag.h"  unlit_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/basic.vert"  "${GEN_INC_DIR}/assets/basic_vert.h"  basic_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/basic.frag"  "${GEN_INC_DIR}/assets/basic_frag.h"  basic_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/line.vert"   "${GEN_INC_DIR}/assets/line_vert.h"   line_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/line.frag"   "${GEN_INC_DIR}/assets/line_frag.h"   line_frag_glsl)

target_link_libraries(gramma
PUBLIC
	project_settings
	glfw glad glm::glm
)
target_compile_features(gramma PUBLIC cxx_std_17)

target_include_directories(gramma
PUBLIC
	inc
	extern/stb
	extern/nanoflann
	${GEN_INC_DIR}
)

# Apps
if(GRAMMA_BUILD_SIM)
	add_executable(sim
				apps/sim/main.cpp
				apps/sim/SimApp.cpp
		)
	target_link_libraries(sim
				PRIVATE gramma glm::glm
		)
	target_include_directories(sim PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(sim PRIVATE cxx_std_17)
endif()

if(GRAMMA_BUILD_RESCUE)
	add_executable(rescue
				apps/rescue/main.cpp
				apps/rescue/RescueApp.cpp
		)
	target_link_libraries(rescue
				PRIVATE gramma glm::glm
		)
	target_include_directories(rescue PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(rescue PRIVATE cxx_std_17)
endif()
