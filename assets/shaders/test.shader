#type vertex
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertColor;

layout(push_constant) uniform Push
{
    mat4 mvp;
    vec4 color;
} push;

layout(binding = 0) uniform UniformScene{
    mat4 view;
    mat4 projection;
    vec3 cameraPos;
} scene;

layout(location = 0) out vec3 fragColor;

void main(){ 
    gl_Position = push.mvp * vec4(vertPosition, 1.0);
    fragColor = push.color.xyz;
}

#type fragment
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragColor;

layout(push_constant) uniform Push
{ 
    mat4 mvp;
    vec4 color;
} push;

layout(location = 0) out vec4 outColor;

void main(){
	outColor = push.color;
}