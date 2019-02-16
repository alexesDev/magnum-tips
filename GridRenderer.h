#ifndef GridRenderer_h
#define GridRenderer_h

#include <Magnum/Resource.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/GL/Buffer.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Shaders/VertexColor.h>

#include "Types.h"

class GridRenderer: public Magnum::SceneGraph::Drawable3D {
  public:
    explicit GridRenderer(Object3D& object, Magnum::SceneGraph::DrawableGroup3D* drawables = nullptr);

  private:
    void draw(const Magnum::Matrix4& transformationMatrix, Magnum::SceneGraph::Camera3D& camera) override;

    Magnum::Resource<Magnum::GL::AbstractShaderProgram, Magnum::Shaders::VertexColor3D> _shader;
    Magnum::Resource<Magnum::GL::Mesh> _mesh;
    Magnum::Resource<Magnum::GL::Buffer> _vertexBuffer;
};

#endif
