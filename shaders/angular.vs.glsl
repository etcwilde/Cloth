#version 330
// vPosition is in spherical coordinates
layout(location = 0) in vec3 vPosition;
uniform mat4 MVP;

void main()
{

        gl_Position = MVP * vec4(
                        vPosition.x * sin(vPosition.y) * cos(vPosition.z), // y
                        vPosition.x * cos(vPosition.z),  // z
                        vPosition.x * cos(vPosition.y) * sin(vPosition.z), // x
                        1.);
}
