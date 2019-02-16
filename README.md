# Magnum tips

### GridRenderer

```cpp
DebugTools::ResourceManager _debugManager;
SceneGraph::DrawableGroup3D _debugDrawables;

Scene3D _scene;

new GridRenderer{_scene, &_debugDrawables};

_camera->draw(_debugDrawables);
```

![GridRenderer](https://raw.githubusercontent.com/alexesDev/magnum-tips/master/GridRenderer.png)
