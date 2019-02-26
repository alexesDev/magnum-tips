# Magnum tips

### TranslateController

```cpp
_translateController = new TranslateController{&_scene, &_debugDrawables};

// controller only works if it has children
Object3D *cube = new Object3D{_translateController};
new CubeDrawable{*_cube, &_drawables};

// disable controller
cube->setParent(&_scene);

// handle mouse events
void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {
  Vector2 screenPoint = Vector2{event.position()} / Vector2{windowSize()};
  Ray cameraRay = getCameraToViewportRay(*_camera, screenPoint);

  _translateController->move(cameraRay);
}

void MyApplication::mousePressEvent(MouseEvent& event) {
  if (event.button() == MouseEvent::Button::Left) {
    Vector2 screenPoint = Vector2{event.position()} / Vector2{windowSize()};
    Ray cameraRay = getCameraToViewportRay(*_camera, screenPoint);

    _translateController->grab(cameraRay);
  }
}

void MyApplication::mouseReleaseEvent(MouseEvent& event) {
  if (event.button() == MouseEvent::Button::Left) {
    _translateController->release();
  }
}
```

![TranslateController](https://raw.githubusercontent.com/alexesDev/magnum-tips/master/TranslateController.gif)

### LineRenderer

```cpp
_lineRenderer = new LineRenderer{_scene, &_debugDrawables};

// static lines (from, to, color)
_lineRenderer->add({ 0, 0, 0 }, { 1, 1, 1 }, { 1, 0, 0 });
_lineRenderer->add({ -1, 2, 0 }, { 1, 1, 1 }, { 0, 1, 0 });

// dynamic line
auto line = _lineRenderer->add({ 0, 0, 0 }, { 1, 1, 1 });
line->setTo({ -1, 2, 0 });
```

![LineRenderer](https://raw.githubusercontent.com/alexesDev/magnum-tips/master/LineRenderer.gif)

### ThirdPersonCameraController

```cpp
_cameraController = new ThirdPersonCameraController{_scene};
_cameraController->camera()
  .setAspectRatioPolicy(SceneGraph::AspectRatioPolicy::Extend)
  .setProjectionMatrix(Matrix4::perspectiveProjection(Deg(60.0f), 4.0f/3.0f, 0.01f, 200.0f))
  .setViewport(GL::defaultFramebuffer.viewport().size());

// ...

_cameraController->translate({ 5, 0, 0 });

// ...

void MyApplication::drawEvent() {
  GL::defaultFramebuffer.clear(GL::FramebufferClear::Color|GL::FramebufferClear::Depth);

  _cameraController->camera().draw(_drawables);

  // ...
}

void MyApplication::mouseMoveEvent(MouseMoveEvent& event) {
  if (event.buttons() == MouseMoveEvent::Button::Left) {
    _cameraController->move(event.relativePosition());
  }
}
```

![ThirdPersonCameraController](https://raw.githubusercontent.com/alexesDev/magnum-tips/master/ThirdPersonCameraController.gif)

### GridRenderer

```cpp
DebugTools::ResourceManager _debugManager;
SceneGraph::DrawableGroup3D _debugDrawables;

Scene3D _scene;

new GridRenderer{_scene, &_debugDrawables};

_camera->draw(_debugDrawables);
```

![GridRenderer](https://raw.githubusercontent.com/alexesDev/magnum-tips/master/GridRenderer.png)

### MathUtils

getCameraToViewportRay:

```cpp
Vector2 screenPoint = Vector2{_mousePosition} / Vector2{windowSize()};
Ray cameraRay = getCameraToViewportRay(_cameraController->camera(), screenPoint);

Vector4 ground = Math::planeEquation(Vector3{0, 1, 0}, Vector3(0));
Float t = Math::Intersection::planeLine(ground, cameraRay.origin, cameraRay.direction);

if (!Magnum::Math::isInf(t) && !Magnum::Math::isNan(t)) {
  Vector3 point = cameraRay.origin + cameraRay.direction * t;
  _line->setTo(point);
}
```
