#version 400

out vec4 outCol;

uniform sampler2D texColour;
uniform sampler2D texDepth;

in vec2 uv;

void main()
{
  const float gamma = 1.8f;
  
  vec3 hdr = texture(texColour, uv).rgb;
  vec3 mapped;// = hdr / (hdr+vec3(1.0f));
  //vec3 mapped = hdr;
  
  mapped = pow(hdr, vec3(1.0f / gamma));
  
  outCol = vec4(mapped, 1.0f);
}