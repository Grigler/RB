#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <glm/glm.hpp>

class Transform
{
public:
  glm::vec3 pos;
  glm::vec3 rot;

  //TODO - movement/rotation helper funcs

  //Gets model mat of the object
  //non-hierarchical for simplicity of example
  glm::mat4 getModelMat();
  glm::mat4 getRotationMat();

  glm::vec3 getUp();
  glm::vec3 getForward();

  //static const vars for up, right and forward
  static const glm::vec3 forward;
  static const glm::vec3 up;
  static const glm::vec3 right;

private:

};


#endif