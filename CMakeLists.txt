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
option(GRAMMA_BUILD_RESCUE "Build rescue app" ON)
option(GRAMMA_BUILD_PLAY "Build play app" ON)
option(GRAMMA_BUILD_PARTICLE "Build particle app" ON)

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

FetchContent_Declare(
		box2d
		GIT_REPOSITORY https://github.com/erincatto/box2d.git
		GIT_TAG v3.1.1
)

FetchContent_MakeAvailable(box2d)

FetchContent_Declare(
		imgui
		GIT_REPOSITORY https://github.com/ocornut/imgui.git
		GIT_TAG v1.92.3
)

FetchContent_MakeAvailable(imgui)

# glad (local)
add_library(glad STATIC extern/glad/src/gl.cpp)
target_include_directories(glad PUBLIC extern/glad/include)


# IMGUI
set(IMGUI_SRC
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
				${imgui_SOURCE_DIR}
				${imgui_SOURCE_DIR}/backends
)

target_link_libraries(imgui PUBLIC glfw)

# nlohmann/json
FetchContent_Declare(
		json
		GIT_REPOSITORY https://github.com/nlohmann/json.git
		GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)


# GUI Layer
add_library(gramma_ui
		src/ui/ImGuiLayer.cpp
)

target_include_directories(gramma_ui
PUBLIC inc
${imgui_SOURCE_DIR}
${imgui_SOURCE_DIR}/backends
extern
)

target_link_libraries(gramma_ui
		PUBLIC imgui
		PRIVATE glfw glad
)


# Common include dir
include_directories(${CMAKE_SOURCE_DIR}/inc)

# Lib: gramma
add_library(gramma
		src/core/LineOfSightService.cpp
		src/core/Runner.cpp
		src/core/Time.cpp
		src/core/Window.cpp

		src/model/Agent.cpp
		src/model/AgentFactory.cpp
		src/model/AgentSerializer.cpp
		src/model/Box2DPhysicsEngine.cpp
		src/model/DirectMovement.cpp
		src/model/EnergyNeed.cpp
		src/model/EnergyNeedTaskBuilder.cpp
		src/model/Environment.cpp
		src/model/Home.cpp
		src/model/MoveTask.cpp
		src/model/Particle.cpp
		src/model/ParticleLifeBehavior.cpp
		src/model/ParticleSystem.cpp
		src/model/RandomDetourMovement.cpp
		src/model/RestTask.cpp
		src/model/SafetyNeedTaskBuilder.cpp
		src/model/SeekAgentTask.cpp
		src/model/SeekResourceTask.cpp
		src/model/SequenceTask.cpp
		src/model/SimAgentFactory.cpp
		src/model/SimpleGridPhysicsEngine.cpp
		src/model/SocialNeed.cpp
		src/model/SocialNeedTaskBuilder.cpp
		src/model/SocialSensor.cpp
		src/model/TaskFactory.cpp
		src/model/VisionSensor.cpp

		src/view/AgentView.cpp
		src/view/Camera2D.cpp
		src/view/EnvironmentView.cpp
		src/view/HomeView.cpp
		src/view/LineShape.cpp
		src/view/ParticleView.cpp
		src/view/ParticleSystemView.cpp
		src/view/ResourceView.cpp
		src/view/Shader.cpp
		src/view/Texture.cpp
		src/view/TriangularMesh.cpp
)


# -------------------------
# Shaders (embed to headers)
# -------------------------
set(SHADER_SRC_DIR "${CMAKE_SOURCE_DIR}/assets")
set(GEN_INC_DIR    "${CMAKE_BINARY_DIR}/generated")

embed_text_to_header("${SHADER_SRC_DIR}/particle.vert"  "${GEN_INC_DIR}/assets/particle_vert.h"  particle_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/particle.frag"  "${GEN_INC_DIR}/assets/particle_frag.h"  particle_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/agent-body.vert"  "${GEN_INC_DIR}/assets/agent_body_vert.h"  agent_body_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/agent-body.frag"  "${GEN_INC_DIR}/assets/agent_body_frag.h"  agent_body_frag_glsl)

embed_text_to_header("${SHADER_SRC_DIR}/agent-line.vert"  "${GEN_INC_DIR}/assets/agent_line_vert.h"  agent_line_vert_glsl)
embed_text_to_header("${SHADER_SRC_DIR}/agent-line.frag"  "${GEN_INC_DIR}/assets/agent_line_frag.h"  agent_line_frag_glsl)

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
	box2d glfw glad glm::glm
	nlohmann_json::nlohmann_json
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
				PRIVATE gramma glm::glm gramma_ui
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
				PRIVATE gramma glm::glm gramma_ui
		)
	target_include_directories(rescue PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(rescue PRIVATE cxx_std_17)
endif()

if(GRAMMA_BUILD_PLAY)
	add_executable(play
				apps/play/main.cpp
				apps/play/PlayApp.cpp
		)
	target_link_libraries(play
				PRIVATE gramma glm::glm gramma_ui
		)
	target_include_directories(play PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(play PRIVATE cxx_std_17)
endif()

if(GRAMMA_BUILD_PARTICLE)
	add_executable(particle
				apps/particle/main.cpp
				apps/particle/ParticleApp.cpp
		)
	target_link_libraries(particle
				PRIVATE gramma glm::glm gramma_ui
		)
	target_include_directories(particle PRIVATE ${CMAKE_SOURCE_DIR}/inc ${CMAKE_SOURCE_DIR}/extern/glad/include)
	target_compile_features(particle PRIVATE cxx_std_17)
endif()

