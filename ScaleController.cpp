#include "ScaleController.h"

#include <Magnum/Math/Intersection.h>
#include <Magnum/SceneGraph/Camera.h>
#include <Magnum/SceneGraph/Drawable.h>
#include <Magnum/Shaders/VertexColor.h>
#include <Magnum/Primitives/Axis.h>
#include <Magnum/DebugTools/ResourceManager.h>
#include <Magnum/GL/Mesh.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/MeshTools/Interleave.h>
#include <Magnum/MeshTools/CompressIndices.h>

using namespace Magnum;

class ScaleObjectRenderer : SceneGraph::Drawable3D {
  public:
    ScaleObjectRenderer(Object3D &parent, SceneGraph::DrawableGroup3D *group, std::function<bool(void)> drawCheck) : SceneGraph::Drawable3D{parent, group}, _drawCheck(drawCheck) {
      /* Shader */
      _shader = DebugTools::ResourceManager::instance().get<GL::AbstractShaderProgram, Shaders::VertexColor3D>("ScaleController-shader");
      if(!_shader) DebugTools::ResourceManager::instance().set<GL::AbstractShaderProgram>(_shader.key(), new Shaders::VertexColor3D);

      /* Mesh and vertex buffer */
      _mesh = DebugTools::ResourceManager::instance().get<GL::Mesh>("ScaleController-mesh");

      if(_mesh) return;

      /* Create the mesh */
      GL::Buffer vertexBuffer{GL::Buffer::TargetHint::Array};
      GL::Buffer indexBuffer{GL::Buffer::TargetHint::ElementArray};
      GL::Mesh* mesh = new GL::Mesh;

      auto data = Primitives::axis3D();

      vertexBuffer.setData(MeshTools::interleave(data.positions(0), data.colors(0)), GL::BufferUsage::StaticDraw);
      indexBuffer.setData(MeshTools::compressIndicesAs<UnsignedByte>(data.indices()), GL::BufferUsage::StaticDraw);

      mesh->setPrimitive(GL::MeshPrimitive::Lines)
        .setCount(data.indices().size())
        .addVertexBuffer(std::move(vertexBuffer), 0, Shaders::VertexColor3D::Position(), Shaders::VertexColor3D::Color4{})
        .setIndexBuffer(std::move(indexBuffer), 0, GL::MeshIndexType::UnsignedByte, 0, data.positions(0).size());

      DebugTools::ResourceManager::instance().set<GL::Mesh>(_mesh.key(), mesh, ResourceDataState::Final, ResourcePolicy::Manual);
    }

  private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override {
      if (_drawCheck()) {
        Matrix4 m = transformationMatrix * Matrix4::scaling(transformationMatrix.scaling()).inverted();
        _shader->setTransformationProjectionMatrix(camera.projectionMatrix() * m);
        _mesh->draw(*_shader);
      }
    }

    Resource<GL::AbstractShaderProgram, Shaders::VertexColor3D> _shader;
    Resource<GL::Mesh> _mesh;
    std::function<bool(void)> _drawCheck;
};

ScaleController::ScaleController(Object3D *parent, SceneGraph::DrawableGroup3D *group) : Object3D(parent)  {
  new ScaleObjectRenderer{*this, group, [this](){
    return children().first();
  }};
}

void ScaleController::move(const Ray &cameraRay) {
  if (_missed) {
    return;
  }

  Float t = Math::Intersection::planeLine(_plane, cameraRay.origin, cameraRay.direction);

  if (Magnum::Math::isInf(t) || Magnum::Math::isNan(t)) {
    return;
  }

  Vector3 currentPoint = cameraRay.origin + cameraRay.direction * t;

  setScaling(_startScaling + _dir * (currentPoint - _startPoint));
}

void ScaleController::grab(const Ray &cameraRay) {
  if (!children().first()) {
    return;
  }

  _startScaling = scaling();
  _missed = true;

  // check x-z
  _plane = Math::planeEquation(Vector3::yAxis(1), translation());
  Float t = Math::Intersection::planeLine(_plane, cameraRay.origin, cameraRay.direction);

  if (Magnum::Math::isInf(t) || Magnum::Math::isNan(t)) {
    return;
  }

  _startPoint = cameraRay.origin + cameraRay.direction * t;
  Vector3 p = _startPoint - translation();

  if (Math::abs(p.z()) < _axisThreshold && p.x() > 0 && p.x() <= 1) {
    _dir = Vector3::xAxis(1);
    _missed = false;
    return;
  }

  if (Math::abs(p.x()) < _axisThreshold && p.z() > 0 && p.z() <= 1) {
    _dir = Vector3::zAxis(1);
    _missed = false;
    return;
  }

  // check y
  _plane = Math::planeEquation(Vector3::xAxis(1), translation());
  t = Math::Intersection::planeLine(_plane, cameraRay.origin, cameraRay.direction);

  if (Magnum::Math::isInf(t) || Magnum::Math::isNan(t)) {
    return;
  }

  _startPoint = cameraRay.origin + cameraRay.direction * t;
  p = _startPoint - translation();

  if (Math::abs(p.z()) < _axisThreshold && p.y() > 0 && p.y() <= 1) {
    _dir = Vector3::yAxis(1);
    _missed = false;
    return;
  }
}

void ScaleController::release() {
  _missed = true;
}
