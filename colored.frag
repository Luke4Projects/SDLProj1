#version 460

layout (location = 0) out vec4 FragColor;
layout (location = 0) in vec2 texCoord;
layout (location = 1) in vec4 uColor;
layout (set = 2, binding = 0) uniform sampler2D aTexture;

void main() {
    vec4 texColor = texture(aTexture, texCoord) * uColor;
    if(texColor.a < 0.1) {
        discard;
    }
    FragColor = texColor;
}