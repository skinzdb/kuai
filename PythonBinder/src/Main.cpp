#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <kuai.h>

using namespace kuai;
namespace py = pybind11;

class PyLayer : public Layer
{
public:
    using Layer::Layer;

    //virtual void onAttach() override
    //{
    //    PYBIND11_OVERRIDE_PURE(
    //        void,
    //        Layer,
    //        onAttach
    //    );
    //}

    //virtual void onDetach() override
    //{
    //    PYBIND11_OVERRIDE_PURE(
    //        void,
    //        Layer,
    //        onDetach
    //    );
    //}

    virtual void update(float dt) override
    {
        PYBIND11_OVERRIDE_PURE(
            void,
            Layer,
            update,
            dt
        );
    }

    virtual void onEvent(Event* event) override
    {
        PYBIND11_OVERRIDE_PURE(
            void,
            Layer,
            onEvent,
            event
        );
    }
};

//class PyEntity : public Entity
//{
//public:
//    void AddMeshMaterial(Mesh* mesh, Material* material)
//    {
//        addComponent<MeshMaterial>(id, mesh, material);
//    }
//};
//
//class PyScene : public Scene
//{
//public:
//    std::shared_ptr<PyEntity> CreateEntity()
//    {
//        return std::make_shared<PyEntity>((PyEntity*)Scene::CreateEntity().get());
//    }
//};

PYBIND11_MODULE(pykuai, m)
{
    Log::Init();
    KU_CORE_WARN("Initialised Core Log");
    KU_INFO("Initialised Client Log");

    m.doc() = "Game Engine";

    py::class_<App>(m, "App")
        .def(py::init<>())
        .def("pushLayer", &App::pushLayer)
        .def("run", &App::run);

    py::class_<Layer, PyLayer>(m, "Layer")
        .def(py::init<>())
        .def("onAttach", &Layer::onAttach)
        .def("onDetach", &Layer::onDetach)
        .def("update", &Layer::update)
        .def("onEvent", &Layer::onEvent, py::return_value_policy::reference);

    py::class_<Event>(m, "Event")
        .def("ToString", &Event::toString);

    py::class_<Input>(m, "Input")
        .def_static("isKeyPressed", &Input::isKeyPressed)
        .def_static("isMouseBtnPressed", &Input::isMouseBtnPressed)
        .def_static("getMousePos", &Input::getMousePos)
        .def_static("getMouseX", &Input::getMouseX)
        .def_static("getMouseY", &Input::getMouseY);

    py::enum_<Mouse::Mouse>(m, "Mouse")
        .value("Button0", Mouse::Button0)
        .value("Button1", Mouse::Button1)
        .value("Button2", Mouse::Button2)
        .export_values();

    py::enum_<Key::Key>(m, "Key")
        .value("W", Key::W)
        .value("A", Key::A)
        .value("S", Key::S)
        .value("D", Key::D)
        .value("Space", Key::Space)
        .value("LeftShift", Key::LeftShift)
        .value("LeftControl", Key::LeftControl)
        .export_values();

    py::class_<Scene>(m, "Scene")
        .def(py::init<>())
        .def("update", &Scene::update)
        .def("createEntity", &Scene::createEntity, py::return_value_policy::reference)
        .def("getMainCam", &Scene::getMainCam, py::return_value_policy::reference);

    py::class_<glm::vec3>(m, "Vec3")
        .def(py::init<float, float, float>())
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(float() * py::self)
        .def(py::self * float())
        .def(py::self / float())
        .def(py::self * py::self)
        .def(-py::self)
        .def_readwrite("x", &glm::vec3::x)
        .def_readwrite("y", &glm::vec3::y)
        .def_readwrite("z", &glm::vec3::z);

    py::class_<Transform>(m, "Transform")
        .def(py::init<>())
        .def("getForward", &Transform::getForward, py::return_value_policy::copy)
        .def("getRight", &Transform::getRight, py::return_value_policy::copy)
        .def("getUp", &Transform::getUp, py::return_value_policy::copy);

    py::enum_<Light::LightType>(m, "LightType")
        .value("Directional", Light::LightType::Directional)
        .value("Point", Light::LightType::Point)
        .value("Spot", Light::LightType::Spot)
        .export_values();

    //py::class_<Light>(m, "Light")
    //    .def(py::init<float>())
    //    .def(py::init<float, float, float>())
    //    .def_readwrite("type", &Light::type)
    //    .def_readwrite("col", &Light::col)
    //    .def_readwrite("intensity", &Light::intensity)
    //    .def_readwrite("linear", &Light::linear)
    //    .def_readwrite("quadratic", &Light::quadratic);

    //py::class_<Camera>(m, "Camera")
    //    .def(py::init<float, float, float, float, float>())
    //    .def("SetPerspective", &Camera::SetPerspective)
    //    .def("SetOrtho", &Camera::SetOrtho)
    //    .def("SetAspect", &Camera::SetAspect);

    py::class_<Texture, std::shared_ptr<Texture>>(m, "Texture")
        .def(py::init<>())
        .def(py::init<const char*>());

    py::class_<Mesh>(m, "Mesh")
        .def(py::init<const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::vector<uint32_t>&>());

    py::class_<Material>(m, "Material");

    py::class_<PhongMaterial, Material>(m, "PhongMaterial")
        .def(py::init<std::shared_ptr<Texture>, std::shared_ptr<Texture>, float>());

    py::class_<MeshMaterial>(m, "MeshMaterial")
        .def(py::init<Mesh*, Material*>())
        .def_readwrite("mesh", &MeshMaterial::mesh)
        .def_readwrite("material", &MeshMaterial::material);

    py::class_<Entity, std::shared_ptr<Entity>>(m, "Entity")
        .def("getTransform", &Entity::getTransform, py::return_value_policy::reference)
        .def("getCam", &Entity::getComponent<Camera>, py::return_value_policy::reference)
        .def("getMeshMaterial", &Entity::getComponent<MeshMaterial>, py::return_value_policy::reference)
        .def("getLight", &Entity::getComponent<Light>, py::return_value_policy::reference)
        .def("addCam", &Entity::addComponent<Camera>)
        .def("addMeshMaterial", &Entity::addComponent<MeshMaterial>)
        .def("addLight", &Entity::addComponent<Light>);

    py::class_<ModelLoader>(m, "ModelLoader")
        .def_static("loadObj", &ModelLoader::LoadObj, py::return_value_policy::reference);
}