--
-- StamWaterVolume
-- Alex Queiroz
--

World.reset()

wv = StamWaterVolume(Point(0.0, 0.0, 0.0), 16, 1.0)
wv:setDiffuse(0.0001)
wv:setViscosity(0.0010)
wv:addSource(Point(8, 8, 14.5), Vector(0.0, 0.0, 0.0), 1.0)
wv:addToWorld()

noise = NoiseVolumeRenderer(wv, 0.1)
noise:addToWorld()

--World.setTimeStep(200)
World.start()
