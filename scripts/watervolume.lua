--
-- WaterVolume test
--

function FrameCallback(frame)
	print("density1: ", wv:density( 2,  2,  2))
	print("density2: ", wv:density(10, 10, 10))
end

World.reset()

wv = WaterVolume(Point(0.0, 0.0, 0.0), 16, 1.0)
wv:addSource(Point(1.5, 1.5, 1.5), 0.1)
wv:addToWorld()

iso = IsoSurfaceRenderer(wv)
iso:addToWorld()

World.start()
