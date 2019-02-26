#include "TranslateController.h"

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

class ObjectRenderer : SceneGraph::Drawable3D {
  public:
    ObjectRenderer(Object3D &parent, SceneGraph::DrawableGroup3D *group, std::function<bool(void)> drawCheck) : SceneGraph::Drawable3D{parent, group}, _drawCheck(drawCheck) {
      /* Shader */
      _shader = DebugTools::ResourceManager::instance().get<GL::AbstractShaderProgram, Shaders::VertexColor3D>("TranslateController-shader");
      if(!_shader) DebugTools::ResourceManager::instance().set<GL::AbstractShaderProgram>(_shader.key(), new Shaders::VertexColor3D);

      /* Mesh and vertex buffer */
      _mesh = DebugTools::ResourceManager::instance().get<GL::Mesh>("TranslateController-mesh");

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
        _shader->setTransformationProjectionMatrix(camera.projectionMatrix() * transformationMatrix);
        _mesh->draw(*_shader);
      }
    }

    Resource<GL::AbstractShaderProgram, Shaders::VertexColor3D> _shader;
    Resource<GL::Mesh> _mesh;
    std::function<bool(void)> _drawCheck;
};

TranslateController::TranslateController(Object3D *parent, SceneGraph::DrawableGroup3D *group) : Object3D(parent)  {
  new ObjectRenderer{*this, group, [this](){
    return children().first();
  }};
}

void TranslateController::move(const Ray &cameraRay) {
  if (_missed) {
    return;
  }

  Float t = Math::Intersection::planeLine(_plane, cameraRay.origin, cameraRay.direction);

  if (Magnum::Math::isInf(t) || Magnum::Math::isNan(t)) {
    return;
  }

  Vector3 currentPoint = cameraRay.origin + cameraRay.direction * t;

  Matrix4 tr = transformation();
  tr.translation() = _startPosition + _dir * (currentPoint - _startPoint);
  setTransformation(tr);
}

void TranslateController::grab(const Ray &cameraRay) {
  if (!children().first()) {
    return;
  }

  _startPosition = transformation().translation();
  _missed = true;

  // check x-z
  _plane = Math::planeEquation(Vector3::yAxis(1), _startPosition);
  Float t = Math::Intersection::planeLine(_plane, cameraRay.origin, cameraRay.direction);

  if (Magnum::Math::isInf(t) || Magnum::Math::isNan(t)) {
    return;
  }

  _startPoint = cameraRay.origin + cameraRay.direction * t;
  Vector3 p = _startPoint - _startPosition;

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
  _plane = Math::planeEquation(Vector3::xAxis(1), _startPosition);
  t = Math::Intersection::planeLine(_plane, cameraRay.origin, cameraRay.direction);

  if (Magnum::Math::isInf(t) || Magnum::Math::isNan(t)) {
    return;
  }

  _startPoint = cameraRay.origin + cameraRay.direction * t;
  p = _startPoint - _startPosition;

  if (Math::abs(p.z()) < _axisThreshold && p.y() > 0 && p.y() <= 1) {
    _dir = Vector3::yAxis(1);
    _missed = false;
    return;
  }
}

void TranslateController::release() {
  _missed = true;
}
