--
-- WaterVolume test
--

function FrameCallback(frame)
	print(frame)
--	print("density1: ", wv:density(2, 2,  2))
--	print("density2: ", wv:density(2, 2, 13))
	v = wv:velocity(10, 10, 4)
	print("velocity: ", v:x(), v:y(), v:z())
end

World.reset()

-- Setting up water volume
wv = FosterWaterVolume(Point(0.0, 0.0, 0.0), 16, 16, 16, 1.0, 1.0, 1.0)
wv:setViscosity(0.0)
wv:addSource(Point(10, 10, 6), 0.1)
--wv:addSink(Point(13.5, 13.5, 1.5), 0.01)
wv:addToWorld()

-- Setting up renderer
iso = IsoSurfaceRenderer(wv, 0.001)
iso:addToWorld()

World.start()
