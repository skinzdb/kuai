# kuai
3D Game Engine.

## Build

This project uses CMake as its build system. To build kuai from scratch, follow these steps: 

1) Clone the repository recursively:
``` git clone --recursive git@github.com:skinzdb/kuai.git ```
2) Create a build folder inside the `PythonBinder/` directory.
3) Create project files by running the following command inside the build folder:
``` cmake .. ```  
Alternatively, you can use the CMake GUI.
4) Open the generated Visual Studio project and build the ALL_BUILD target.
5) Extract the Python module from the build destination (usually located in `Debug` or `Release` depending on your build configuration).


Note that kuai engine only supports Windows x64 systems. If using the pre-built Python module, PLEASE ensure you run it using Python 3.10.x

## First Steps

Once the `pykuai.py` Python module is built, you can import it into your Python script like so:
```Python
from pykuai import *
```
To make your first window, you need to create an `App` object and run it. This is the basis of any kuai application.

```Python
app = App()
app.run()
```
You'll notice the window is blank and doesn't have any behaviour - which is no fun. We can introduce behaviour via layers.

### Layer:

A layer is a class that can be attached to an app to add your own behaviours. You will need to create a class that extends from the `Layer` base class like so:
```Python
class MyLayer(Layer):
    def __init__(self):     
        Layer.__init__(self)
    def update(self, dt):  
        pass
    def onEvent(self, dt):   
        pass
```
There are three main functions: 
- `__init__()`: Called when the layer is first pushed to the application. You must specify `Layer.__init__(self)` first for the base class.
- `update(dt)`: Called once a frame; `dt` is the amount of time elapsed in seconds since the last frame.
- `onEvent(event)`: Called upon an event firing. The parameter, `event`, contains the information of that event.

**NOTE**: It is necessary to provide definitions of these functions in your derived layer class otherwise you will get an error.

Attach the layer to the application. The code should look like this:
```Python
# Layer Class Code
.
.
.
app = App()
layer = MyLayer()
app.pushLayer(layer)
app.run()
```

### Scene:
Scenes facilitate the creation and destruction of entities. Let's initialise a scene and our first entity in the `MyLayer` class we made:
```Python
class MyLayer(Layer):
    def __init__(self):
        Layer.__init__(self)

        self.scene = Scene()
        self.myEntity = self.scene.createEntity()
        .
        .
        .
```
Note that the variables are being created as class members (from the `self.` prefix) so they can be used in other functions in the class. The scene must be updated every frame so this needs to be added to the layer update function:
```
self.scene.update(dt)
```

### Components:

Components can be attached to entities to give them properties and behaviour. A `Transform` component is attached to every entity by default. This allows us to control the position, rotation and scale of an object:
- ```setPos(x, y, z)```
- ```setRot(x, y, z)``` - in degrees, not radians!
- ```setScale(x, y, z)```  

We can also retrieve these properties with ```getPos()```, ```getRot()``` and ```getScale()```. Like any component, you will need to "get" it first by calling `entity.getTransform()` for example. This may be nice but we still have nothing rendering to the screen yet. To achieve our drawing goals, an entity must have a `Renderer` component. We'll also need to give this renderer component a 3D model for it to draw:
```Python
model = Model(<filename>)   # Load a 3D object

self.myEntity.addRenderer() # Attach renderer component
self.myEntity.getRenderer().setModel(model) # Set a model for the renderer component to draw
```
**NOTE**: The `Model` class loads a 3D object file from `<filename>`.   
You may need to adjust the positioning of the object for it to appear in front of the camera. We can set the position of the entity forwards if need be:
```Python
self.myEntity.getTransform().setPos(0, 0, -1)
```
**NOTE**: The forwards z-direction (i.e. going into the screen) is negative.

If all has gone to plan then you should be able to see your model on the screen. 
### Final code listing:
```Python
from pykuai import *
import sys

class MyLayer(Layer):
    def __init__(self):     
        Layer.__init__(self)

        self.scene = Scene()
        self.myEntity = self.scene.createEntity()

        model = Model(sys.path[0] + "\\" + "bunny.obj")

        self.myEntity.addRenderer()
        self.myEntity.getRenderer().setModel(model) 

        self.myEntity.getTransform().translate(0, -1, -2)

    def update(self, dt):  
        self.scene.update(dt)
    def onEvent(self, dt):   
        pass

app = App()
layer = MyLayer()
app.pushLayer(layer)
app.run()
```

The `sys` library is being used here to get the local file path of the program.

## Documentation

The documentation for the C++ engine can be found here: http://www.reddit.com.
