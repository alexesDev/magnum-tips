#include "GridRenderer.h"

#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/DebugTools/ResourceManager.h>

using namespace Magnum;

Color3 getLineColor(Float i, const Color3 &baseColor, const Color3 &axesColor, const Color3 &subColor);

Color3 getLineColor(Float i, const Color3 &baseColor, const Color3 &axesColor, const Color3 &subColor) {
  if (i == 0) {
    return axesColor;
  }

  if (std::fmod(i, 5) == 0) {
    return subColor;
  }

  return baseColor;
}

GridRenderer::GridRenderer(Object3D& object, Magnum::SceneGraph::DrawableGroup3D* drawables) : SceneGraph::Drawable3D{object, drawables} {
  _shader = DebugTools::ResourceManager::instance().get<GL::AbstractShaderProgram, Shaders::VertexColor3D>("VertexColorShader3D");
  if(!_shader) DebugTools::ResourceManager::instance().set<GL::AbstractShaderProgram>(_shader.key(), new Shaders::VertexColor3D);

  _mesh = DebugTools::ResourceManager::instance().get<GL::Mesh>("grid");
  if(_mesh) return;

  GL::Buffer vertexBuffer{GL::Buffer::TargetHint::Array};
  GL::Mesh* mesh = new GL::Mesh;

  struct Vertex {
    Vector3 position;
    Color3 color;
  };

  std::vector<Vertex> data;

  Float size = 10;
  Float brightness = 0.3f;

  Color3 baseColor(brightness);
  Color3 subColor(brightness * 1.5f);
  Color3 xColor{ brightness * 2.f, brightness, brightness };
  Color3 zColor{ brightness, brightness, brightness * 2.f };

  for (Float i = -size; i <= size; ++i) {
    data.push_back({ { -size, 0, i }, getLineColor(i, baseColor, xColor, subColor) });
    data.push_back({ { size, 0, i }, getLineColor(i, baseColor, xColor, subColor) });

    data.push_back({ { i, 0, -size }, getLineColor(i, baseColor, zColor, subColor) });
    data.push_back({ { i, 0, size }, getLineColor(i, baseColor, zColor, subColor) });
  }

  vertexBuffer.setData(data, GL::BufferUsage::StaticDraw);

  mesh->setPrimitive(GL::MeshPrimitive::Lines)
      .setCount(data.size())
      .addVertexBuffer(std::move(vertexBuffer), 0, Shaders::VertexColor3D::Position{}, Shaders::VertexColor3D::Color3{});

  DebugTools::ResourceManager::instance().set<GL::Mesh>(_mesh.key(), mesh, ResourceDataState::Final, ResourcePolicy::Manual);
}

void GridRenderer::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
  _shader->setTransformationProjectionMatrix(camera.projectionMatrix() * transformationMatrix);
  _mesh->draw(*_shader);
}
