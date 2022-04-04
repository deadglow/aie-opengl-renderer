#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init) : Light(color_init, intensity_init)
{
    transform.SetRotation(glm::quatLookAt(direction_init, { 0, 1, 0 }));
}

LightType DirectionalLight::GetType()
{
    return LightType::Directional;
}

LightShaderData DirectionalLight::ConstructShaderData()
{
    LightShaderData shaderData;
    shaderData.type = (int)GetType();
    shaderData.direction = transform.GetForward();
    shaderData.color = GetFinalColor();

    return shaderData;
}
