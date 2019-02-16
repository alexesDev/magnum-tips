#ifndef MathUtils_h
#define MathUtils_h

#include <Magnum/Magnum.h>

struct Ray {
  Magnum::Vector3 origin;
  Magnum::Vector3 direction;
};

Ray getCameraToViewportRay(Magnum::SceneGraph::Camera3D& camera, const Magnum::Vector2& screenPoint);

#endif
