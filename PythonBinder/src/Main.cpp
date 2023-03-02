#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <Smongine.h>

using namespace Smong;
namespace py = pybind11;

class PyLayer : public Layer
{
public:
    using Layer::Layer;

    //virtual void OnAttach() override
    //{
    //    PYBIND11_OVERRIDE_PURE(
    //        void,
    //        Layer,
    //        OnAttach
    //    );
    //}

    //virtual void OnDetach() override
    //{
    //    PYBIND11_OVERRIDE_PURE(
    //        void,
    //        Layer,
    //        OnDetach
    //    );
    //}

    virtual void Update(float dt) override
    {
        PYBIND11_OVERRIDE_PURE(
            void,
            Layer,
            Update,
            dt
        );
    }

    virtual void OnEvent(Event* event) override
    {
        PYBIND11_OVERRIDE_PURE(
            void,
            Layer,
            OnEvent,
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

PYBIND11_MODULE(pysmong, m)
{
    Log::Init();
    SM_CORE_WARN("Initialised Core Log");
    SM_INFO("Initialised Client Log");

    m.doc() = "Game Engine";

    py::class_<App>(m, "App")
        .def(py::init<>())
        .def("PushLayer", &App::PushLayer)
        .def("Run", &App::Run);

    py::class_<Layer, PyLayer>(m, "Layer")
        .def(py::init<>())
        .def("OnAttach", &Layer::OnAttach)
        .def("OnDetach", &Layer::OnDetach)
        .def("Update", &Layer::Update)
        .def("OnEvent", &Layer::OnEvent, py::return_value_policy::reference);

    py::class_<Event>(m, "Event")
        .def("ToString", &Event::ToString);

    py::class_<Input>(m, "Input")
        .def_static("IsKeyPressed", &Input::IsKeyPressed)
        .def_static("IsMouseBtnPressed", &Input::IsMouseBtnPressed)
        .def_static("GetMousePos", &Input::GetMousePos)
        .def_static("GetMouseX", &Input::GetMouseX)
        .def_static("GetMouseY", &Input::GetMouseY);

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
        .def("Update", &Scene::Update)
        .def("CreateEntity", &Scene::CreateEntity, py::return_value_policy::reference)
        .def("GetMainCam", &Scene::GetMainCam, py::return_value_policy::reference)
        .def("GetMainCamTransform", &Scene::GetMainCamTransform, py::return_value_policy::reference);

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
        .def_readwrite("pos", &Transform::pos)
        .def_readwrite("rot", &Transform::rot)
        .def_readwrite("scale", &Transform::scale)
        .def("GetForward", &Transform::GetForward, py::return_value_policy::copy)
        .def("GetRight", &Transform::GetRight, py::return_value_policy::copy)
        .def("GetUp", &Transform::GetUp, py::return_value_policy::copy);

    py::enum_<Light::LightType>(m, "LightType")
        .value("Directional", Light::LightType::Directional)
        .value("Point", Light::LightType::Point)
        .value("Spot", Light::LightType::Spot)
        .export_values();

    py::class_<Light>(m, "Light")
        .def(py::init<float>())
        .def(py::init<float, float, float>())
        .def_readwrite("type", &Light::type)
        .def_readwrite("col", &Light::col)
        .def_readwrite("intensity", &Light::intensity)
        .def_readwrite("linear", &Light::linear)
        .def_readwrite("quadratic", &Light::quadratic);

    py::class_<Camera>(m, "Camera")
        .def(py::init<float, float, float, float, float>())
        .def("SetPerspective", &Camera::SetPerspective)
        .def("SetOrtho", &Camera::SetOrtho)
        .def("SetAspect", &Camera::SetAspect);

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
        .def("GetTransform", &Entity::GetTransform, py::return_value_policy::reference)
        .def("GetCam", &Entity::GetComponent<Camera>, py::return_value_policy::reference)
        .def("GetMeshMaterial", &Entity::GetComponent<MeshMaterial>, py::return_value_policy::reference)
        .def("GetLight", &Entity::GetComponent<Light>, py::return_value_policy::reference)
        .def("AddCam", &Entity::AddComponent<Camera>)
        .def("AddMeshMaterial", &Entity::AddComponent<MeshMaterial>)
        .def("AddLight", &Entity::AddComponent<Light>);

    py::class_<ModelLoader>(m, "ModelLoader")
        .def_static("LoadObj", &ModelLoader::LoadObj, py::return_value_policy::reference);
}