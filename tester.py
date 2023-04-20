from pykuai import *

class MyLayer(Layer):
    def __init__(self):
        Layer.__init__(self)

        self.scene = Scene()

        bunnyModel = Model("C:/Users/David/Documents/bunny.obj")
        song = AudioClip("C:/Users/David/Music/videotape.wav")

        bunny = self.scene.createEntity()
        bunny.addRenderer().setModel(bunnyModel)]
        bunny.addAudioSource()
        bunny.getAudioSource().source.setClip(song)
        bunny.getAudioSource().source.play()

        pass
    def update(self, dt):
        self.scene.update(dt)
        pass
    def onEvent(self, event):
        pass

app = App()
layer = MyLayer()
app.pushLayer(layer)
app.run()
