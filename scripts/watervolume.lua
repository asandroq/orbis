--
-- WaterVolume test
--

function FrameCallback(frame)
	print(frame)
	print("density1: ", wv:density( 2,  2,  2))
--	print("density2: ", wv:density(10, 10, 10))
	v = wv:velocity(5, 5, 5)
	print("velocity: ", v:x(), v:y(), v:z())
end

World.reset()

-- Setting up water volume
wv = WaterVolume(Point(0.0, 0.0, 0.0), 16, 1.0)
wv:setDiffuse(1.0)
wv:setViscosity(10.0)
wv:addSource(Point(1.2, 1.2, 13.2), 1.0)
--wv:addSink(Point(13.5, 13.5, 1.5), 0.01)
wv:addToWorld()

-- Setting up renderer
iso = IsoSurfaceRenderer(wv)
iso:addToWorld()

World.start()
