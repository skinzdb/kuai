#include "pybind11/pybind11.h"
#include "pybind11/operators.h"
#include "pybind11/stl.h"

#include "kuai.h"

#include "glm/glm.hpp"

using namespace kuai;
namespace py = pybind11;
using namespace py::literals;

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
    py::options options;
    options.disable_enum_members_docstring();
    //options.disable_function_signatures();

    py::class_<App>(m, "App",
        R"(This class runs your game. It handles windowing, events and layer updates. 
           It uses layers to execute game logic.
        )")
        .def(py::init<>())
        .def("pushLayer", &App::pushLayer, "Pushes a layer on to the app's layer stack.")
        .def("run", &App::run, "Starts the application main loop. Layers on the layer stack get updated every iteration of the loop.")
        .def_static("get", &App::get, py::return_value_policy::reference, "Returns the app instance.")
        .def("getWindow", &App::getWindow, py::return_value_policy::reference, "Returns the window instance.");

    py::class_<Window>(m, "Window")
        .def("getWidth", &Window::getWidth)
        .def("getHeight", &Window::getHeight);
    
    py::class_<Layer, PyLayer>(m, "Layer", 
        R"(A layer describes a self - contained section of game logic or rendering. The App class uses layers in order to run a game.
           The user must create their own layers by inheriting from this class.
           Any necessary initialisation can be handled in the constructor.
           Code within the update() function will be called every frame.)
        )")
        .def(py::init<>())
        .def("onAttach", &Layer::onAttach, "Called when the layer is attached to the app via App::pushLayer()")
        .def("onDetach", &Layer::onDetach, "Called when the layer is removed from the app.")
        .def("update", &Layer::update, " Called every frame. @param dt Delta time : time elapsed between frames.")
        .def("onEvent", &Layer::onEvent, py::return_value_policy::reference, "Called when an event reaches this layer.");

    py::enum_<EventType>(m, "EventType")
        .value("KEY_RELEASE", EventType::KeyPress)
        .value("KEY_PRESS", EventType::KeyRelease)
        .value("MOUSE_MOVE", EventType::MouseMove)
        .value("MOUSE_SCROLL", EventType::MouseScroll)
        .value("MOUSE_BTN_PRESS", EventType::MouseBtnPress)
        .value("MOUSE_BTN_RELEASE", EventType::MouseBtnRelease)
        .export_values();

    py::class_<Event>(m, "Event", "Base class for events.")
        .def("getType", &Event::getEventType)
        .def("toString", &Event::toString);

    py::class_<KeyPressEvent>(m, "KeyPressEvent", "Keyboard event when key is pressed down.")
        .def("getType", &KeyPressEvent::getEventType)
        .def("toString", &KeyPressEvent::toString)
        .def("getKey", &KeyPressEvent::getKeyCode)
        .def("getRepeats", &KeyPressEvent::getRepeatCount);

    py::class_<KeyReleaseEvent>(m, "KeyReleaseEvent", "Keyboard event when pressed key is released.")
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

    py::class_<Input>(m, "Input", "Used to poll mouse and keyboard input, as well as getting input values from the mouse.")
        .def_static("isKeyPressed", &Input::isKeyPressed)
        .def_static("isMouseBtnPressed", &Input::isMouseBtnPressed)
        .def_static("getMousePos", &Input::getMousePos)
        .def_static("getMouseX", &Input::getMouseX)
        .def_static("getMouseY", &Input::getMouseY);

    py::enum_<Mouse::Mouse>(m, "Mouse", "Mouse button codes.")
        .value("BTN0", Mouse::Button0)
        .value("BTN1", Mouse::Button1)
        .value("BTN2", Mouse::Button2)
        .export_values();

    py::enum_<Key::Key>(m, "Key", "Key codes.")
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

    py::class_<Scene>(m, "Scene", 
        R"(Represents a game scene by holding a list of entity objects. The scene owns a main camera; the view of this camera gets rendered to the viewport.
           The scene owns a collection of systems that update game logic
        )")
        .def(py::init<>())
        .def("update", &Scene::update)
        .def("createEntity", &Scene::createEntity, py::return_value_policy::reference)
        .def("getMainCam", &Scene::getMainCam, py::return_value_policy::reference);

    py::class_<glm::vec2>(m, "Vec2", "2D vector of floats.")
        .def(py::init<float, float>())
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(float()* py::self)
        .def(py::self* float())
        .def(py::self / float())
        .def(py::self* py::self)
        .def(-py::self)
        .def_readwrite("x", &glm::vec2::x)
        .def_readwrite("y", &glm::vec2::y);

    py::class_<glm::vec3>(m, "Vec3", "3D vector of floats.")
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

    py::class_<glm::vec4>(m, "Vec4", "4D vector of floats.")
        .def(py::init<float, float, float, float>())
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(float()* py::self)
        .def(py::self* float())
        .def(py::self / float())
        .def(py::self* py::self)
        .def(-py::self)
        .def_readwrite("x", &glm::vec4::x)
        .def_readwrite("y", &glm::vec4::y)
        .def_readwrite("y", &glm::vec4::z)
        .def_readwrite("z", &glm::vec4::w);

    py::class_<Transform>(m, "Transform", "Describes 3D world position, rotation and scale of an object.")
        .def("getForward", &Transform::getForward, py::return_value_policy::copy, "Get normalised forward-facing vector.")
        .def("getRight", &Transform::getRight, py::return_value_policy::copy, "Get normalised right-facing vector.")
        .def("getUp", &Transform::getUp, py::return_value_policy::copy, "Get normalised upward-facing vector.")
        .def("getPos", &Transform::getPos, py::return_value_policy::copy, "Gets world position.")
        .def("setPos", py::overload_cast<float, float, float>(&Transform::setPos), "Sets world position.", "x"_a, "y"_a, "z"_a)
        .def("getRot", &Transform::getRot, py::return_value_policy::copy, "Gets rotation in Euler angles (in degrees).")
        .def("setRot", py::overload_cast<float, float, float>(&Transform::setRot), "Sets rotation in Euler angles (in degrees).", "x"_a, "y"_a, "z"_a)
        .def("getScale", &Transform::getScale, py::return_value_policy::copy, "Gets scale.")
        .def("setScale", py::overload_cast<float, float, float>(&Transform::setScale), "Sets scale.", "x"_a, "y"_a, "z"_a)
        .def("translate", py::overload_cast<float, float, float>(&Transform::translate), "Moves object by specified amount in each direction.")
        .def("translate", py::overload_cast<const glm::vec3&>(&Transform::translate), "Moves this object by applying provided vector to its position.")
        .def("rotate", py::overload_cast<float, float, float>(&Transform::rotate), "Rotates object by specified amount (in degrees) over each axis.")
        .def("rotate", py::overload_cast<const glm::vec3&>(&Transform::rotate), "Rotates object by by applying provided vector (in degrees) to its rotation.");

    py::enum_<Light::LightType>(m, "LightType")
        .value("DIR", Light::LightType::Directional, "Directional light.")
        .value("POINT", Light::LightType::Point, "Point light.")
        .value("SPOT", Light::LightType::Spot, "Spotlight.")
        .export_values();

    py::class_<Light>(m, "Light",
        R"( Illuminates a scene. There are three types of light: directional, point and spot.
			Directional lights only shine in one direction; they are used to model far away light sources such as the sun.
			Point lights create an area of lighting.
			Spotlights shine a concentrated beam of light with a provided angle and direction.
        )")
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

    py::class_<CameraComponent>(m, "Camera", "Device through which the user views the world.")
        .def("setPersp", &CameraComponent::setPerspective)
        .def("setOrtho", &CameraComponent::setOrtho)
        .def("setAspect", &CameraComponent::setAspect)
        .def("getTransform", &CameraComponent::getTransform, py::return_value_policy::reference);

    py::class_<AudioClip, std::shared_ptr<AudioClip>>(m, "AudioClip", "Stores an audio file from a given location.")
        .def(py::init<const std::string&>(), "Creates audio clip from a sound file.");

    py::class_<AudioListener>(m, "Listener", "Acts like a microphone within a scene to playback sounds. Only one Listener is permitted per scene.")
        .def("getGain", &AudioListener::getGain, py::return_value_policy::copy)
        .def("setGain", &AudioListener::setGain);

    py::class_<AudioSourceComponent>(m, "AudioSource", "Acts like a speaker; generates sounds in a scene. Must be provided with an AudioClip to play.")
        .def("get", &AudioSourceComponent::get, py::return_value_policy::reference);

    py::class_<AudioSource>(m, "Source")
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

    py::class_<Cubemap, std::shared_ptr<Cubemap>>(m, "Cubemap", "A collection of six textures that form a cube. Used in environment mapping and to create Skybox objects.")
        .def(py::init<const std::vector<std::string>&>(), "A list of filenames that correspond to each face of the cube. Order of faces : px, nx, py, ny, pz, nz.");

    py::class_<Texture, std::shared_ptr<Texture>>(m, "Texture", "A 2D texture that can support transparency.")
        .def(py::init<>(), "Creates default blank texture.")
        .def(py::init<const char*>(), "Loads texture from image file.");

    py::class_<Material, std::shared_ptr<Material>>(m, "Material", "Base class for materials. Every material has a shader that sets rendering properties in each render loop.");

    py::class_<DefaultMaterial, std::shared_ptr<DefaultMaterial>, Material>(m, "DefaultMaterial", "Default material that uses the Phong model.")
        .def(py::init<std::shared_ptr<Texture>, std::shared_ptr<Texture>, float>())
        .def("setDiffuse", &DefaultMaterial::setDiffuse, "Sets the diffuse texture.")
        .def("setSpecular", &DefaultMaterial::setSpecular, "Sets the specular texture.")
        .def("setReflection", &DefaultMaterial::setReflection, "Set a reflection cubemap.")
        .def("getShininess", &DefaultMaterial::getSpecularAmount, "The shininess of the material; higher values are less shiny.")
        .def("setShininess", &DefaultMaterial::setSpecularAmount, "The shininess of the material; higher values are less shiny.");

    py::class_<MeshRenderer>(m, "Renderer", "Renders models or meshes to the screen.")
        .def("setModel", &MeshRenderer::setModel);

    py::class_<Mesh, std::shared_ptr<Mesh>>(m, "Mesh", "A collection of vertices, normals and texture coordinates that define a polyhedral object. Each mesh has a Material.")
        .def(py::init<const std::vector<float>&, const std::vector<float>&, const std::vector<float>&, const std::vector<uint32_t>&>(), R"(
          Constructs a mesh by specifying vertex data and indices.
		  positions: Each three consecutive elements (starting at index 0) represent a vertex.
		  normals: Each three consectuive elements (starting at index 0) represent a normal for the corresponding vertex.
		  texCoords: Each two consecutive elements (starting at index 0) represent a texture coordinate for the corresponding vertex.
		  indices: Vector of pointers that choose what vertices get drawn from the positions list.
        )", "positions"_a, "normals"_a, "texCoords"_a, "indices"_a);

    py::class_<Model, std::shared_ptr<Model>>(m, "Model", "A 3D object that is comprised of a collection of Meshes.")
        .def(py::init<const std::string&>(), "Load model from a 3D object file.")
        .def(py::init<std::shared_ptr<Mesh>>(), "Create model by specifying a singlular mesh.")
        .def("getMeshes", &Model::getMeshes, py::return_value_policy::reference, "Returns list of meshes this model is made of.");

    py::class_<Skybox, std::shared_ptr<Skybox>, Model>(m, "Skybox", "A 3D background that is rendered behind everything in the scene. It gives the impression of objects or details at the horizon.")
        .def(py::init<std::shared_ptr<Cubemap>>());

    py::class_<Entity, std::shared_ptr<Entity>>(m, "Entity", "Base class for all game objects.")
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