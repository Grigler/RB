#ifndef DRAWOBJECT_H_
#define DRAWOBJECT_H_

#include <ngl/NGLInit.h>
#include <ngl/ShaderLib.h>
#include <ngl/VAOPrimitives.h>
#include <ngl/NGLStream.h>

class DrawObject
{
public:
  virtual void Init() {}
  virtual void Draw() {}

private:
  ngl::ShaderLib *shader = nullptr;

};

#endif