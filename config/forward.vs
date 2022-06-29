#version 330 core
layout (location = 3) in vec3 world_coords;
out vec4 vertexColor;
uniform mat4 K;
uniform mat4 pose;
uniform mat4 view;
uniform mat4 projection;
void main() {
	gl_Position = projection*view*vec4(world_coords, 1.0f);
	//gl_Position = projection*view*K*pose*vec4(world_coords, 1.0f);
	vertexColor = pose*vec4(0.0,0.3,0.0,1.0);
}
