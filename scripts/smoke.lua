--
-- StamWaterVolume
-- Alex Queiroz
--

World.reset()

wv = StamWaterVolume(Point(0.0, 0.0, 0.0), 32, 0.5)
wv:setDiffuse(0.00001)
wv:setViscosity(0.0100)
wv:addSource(Point(8, 8, 1.5), Vector(0.0, 0.0, 10000.0), 1.0)
--wv:addSource(Point(8, 8, 10.5), Vector(5.0, 5.0, 0.0), 0.0)
wv:addToWorld()

noise = NoiseVolumeRenderer(wv, 0.001)
noise:addToWorld()

cam = Camera()
cam:setLocation(Point(20, 20, 10))
cam:setView(Vector(-20, -20, -10))
World.view(0):setCamera(cam)


World.start()
