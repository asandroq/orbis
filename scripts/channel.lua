--
-- The Orbis Simulation Engine
-- Random terrain with a channel
-- Alex Sandro Queiroz e Silva <asandro@lcg.dc.ufc.br>
--

width = 128
xstep = 1.0
depth = 128
ystep = 1.0
origin = Point(-(width-1)*xstep/2, -(depth-1)*ystep/2)

cam = Camera()
--[[
function FrameCallback(frame)
	if frame < 150 or math.mod(frame, 10) > 0 then
		return
	end
	cam:setLocation(Point(-60.0, -60.0, 5.0))
	local lookat = Point(60.0, 60.0, 0.0)
	cam:setView(lookat - cam:location())
	local wv = WorldView("POV", "orbis_pov" .. frame .. ".pov")
	wv:setCamera(cam)
	wv:render()
	print("wrote frame orbis_pov" .. frame .. ".pov")
end
]]
World.reset()

-- creating terrain
terrain = GridTerrain(origin, xstep, ystep, width, depth)
terrain:setTexture("LlanoTex2.jpg")
terrain:faultLineGeneration(200)
terrain:smooth(0.7)

patch = Patch()
patch:addPoints(Point(-10, -10), Point(10, -10), Point(15, 0), Point(10, 10), Point(-10, 10))
patch:setAttribute("texture", "dirt.jpg")
terrain:addPatch(patch)

-- now I'm gonna sweep a 2d paraboloid along the channel axis
for j = 0, depth - 1 do
	for i = 0, width - 1 do
		local p = terrain:point(i, j)
		local h = ((p:x()) ^ 2.0) / 10.0 - 10.0
		if h < p:z() then
			terrain:setPoint(i, j, h)
			if j == 64 then
				local loc = terrain:point(i, j)
				loc = Point(loc:x(), loc:y(), loc:z() + 1.0)
				cam:setLocation(loc)
			end
		end
	end
end

terrain:addToWorld()

World.view(0):setCamera(cam)

-- creating flowing water

water = WaterHeightField(origin, xstep, ystep, width, depth)
water:setTexture("water001.jpg")
water:addSource(Point(0, 60), 0.001)
water:setBottom(terrain)
water:addToWorld()

--World.setTimeStep(100)
World.start()
