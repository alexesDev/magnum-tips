# Magnum tips

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
