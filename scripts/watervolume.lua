--
-- WaterVolume test
--

World.reset()

-- Setting up water volume
wv = FosterWaterVolume(Point(0.0, 0.0, 0.0), 16, 16, 16, 1.0, 1.0, 1.0)
wv:setViscosity(0.0)
wv:addSource(Point(1.5, 1.5, 1.5), Vector(0.5, 0.0, 0.0), 0.1)
wv:addSource(Point(2.5, 1.5, 1.5), Vector(0.5, 0.0, 0.0), 0.1)
wv:addSource(Point(1.5, 2.5, 1.5), Vector(0.5, 0.0, 0.0), 0.1)
wv:addSource(Point(2.5, 2.5, 1.5), Vector(0.5, 0.0, 0.0), 0.1)
--wv:addSource(Point(10.5, 10.5, 6.5), 0.1)
--wv:addSink(Point(13.5, 13.5, 1.5), 0.01)
wv:addToWorld()

-- Setting up renderer
iso = IsoSurfaceRenderer(wv, 0.001)
iso:addToWorld()

World.start()
