--
-- WaterVolume test
--

function FrameCallback(frame)
	print("density: ", wv:density( 5,  5,  5))
--	print("density2: ", wv:density(10, 10, 10))
	v = wv:velocity(5, 5, 5)
	print("velocity: ", v:x(), v:y(), v:z())
end

World.reset()

wv = WaterVolume(Point(0.0, 0.0, 0.0), 16, 1.0)
wv:addSource(Point(1.5, 1.5, 1.5), 0.001)
--wv:addSink(Point(13.5, 13.5, 1.5), 0.01)
wv:addToWorld()

iso = IsoSurfaceRenderer(wv)
iso:addToWorld()

World.start()
