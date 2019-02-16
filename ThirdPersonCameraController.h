#ifndef ThirdPersonCameraController_h
#define ThirdPersonCameraController_h

#include <Magnum/SceneGraph/Camera.h>
#include "Types.h"

class ThirdPersonCameraController : public Object3D {
  public:
    explicit ThirdPersonCameraController(Object3D& object);

    Magnum::SceneGraph::Camera3D& camera() const {
      return *_camera;
    }

    Object3D& cameraObject() const {
      return *_cameraObject;
    }

    ThirdPersonCameraController& move(const Magnum::Vector2i& shift);

    ThirdPersonCameraController& setSpeed(const Magnum::Vector2 &speed) {
      _speed = speed;
      return *this;
    }

  private:
    Object3D* _yawObject;
    Object3D* _pitchObject;
    Object3D* _cameraObject;

    Magnum::SceneGraph::Camera3D* _camera;
    Magnum::Vector2 _speed{-0.05f, 0.05f};
};

#endif
