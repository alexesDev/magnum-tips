#ifndef CubeDrawable_h
#define CubeDrawable_h

#include <Magnum/GL/Mesh.h>
#include <Magnum/Primitives/Cube.h>
#include <Magnum/Shaders/Phong.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Trade/MeshData3D.h>
#include <Types.h>

using namespace Magnum;

class CubeDrawable: public SceneGraph::Drawable3D {
  public:
    explicit CubeDrawable(Object3D& object, SceneGraph::DrawableGroup3D* group): SceneGraph::Drawable3D{object, group}
    {
      _mesh = MeshTools::compile(Primitives::cubeSolid());
    }

  private:
    void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override {
      using namespace Math::Literals;

      _shader.setDiffuseColor(0xa5c9ea_rgbf)
        .setLightPosition(camera.cameraMatrix().transformPoint(
              {5.0f, 5.0f, 7.0f}))
      .setTransformationMatrix(transformationMatrix)
        .setNormalMatrix(transformationMatrix.rotationScaling())
        .setProjectionMatrix(camera.projectionMatrix());
      _mesh.draw(_shader);
    }

    GL::Mesh _mesh;
    Shaders::Phong _shader;
};

#endif
