#version 450

//Attributes that we expect from the vertex buffer
layout (location = 0) in vec3 Position;
layout (location = 1) in vec4 VertColor;
uniform float _Time;

out vec4 VertColour;
out vec3 ndcPoint;



void main()
{
	vec3 offset = vec3(cos(_Time), sin(_Time), 0.0) * 0.1;
	vec3 finalPoint = vec3(Position) + offset;
	ndcPoint = finalPoint;
	gl_Position = vec4(finalPoint, 1.0);
	VertColour = VertColor.xyzw;
}