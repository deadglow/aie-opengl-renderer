#include "Light.h"

Light::Light(glm::vec3 color_init, float intensity_init)
{
    color = color_init;
    intensity = intensity_init;
}

Light::Light()
{
}

Light::~Light()
{
}

int Light::GetType()
{
    return 0;
}

glm::vec4 Light::GetFinalColor()
{
    return glm::vec4(color * intensity, 1.0f);
}

LightShaderData Light::ConstructShaderData()
{
    return LightShaderData();
}
