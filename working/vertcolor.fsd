#version 450

in vec4 VertColour;
in vec3 ndcPoint;
out vec4 FragColour;

//uniform goes here
uniform float _Time;

void main()
{
	float angle = atan(ndcPoint.y / ndcPoint.x);

	float s = (sin(_Time) + 2) * 3;
	float t = abs((cos(angle * s + _Time) + 1) / 2);
	float multiplier = 1 - step(1, 1 - (t * 0.5 - dot(ndcPoint, ndcPoint)));
	FragColour = VertColour.xyzw * multiplier;
}