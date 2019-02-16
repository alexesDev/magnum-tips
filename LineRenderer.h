#ifndef LineRenderer_h
#define LineRenderer_h

#include <Corrade/Containers/Pointer.h>
#include <Magnum/Resource.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Shaders/Flat.h>

#include "Types.h"

class LineRendererOptions {
  public:
    explicit LineRendererOptions(const Magnum::Vector3 &from, const Magnum::Vector3 &to, const Magnum::Color3 color) :
      _from(from), _to(to), _color(color) {}

    Magnum::Vector3 color() const {
      return _color;
    }

    LineRendererOptions &setTo(const Magnum::Vector3& to) {
      _to = to;
      _dirty = true;
      return *this;
    }

    LineRendererOptions &setFrom(const Magnum::Vector3& from) {
      _from = from;
      _dirty = true;
      return *this;
    }

    LineRendererOptions &setColor(const Magnum::Color3& color) {
      _color = color;
      return *this;
    }

    Magnum::Matrix4 transformation();

  protected:
    Magnum::Vector3 _from;
    Magnum::Vector3 _to;
    Magnum::Matrix4 _transformation;
    Magnum::Color3 _color;
    bool _dirty = true;
};

class LineRenderer: public Magnum::SceneGraph::Drawable3D {
  public:
    explicit LineRenderer(Object3D& object, Magnum::SceneGraph::DrawableGroup3D* drawables = nullptr);

    LineRendererOptions* add(const Magnum::Vector3 &from, const Magnum::Vector3 &to, const Magnum::Color3 color = Magnum::Color3(1)) {
      LineRendererOptions *line = new LineRendererOptions(from, to, color);
      _lines.push_back(Corrade::Containers::Pointer<LineRendererOptions>(line));
      return line;
    }

  private:
    void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

    Magnum::Resource<Magnum::GL::AbstractShaderProgram, Magnum::Shaders::Flat3D> _shader;
    Magnum::Resource<Magnum::GL::Mesh> _mesh;
    Magnum::Resource<Magnum::GL::Buffer> _vertexBuffer;

    std::vector<Corrade::Containers::Pointer<LineRendererOptions>> _lines;
};

#endif
