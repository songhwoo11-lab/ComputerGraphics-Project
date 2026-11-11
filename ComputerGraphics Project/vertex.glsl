#version 330 core

uniform mat4 world;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTextCoord;

out vec3 FragNormal;
out vec3 FragPos;

void main()
{
	FragPos = vec3(world * vec4(vPos, 1.0));
	FragNormal = vec3(transpose(inverse(world)) * vec4(vNormal, 1.0));

	gl_Position = projection * view * world * vec4(vPos, 1.0);
}