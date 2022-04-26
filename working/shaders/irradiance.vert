#version 460
layout (location = 0) in vec3 _Position;

layout (std140, binding = 0) uniform _Camera
{
	mat4 _Pmat;
	mat4 _iPmat;
	mat4 _Vmat;
	mat4 _iVmat;
};

out vec3 localPos;

void main()
{
    localPos = _Position;
    gl_Position = _Pmat * _Vmat * vec4(localPos, 1.0);
}