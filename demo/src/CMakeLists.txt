find_package(Magnum REQUIRED
  GL
  Primitives
  Shaders
  SceneGraph
  MeshTools
  DebugTools
  Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)
include_directories(../..)

add_executable(MyApplication
  ../../MathUtils.cpp
  ../../TranslateController.cpp
  ../../GridRenderer.cpp
  ../../ThirdPersonCameraController.cpp
  MyApplication.cpp)

target_link_libraries(MyApplication PRIVATE
  Magnum::Application
  Magnum::GL
  Magnum::Magnum
  Magnum::Primitives
  Magnum::Shaders
  Magnum::DebugTools
  Magnum::MeshTools
  Magnum::SceneGraph)
