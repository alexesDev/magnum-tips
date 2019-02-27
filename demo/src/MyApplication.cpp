#include <Magnum/GL/DefaultFramebuffer.h>
#include <Magnum/Platform/Sdl2Application.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Timeline.h>
#include <Magnum/GL/Renderer.h>

#include <Magnum/DebugTools/ResourceManager.h>

#include <TranslateController.h>
#include <ScaleController.h>
#include <GridRenderer.h>
#include <LineRenderer.h>
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
    ScaleController *_scaleController;
    ThirdPersonCameraController *_cameraController;
    LineRenderer *_lineRenderer;

    DebugTools::ResourceManager _debugManager;
    SceneGraph::DrawableGroup3D _debugDrawables;
};

MyApplication::MyApplication(const Arguments& arguments): Platform::Application{arguments, Configuration{}.setSize({1280,1024}), GLConfiguration{}.setSampleCount(4)} {
  GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

  /* Configure camera */
  _cameraController = new ThirdPersonCameraController{_scene};
  _cameraController->camera()
    .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
    .setProjectionMatrix(Matrix4::perspectiveProjection(Deg(60.0f), 4.0f/3.0f, 0.01f, 200.0f))
    .setViewport(GL::defaultFramebuffer.viewport().size());

  /* Demo scene */
  _translateController = new TranslateController{&_scene, &_debugDrawables};
  _scaleController = new ScaleController{&_scene, &_debugDrawables};

  _cube = new Object3D{_scaleController};
  _cube->scale(Vector3(0.25));
  new CubeDrawable{*_cube, &_drawables};

  new GridRenderer{_scene, &_drawables};

  _lineRenderer = new LineRenderer{_scene, &_debugDrawables};

  // static lines (from, to, color)
  _lineRenderer->add({ 2, 0, 2 }, { 3, 0, 3 }, { 1, 0, 0 });
  _lineRenderer->add({ 2, 0, 2 }, { 3, 1, 3 }, { 0, 1, 0 });
  // TODO: bug, dont use Matrix4::lookAt
  _lineRenderer->add({ 3, 0, 3 }, { 3, 1, 3 }, { 0, 0, 1 });

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
    _scaleController->move(cameraRay);
  }
}

void MyApplication::mousePressEvent(MouseEvent& event) {
  if (event.button() == MouseEvent::Button::Left) {
    Vector2 screenPoint = Vector2{event.position()} / Vector2{windowSize()};
    Ray cameraRay = getCameraToViewportRay(_cameraController->camera(), screenPoint);

    _translateController->grab(cameraRay);
    _scaleController->grab(cameraRay);
  }
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {
  if (event.button() == MouseEvent::Button::Left) {
    _translateController->release();
    _scaleController->release();
  }
}

MAGNUM_APPLICATION_MAIN(MyApplication)
