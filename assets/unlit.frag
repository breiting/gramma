#version 330 core

uniform sampler2D uTexture;
uniform bool uUseTexture;

in vec2 vTex;
in vec4 vCol;

out vec4 fragColor;

void main() {
  if (uUseTexture) {
    fragColor = texture(uTexture, vTex) * vCol;
  } else {
    fragColor = vCol;
  }
}
