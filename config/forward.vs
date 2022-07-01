#version 330 core
layout (location = 0) in vec3 world_coords;
out vec4 vertexColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
	gl_Position = projection*view*model*vec4(world_coords, 1.0f);
	vertexColor = vec4(1.0,0.2,0.0,0.04);
}
