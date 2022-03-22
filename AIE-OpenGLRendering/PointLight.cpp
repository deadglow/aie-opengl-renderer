#include "PointLight.h"

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

glm::vec4 PointLight::GetFinalColor()
{
    return glm::vec4(color * intensity, 1.0f);
}
