--
-- The Orbis Simulation Engine
-- Flies over a random terrain with a channel
-- Alex Sandro Queiroz e Silva <asandro@lcg.dc.ufc.br>
--

width = 128
xstep = 1.0
depth = 128
ystep = 1.0
origin = Point(-(width-1)*xstep/2, -(depth-1)*ystep/2)

cam = Camera(Point(0.0, 70.0, 5.0), Vector(0.0, -3, -1))

function FrameCallback(frame)
	p = cam:location()
	p = Point(p:x(), p:y() - 1.0, p:z())
	cam:setLocation(p)
	World.view(0):setCamera(cam)
end

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
		end
	end
end

terrain:addToWorld()

World.start()
