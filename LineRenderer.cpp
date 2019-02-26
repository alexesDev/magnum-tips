#include "LineRenderer.h"

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/DebugTools/ResourceManager.h>

using namespace Magnum;

constexpr std::array<Vector3, 2> positions{{
  { 0, 0, 0 },
  { 0, 0, -1.f }
}};

LineRenderer::LineRenderer(Object3D& object, Magnum::SceneGraph::DrawableGroup3D* drawables) : SceneGraph::Drawable3D{object, drawables} {
  _shader = DebugTools::ResourceManager::instance().get<GL::AbstractShaderProgram, Shaders::Flat3D>("FlatShader3D");
  if(!_shader) DebugTools::ResourceManager::instance().set<GL::AbstractShaderProgram>(_shader.key(), new Shaders::Flat3D);

  _mesh = DebugTools::ResourceManager::instance().get<GL::Mesh>("my-line");
  if(_mesh) return;

  GL::Mesh* mesh = new GL::Mesh;

  GL::Buffer vertexBuffer{GL::Buffer::TargetHint::Array};
  vertexBuffer.setData(positions, GL::BufferUsage::StaticDraw);

  mesh->setPrimitive(GL::MeshPrimitive::Lines)
      .setCount(2)
      .addVertexBuffer(std::move(vertexBuffer), 0, Shaders::Flat3D::Position{});

  DebugTools::ResourceManager::instance().set<GL::Mesh>(_mesh.key(), mesh, ResourceDataState::Final, ResourcePolicy::Manual);
}

void LineRenderer::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
  for (auto &line : _lines) {
    _shader->setTransformationProjectionMatrix(camera.projectionMatrix() * transformationMatrix * line->transformation())
      .setColor(line->color());

    _mesh->draw(*_shader);
  }
}

Matrix4 LineRendererOptions::transformation() {
  if (_dirty) {
    Float len = (_to - _from).length();
    _transformation = Matrix4::lookAt(_from, _to, Vector3::yAxis(1)) * Matrix4::scaling(Vector3(len));
    _dirty = false;
  }

  return _transformation;
}
