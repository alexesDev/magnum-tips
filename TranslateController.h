#ifndef TranslateController_h
#define TranslateController_h

#include <Magnum/DebugTools/ObjectRenderer.h>
#include <Magnum/SceneGraph/Drawable.h>

#include <Types.h>
#include <MathUtils.h>

class TranslateController : public Object3D {
  public:
    explicit TranslateController(Object3D *parent = nullptr, Magnum::SceneGraph::DrawableGroup3D *group = nullptr);

    void grab(const Ray &cameraRay);
    void move(const Ray &cameraRay);
    void release();

  private:
    Magnum::Vector3 _startPosition;
    Magnum::Vector3 _startPoint;
    Magnum::Vector3 _dir;
    Magnum::Vector4 _plane;
    Magnum::Float _axisThreshold = 0.3f;

    bool _missed = true;
};

#endif
