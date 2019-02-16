#include "ThirdPersonCameraController.h"

using namespace Magnum;

ThirdPersonCameraController::ThirdPersonCameraController(Object3D& object) : Object3D{&object} {
  _yawObject = new Object3D{this};
  _pitchObject = new Object3D{_yawObject};

  _cameraObject = new Object3D{_pitchObject};
  _cameraObject->setTransformation(Matrix4::lookAt({ 7, 7, 0 }, Vector3{}, Vector3::yAxis(1)));

  _camera = new SceneGraph::Camera3D{*_cameraObject};
}

ThirdPersonCameraController& ThirdPersonCameraController::move(const Magnum::Vector2i& shift) {
  Vector2 s = Vector2{shift} * _speed;

  _yawObject->rotate(Rad(s.x()), Vector3::yAxis(1));
  _pitchObject->rotate(Rad(s.y()), Vector3::zAxis(1));

  return *this;
}
