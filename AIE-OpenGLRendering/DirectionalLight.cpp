#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init)
{
    direction = direction_init;
    color = color_init;
    intensity = intensity_init;
}

DirectionalLight::DirectionalLight()
{
    direction = glm::normalize(glm::vec3( 1, 1, 1 ));
    color = { 1.0f, 1.0f, 1.0f};
    intensity = 1.0f;
}

DirectionalLight::~DirectionalLight()
{
}


glm::vec4 DirectionalLight::GetFinalColor()
{
    return glm::vec4(color * intensity, 1.0f);
}
