#version 460

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 0) out vec2 texCoord;

layout (set = 1, binding = 0) uniform TransformData {
    mat4 proj;
    mat4 view;
    mat4 model;
    vec4 texture; //x,y offset, z,w for scale
} transform;


void main()
{
    gl_Position = transform.proj * transform.view * transform.model * vec4(aPos, 1.0);
    texCoord = (aTexCoord * transform.texture.zw) + transform.texture.xy;
}