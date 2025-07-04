#pragma once
#include "CGObject.h"
#include "CGRenderState.h"
using namespace std;
class CGLight : public CGRenderState
{
public:
    enum class LightType {
        Directional, // 平行光
        Point,       // 点光源
        Spot         // 聚光灯
    };

    CGLight();
    ~CGLight();

    void setType(LightType type);

    void setAmbient(const glm::vec4& color);
    void setDiffuse(const glm::vec4& color);
    void setSpecular(const glm::vec4& color);

    void setPosition(const glm::vec4& position);

    void setSpotDirection(const glm::vec3& direction);
    void setSpotCutoff(float degrees);
    void setSpotExponent(float exponent);

    void setAttenuation(float constant, float linear, float quadratic);
    virtual ERenderState type() const { return RS_Light; }
    virtual void apply(const CGCamera*, CGRenderContext*, int) const { apply(); };
    void enable(bool enable);
    void apply()const;

    int Index() const;
    void setIndex(int idx);

    LightType getType() const;

private:
    int index = 0;
    LightType mType = LightType::Point;

    glm::vec4 mAmbient;
    glm::vec4 mDiffuse;
    glm::vec4 mSpecular;

    
    glm::vec3 mSpotDirection;
    float mSpotCutoff;
    float mSpotExponent;

    float mAttenuationConstant;
    float mAttenuationLinear;
    float mAttenuationQuadratic;

    bool mEnabled;
public:
    glm::vec4 mPosition;
};

namespace SceneLights {

    inline shared_ptr<CGLight> DirectionalLight = [] {
        std::shared_ptr<CGLight> light = std::make_shared<CGLight>();
        light->setType(CGLight::LightType::Directional);
        light->setAmbient(glm::vec4(0.2f));
        light->setDiffuse(glm::vec4(0.8f));
        light->setSpecular(glm::vec4(1.0f));
        light->setIndex(0);
        return light;
        }();

    inline shared_ptr<CGLight> PointLight = [] {
        std::shared_ptr<CGLight> light = std::make_shared<CGLight>();
        light->setType(CGLight::LightType::Point);
        light->setAmbient(glm::vec4(0.1f));
        light->setDiffuse(glm::vec4(0.9f));
        light->setSpecular(glm::vec4(1.0f));
        light->setAttenuation(1.0f, 0.0f, 0.0f);
        light->setIndex(1);
        return light;
        }();

    inline shared_ptr<CGLight> SpotLight = [] {
        std::shared_ptr<CGLight> light = std::make_shared<CGLight>();
        light->setType(CGLight::LightType::Spot);
        light->setSpotDirection(glm::vec3(0.0f, -1.0f, -1.0f));
        light->setSpotCutoff(60.0f);
        light->setSpotExponent(16.0f);
        light->setAmbient(glm::vec4(0.05f));
        light->setDiffuse(glm::vec4(0.9f));
        light->setSpecular(glm::vec4(1.0f));
        light->setAttenuation(1.0f, 0.0f, 0.0f);
        light->setIndex(2);
        return light;
        }();

}

