--
-- The Orbis Simulation Engine
-- WaterVolume test
-- Alex Sandro Queiroz e Silva <asandro@lcg.dc.ufc.br>
--

World.reset()

-- Setting up water volume
wv = FosterWaterVolume(Point(0.0, 0.0, 0.0), 16, 16, 16, 0.5, 0.5, 0.5)
wv:setViscosity(0.001)
wv:addSource(Point(0.75, 1.5, 0.75), Vector(0.0, 1.0, 0.0), 0.1)
wv:addSource(Point(1.25, 1.5, 0.75), Vector(0.0, 1.0, 0.0), 0.1)
wv:addSource(Point(1.75, 1.5, 0.75), Vector(0.0, 1.0, 0.0), 0.1)
--wv:addSource(Point(1.5, 2.5, 1.5), Vector(0.0, 0.5, 0.0), 0.1)
--wv:addSource(Point(2.5, 2.5, 1.5), Vector(0.0, 0.5, 0.0), 0.1)
--wv:addSource(Point(3.5, 2.5, 1.5), Vector(0.0, 0.5, 0.0), 0.1)
--wv:addSource(Point(2.5, 2.5, 1.5), Vector(0.5, 0.0, 0.0), 0.1)
--wv:addSource(Point(10.5, 10.5, 6.5), 0.1)
--wv:addSink(Point(13.5, 13.5, 1.5), 0.01)
wv:addToWorld()

-- Setting up renderer
iso = IsoSurfaceRenderer(wv, 0.001)
iso:addToWorld()

World.setTimeStep(10)
World.start()
