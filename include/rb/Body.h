#ifndef BODY_H_
#define BODY_H_

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtx/quaternion.hpp>

#include <AABB.h>
#include <GreedyCollider.h>

namespace RB
{

  class Body
  {
    friend class World;
  public:
    Body();
    Body(glm::vec3 _position, glm::quat _orientation,
      glm::vec3 _bvMin = glm::vec3(-1.0f), glm::vec3 _bvMax = glm::vec3(1.0f));

    glm::vec3 position = glm::vec3(0.0f);
    glm::quat orientation = glm::identity<glm::quat>();

    glm::vec3 linearVelocity = glm::vec3(0.0f);
    glm::vec3 accumulatedForce = glm::vec3(0.0f);
    //DO NOT SET MANUALLY
    float mass = 1.0f;
    float invMass = 1.0f; //TODO
    //Any value <= 0 is evaluated to be infinite mass
    void SetMass(float _mass);

    glm::vec3 angularVelocity = glm::vec3(0.0f);
    glm::vec3 accumulatedTorque = glm::vec3(0.0f);
    glm::mat3 inertiaTensor = glm::identity<glm::mat3>();
    glm::mat3 invWorldInertiaTensor;

    //Helper functions
    glm::vec4 getPosition4() { return glm::vec4(position, 1); }
    glm::mat3 getRotationMatrix3() { return glm::toMat3(orientation); }
    glm::mat4 getRotationMatrix4() { return glm::toMat4(orientation); }
    glm::vec3 getRotationEuler() { return glm::eulerAngles(orientation); }
    glm::mat4 getModelMat();

    void applyForceImpulse(glm::vec3 _force);
    void applyForceImpulseAtLocation(glm::vec3 _force, glm::vec3 _localLocation);

    void applyTorqueImpulse(glm::vec3 _torque);

    std::shared_ptr<AABB> boundingBox;
    std::shared_ptr<GreedyCollider> collider;

    //rotates local inertia tensor by orientation quat and inverses
    void CalcWorldInvInertiaTensor();

    //Calculates inertia tensor as if body is a sphere with _radius
    void CalcInertiaTensorSphere(float _radius);
    //Calculates intertia tensor as if body is box with given halfExtents
    void CalcInertiaTensorBox(glm::vec3 _halfExtents);

    void SetBVScale(glm::vec3 _scale);

    void kill();
       
  private:
    //Used to easily pass 
    std::weak_ptr<Body> selfRef;
    std::weak_ptr<Body> world;

    //provided by implementation to scale BV
    glm::mat4 scaleBVMat = glm::mat4(1);
  };

}//RB namespace
#endif