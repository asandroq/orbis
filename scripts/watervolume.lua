--
-- WaterVolume test
--

function FrameCallback(frame)
	print("density: ", wv:density(2, 2, 2))
end

World.reset()

wv = WaterVolume(Point(0.0, 0.0, 0.0), 16, 1.0)
wv:addSource(Point(1.5, 1.5, 1.5), 50.0)
wv:addToWorld()

iso = IsoSurfaceRenderer(wv)
iso:addToWorld()

World.start()

