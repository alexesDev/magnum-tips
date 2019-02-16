#include <Magnum/SceneGraph/Camera.h>
#include "MathUtils.h"

using namespace Magnum;

Ray getCameraToViewportRay(SceneGraph::Camera3D& camera, const Vector2& screenPoint) {
  Matrix4 inverseVP = (camera.projectionMatrix() * camera.cameraMatrix()).inverted();

  Float nx = (2.0f * screenPoint.x()) - 1.0f;
  Float ny = 1.0f - (2.0f * screenPoint.y());

  Vector3 nearPoint(nx, ny, -1.f);
  Vector3 midPoint (nx, ny,  0.0f);

  Vector3 rayOrigin = inverseVP.transformPoint(nearPoint);
  Vector3 rayTarget = inverseVP.transformPoint(midPoint);
  Vector3 rayDirection = (rayTarget - rayOrigin).normalized();

  return { rayOrigin, rayDirection };
}

