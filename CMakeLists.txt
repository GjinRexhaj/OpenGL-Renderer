cmake_minimum_required(VERSION 3.30.5)

# Set some basic project attributes
project (OpenGL-Renderer
	VERSION 0.1
	DESCRIPTION "An OpenGL renderer with an ImGui docking GUI")


# include all imgui directories: opengl3 + glfw backends
set(IMGUI_SRC
	"external/imgui-docking/backends/imgui_impl_opengl3.h"
	"external/imgui-docking/backends/imgui_impl_opengl3.cpp"
	"external/imgui-docking/backends/imgui_impl_opengl3_loader.h"
	"external/imgui-docking/backends/imgui_impl_glfw.h"
	"external/imgui-docking/backends/imgui_impl_glfw.cpp"
	"external/imgui-docking/imgui.h"
	"external/imgui-docking/imgui.cpp"
	"external/imgui-docking/imgui_draw.cpp"
	"external/imgui-docking/imgui_tables.cpp"
	"external/imgui-docking/imgui_widgets.cpp"
	"external/imgui-docking/imgui_demo.cpp"
	)

# This project will output an executable file
add_executable(${PROJECT_NAME} "src/main.cpp"
	"src/framework.cpp" "src/framework.h"
	"src/FrameBuffer.cpp" "src/FrameBuffer.h"
	"src/graphics.cpp" "src/graphics.h"
	"src/Logger.cpp" "src/Logger.h"
	"src/Shader.cpp" "src/Shader.h"
	"src/TextureLoader.cpp" "src/TextureLoader.h"
	"external/glm/glm.hpp"
	"external/stb-master/stb_image.h"
	"src/Camera.h" "src/Camera.cpp"
	"external/glad/src/glad.c" ${IMGUI_SRC})

# add lib subdir layers
add_subdirectory(external/glfw-3.4)



# must have glm installed locally
# find_package(glm CONFIG REQUIRED)

# use absoulte references when including
target_include_directories(${PROJECT_NAME} 
	PUBLIC external/glfw-3.4/include
	PUBLIC external/glad/include
	PUBLIC external/imgui-docking
	PUBLIC external/glm
	PUBLIC external/stb-master
)


# links external libraries
target_link_directories(${PROJECT_NAME} 
	PRIVATE external/glfw-3.4/src
	PRIVATE external/glad/src
)

# link glfw to executable at build time
target_link_libraries(${PROJECT_NAME} glfw)



