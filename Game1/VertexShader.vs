#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

uniform float offsetX;
uniform float offsetY;

float newX = aPos.x + offsetX;
float newY = aPos.y + offsetY;
  
out vec3 ourColor; // output a color to the fragment shader

void main()
{
    gl_Position = vec4(newX, newY, aPos.z, 1.0);
    ourColor = vec3(newX, newY, aPos.z);
}     
