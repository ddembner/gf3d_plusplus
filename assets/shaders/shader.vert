#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertColor;

layout(binding = 0, set = 0) uniform CameraUBO
{
    mat4 viewMatrix;
    mat4 projectMatrix;
    mat4 viewProjectMatrix;
} cameraUBO;

layout(location = 0) out vec3 fragColor;

void main(){
    //mat4 objectMatrix = mat4(1.0); 
    gl_Position =  vec4(vertPosition, 1.0);
    //gl_Position = push.viewProjection * objectMatrix * vec4(vertPosition, 1.0);
    fragColor = vertColor;
}