#version 330 core

layout (location = 0) out vec4 fragCol;

in vec4 outScreenPos;
in vec4 outColour;

void main()
{
  fragCol = outColour;
}