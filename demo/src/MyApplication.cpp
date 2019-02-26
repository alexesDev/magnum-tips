#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Timeline.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/DebugTools/ResourceManager.h>

#include <TranslateController.h>
#include <GridRenderer.h>
#include <ThirdPersonCameraController.h>

#include "CubeDrawable.h"

using namespace Magnum;

class MyApplication: public Platform::Application {
  public:
    explicit MyApplication(const Arguments& arguments);

  private:
    void drawEvent() override;
    void mouseMoveEvent(MouseMoveEvent& event) override;
    void mousePressEvent(MouseEvent& event) override;
    void mouseReleaseEvent(MouseEvent& event) override;

    Scene3D _scene;
    SceneGraph::DrawableGroup3D _drawables;
    Object3D *_cube;
    Timeline _timeline;

    TranslateController *_translateController;
    ThirdPersonCameraController *_cameraController;

    DebugTools::ResourceManager _debugManager;
    SceneGraph::DrawableGroup3D _debugDrawables;
};

MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments, Configuration{}.setSize({1280,1024}), GLConfiguration{}.setSampleCount(4)} {
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

  using namespace Math::Literals;

  /* Configure camera */
  _cameraController = new ThirdPersonCameraController{_scene};
  _cameraController->camera()
    .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
    .setProjectionMatrix(Matrix4::perspectiveProjection(Deg(60.0f), 4.0f/3.0f, 0.01f, 200.0f))
    .setViewport(GL::defaultFramebuffer.viewport().size());

  /* Demo scene */
  _translateController = new TranslateController{&_scene, &_debugDrawables};

  _cube = new Object3D{_translateController};
  _cube->scale(Vector3(0.25));
  new CubeDrawable{*_cube, &_drawables};

  new GridRenderer{_scene, &_drawables};

  _timeline.start();
}

void MyApplication::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

  GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
  _cameraController->camera().draw(_drawables);
  GL::Renderer::disable(GL::Renderer::Feature::DepthTest);

  _cameraController->camera().draw(_debugDrawables);

  swapBuffers();
  redraw();

  _timeline.nextFrame();
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {
  if (event.buttons() == MouseMoveEvent::Button::Right) {
    _cameraController->move(event.relativePosition());
    return;
  }

  if (event.buttons() == MouseMoveEvent::Button::Left) {
    Vector2 screenPoint = Vector2{event.position()} / Vector2{windowSize()};
    Ray cameraRay = getCameraToViewportRay(_cameraController->camera(), screenPoint);

    _translateController->move(cameraRay);
  }
}

void MyApplication::mousePressEvent(MouseEvent& event) {
  if (event.button() == MouseEvent::Button::Left) {
    Vector2 screenPoint = Vector2{event.position()} / Vector2{windowSize()};
    Ray cameraRay = getCameraToViewportRay(_cameraController->camera(), screenPoint);

    _translateController->grab(cameraRay);
  }
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {
  if (event.button() == MouseEvent::Button::Left) {
    _translateController->release();
  }
}

MAGNUM_APPLICATION_MAIN(MyApplication)
