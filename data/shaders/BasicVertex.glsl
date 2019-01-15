#version 330 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNorm;

uniform mat4 MVP;
uniform vec4 colour;

out vec4 outScreenPos;
out vec4 outColour;

void main()
{
  outScreenPos = MVP * vec4(inVert, 1.0f);
  outColour = colour;
  gl_Position = outScreenPos;
}