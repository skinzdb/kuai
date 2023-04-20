#include "pybind11/pybind11.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"

#include "kuai.h"

#include "glm/glm.hpp"

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

PYBIND11_MODULE(pykuai, m)
{
    Log::Init();

    m.doc() = "3D Game Engine";

    py::class_<App>(m, "App")
        .def(py::init<>())
        .def("pushLayer", &App::pushLayer, "Pushes a layer on to the app's layer stack.")
        .def("run", &App::run, "Starts the application main loop. Layers on the layer stack get updated every iteration of the loop.")
        .def_static("get", &App::get, py::return_value_policy::reference)
        .def("getWindow", &App::getWindow, py::return_value_policy::reference);

    py::class_<Window>(m, "Window")
        .def("getWidth", &Window::getWidth)
        .def("getHeight", &Window::getHeight);
    
    py::class_<Layer, PyLayer>(m, "Layer")
        .def(py::init<>())
        .def("onAttach", &Layer::onAttach)
        .def("onDetach", &Layer::onDetach)
        .def("update", &Layer::update)
        .def("onEvent", &Layer::onEvent, py::return_value_policy::reference);

    py::enum_<EventType>(m, "EventType")
        .value("KEY_RELEASE", EventType::KeyPress)
        .value("KEY_PRESS", EventType::KeyRelease)
        .value("MOUSE_MOVE", EventType::MouseMove)
        .value("MOUSE_SCROLL", EventType::MouseScroll)
        .value("MOUSE_BTN_PRESS", EventType::MouseBtnPress)
        .value("MOUSE_BTN_RELEASE", EventType::MouseBtnRelease)
        .export_values();

    py::class_<Event>(m, "Event")
        .def("getType", &Event::getEventType)
        .def("toString", &Event::toString);

    py::class_<KeyPressEvent>(m, "KeyPressEvent")
        .def("getType", &KeyPressEvent::getEventType)
        .def("toString", &KeyPressEvent::toString)
        .def("getKey", &KeyPressEvent::getKeyCode)
        .def("getRepeats", &KeyPressEvent::getRepeatCount);

    py::class_<KeyReleaseEvent>(m, "KeyReleaseEvent")
        .def("getType", &KeyReleaseEvent::getEventType)
        .def("toString", &KeyReleaseEvent::toString)
        .def("getKey", &KeyReleaseEvent::getKeyCode);

    py::class_<MouseMoveEvent>(m, "MouseMoveEvent")
        .def("getType", &MouseMoveEvent::getEventType)
        .def("toString", &MouseMoveEvent::toString)
        .def("getX", &MouseMoveEvent::getX)
        .def("getY", &MouseMoveEvent::getY);

    py::class_<MouseScrollEvent>(m, "MouseScrollEvent")
        .def("getType", &MouseScrollEvent::getEventType)
        .def("toString", &MouseScrollEvent::toString)
        .def("getXOff", &MouseScrollEvent::getXOffset)
        .def("getYOff", &MouseScrollEvent::getYOffset);

    py::class_<Input>(m, "Input")
        .def_static("isKeyPressed", &Input::isKeyPressed)
        .def_static("isMouseBtnPressed", &Input::isMouseBtnPressed)
        .def_static("getMousePos", &Input::getMousePos)
        .def_static("getMouseX", &Input::getMouseX)
        .def_static("getMouseY", &Input::getMouseY);

    py::enum_<Mouse::Mouse>(m, "Mouse")
        .value("BTN0", Mouse::Button0)
        .value("BTN1", Mouse::Button1)
        .value("BTN2", Mouse::Button2)
        .export_values();

    py::enum_<Key::Key>(m, "Key")
        .value("SPACE", Key::Space)
        .value("A", Key::A)
        .value("B", Key::B)
        .value("C", Key::C)
        .value("D", Key::D)
        .value("E", Key::E)
        .value("F", Key::F)
        .value("G", Key::G)
        .value("H", Key::H)
        .value("I", Key::I)
        .value("J", Key::J)
        .value("K", Key::K)
        .value("L", Key::L)
        .value("M", Key::M)
        .value("N", Key::N)
        .value("O", Key::O)
        .value("P", Key::P)
        .value("Q", Key::Q)
        .value("R", Key::R)
        .value("S", Key::S)
        .value("T", Key::T)
        .value("U", Key::U)
        .value("V", Key::V)
        .value("W", Key::W)
        .value("X", Key::X)
        .value("Y", Key::Y)
        .value("Z", Key::Z)
        .value("ESC", Key::Escape)
        .value("ENTER", Key::Enter)
        .value("TAB", Key::Tab)
        .value("RIGHT", Key::Right)
        .value("LEFT", Key::Left)
        .value("DOWN", Key::Down)
        .value("UP", Key::Up)
        .value("F1", Key::F1)
        .value("F2", Key::F2)
        .value("F3", Key::F3)
        .value("F4", Key::F4)
        .value("F5", Key::F5)
        .value("F6", Key::F6)
        .value("F7", Key::F7)
        .value("F8", Key::F8)
        .value("F9", Key::F9)
        .value("F10", Key::F10)
        .value("F11", Key::F11)
        .value("F12", Key::F12)
        .value("L_SHIFT", Key::LeftShift)
        .value("L_CTRL", Key::LeftControl)
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
        //.def(py::init<Entity*>())
        .def("getForward", &Transform::getForward, py::return_value_policy::copy)
        .def("getRight", &Transform::getRight, py::return_value_policy::copy)
        .def("getUp", &Transform::getUp, py::return_value_policy::copy)
        .def("getPos", &Transform::getPos, py::return_value_policy::copy)
        .def("setPos", py::overload_cast<float, float, float>(&Transform::setPos))
        .def("getRot", &Transform::getRot, py::return_value_policy::copy)
        .def("setRot", py::overload_cast<float, float, float>(&Transform::setRot))
        .def("getScale", &Transform::getScale, py::return_value_policy::copy)
        .def("setScale", py::overload_cast<float, float, float>(&Transform::setScale))
        .def("translate", py::overload_cast<float, float, float>(&Transform::translate))
        .def("translate", py::overload_cast<const glm::vec3&>(&Transform::translate))
        .def("rotate", py::overload_cast<float, float, float>(&Transform::rotate))
        .def("rotate", py::overload_cast<const glm::vec3&>(&Transform::rotate));

    py::enum_<Light::LightType>(m, "LightType")
        .value("DIR", Light::LightType::Directional)
        .value("POINT", Light::LightType::Point)
        .value("SPOT", Light::LightType::Spot)
        .export_values();

    py::class_<Light>(m, "Light")
        //.def(py::init<Entity*>())
        //.def(py::init<Entity*, float>())
        .def("getType", &Light::getType, py::return_value_policy::copy)
        .def("getCol", &Light::getCol, py::return_value_policy::copy)
        .def("getIntensity", &Light::getIntensity, py::return_value_policy::copy)
        .def("getLinear", &Light::getLinear, py::return_value_policy::copy)
        .def("getQuadratic", &Light::getQuadratic, py::return_value_policy::copy)
        .def("setType", &Light::setType)
        .def("setCol", py::overload_cast<const glm::vec3&>(&Light::setCol))
        .def("setCol", py::overload_cast<float, float, float>(&Light::setCol))
        .def("setIntensity", &Light::setIntensity)
        .def("setAttenuation", &Light::setAttenuation);

    py::class_<CameraComponent>(m, "Camera")
        //.def(py::init<Entity*>())
        //.def(py::init<float, float, float, float, float>())
        .def("setPersp", &CameraComponent::setPerspective)
        .def("setOrtho", &CameraComponent::setOrtho)
        .def("setAspect", &CameraComponent::setAspect)
        .def("getTransform", &CameraComponent::getTransform, py::return_value_policy::reference);

    py::class_<AudioClip, std::shared_ptr<AudioClip>>(m, "AudioClip")
        .def(py::init<const std::string&>());

    py::class_<AudioListener>(m, "Listener")
        .def("getGain", &AudioListener::getGain, py::return_value_policy::copy)
        .def("setGain", &AudioListener::setGain);

    py::class_<AudioSourceComponent>(m, "AudioSource")
        .def_readonly("source", &AudioSourceComponent::source, py::return_value_policy::reference);

    py::class_<AudioSource>(m, "sauce")
        .def("getPitch", &AudioSource::getPitch, py::return_value_policy::copy)
        .def("setPitch", &AudioSource::setPitch)
        .def("getGain", &AudioSource::getGain, py::return_value_policy::copy)
        .def("setGain", &AudioSource::setGain)
        .def("getRolloff", &AudioSource::getRolloff, py::return_value_policy::copy)
        .def("setRolloff", &AudioSource::setRolloff)
        .def("getRefDist", &AudioSource::getRefDist, py::return_value_policy::copy)
        .def("setRefDist", &AudioSource::setRefDist)
        .def("isLoop", &AudioSource::isLoop, py::return_value_policy::copy)
        .def("setLoop", &AudioSource::setLoop)
        .def("setClip", &AudioSource::setAudioClip)
        .def("play", &AudioSource::play)
        .def("stop", &AudioSource::stop);

    py::class_<Cubemap, std::shared_ptr<Cubemap>>(m, "Cubemap")
        .def(py::init<const std::vector<std::string>&>());

    py::class_<Texture, std::shared_ptr<Texture>>(m, "Texture")
        .def(py::init<>())
        .def(py::init<const char*>());

    py::class_<MeshRenderer>(m, "Renderer")
        //.def(py::init<std::shared_ptr<Model>>())
        .def("setModel", &MeshRenderer::setModel);

    py::class_<Mesh, std::shared_ptr<Mesh>>(m, "Mesh")
        .def(py::init<const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::vector<uint32_t>&>())
        .def("getMat", &Mesh::getMaterial, py::return_value_policy::reference)
        .def("setMat", &Mesh::setMaterial)
        .def("setTexCoords", &Mesh::setTexCoords);

    py::class_<Skybox, std::shared_ptr<Skybox>, Mesh>(m, "Skybox")
        .def(py::init<std::shared_ptr<Cubemap>>());

    py::class_<Material, std::shared_ptr<Material>>(m, "Material");

    py::class_<DefaultMaterial, std::shared_ptr<DefaultMaterial>, Material>(m, "DefaultMaterial")
        .def(py::init<std::shared_ptr<Texture>, std::shared_ptr<Texture>, float>())
        .def("setDiffuse", &DefaultMaterial::setDiffuse)
        .def("setSpecular", &DefaultMaterial::setSpecular)
        .def("setReflection", &DefaultMaterial::setReflection)
        .def_readwrite("shininess", &DefaultMaterial::specularAmount);

    py::class_<Model, std::shared_ptr<Model>>(m, "Model")
        .def(py::init<const std::string&>())
        .def(py::init<std::shared_ptr<Mesh>>())
        .def("getMeshes", &Model::getMeshes, py::return_value_policy::reference);

    py::class_<Entity, std::shared_ptr<Entity>>(m, "Entity")
        .def("getTransform", &Entity::getTransform, py::return_value_policy::reference)
        .def("getCam", &Entity::getComponent<CameraComponent>, py::return_value_policy::reference)
        .def("getLight", &Entity::getComponent<Light>, py::return_value_policy::reference)
        .def("getRenderer", &Entity::getComponent<MeshRenderer>, py::return_value_policy::reference)
        .def("getListener", &Entity::getComponent<AudioListener>, py::return_value_policy::reference)
        .def("getAudioSource", &Entity::getComponent<AudioSourceComponent>, py::return_value_policy::reference)
        .def("addCam", &Entity::addComponent<CameraComponent>, py::return_value_policy::reference)
        .def("addLight", &Entity::addComponent<Light>, py::return_value_policy::reference)
        .def("addRenderer", &Entity::addComponent<MeshRenderer>, py::return_value_policy::reference)
        .def("addAudioSource", &Entity::addComponent<AudioSourceComponent>, py::return_value_policy::reference);
}