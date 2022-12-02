extends Node3D

var skwox
var room_scene
const num_rooms = 4
const horizontal_gap = 60
const vertical_gap = 16
const room_radius = 10.0

var rooms;
var grid = null

func get_default_cell():
	var cell = {}
	cell.wall = []
	for i in range(0,3):
		cell.wall.append(true)
	cell.group = 0
	return cell

func create_grid():
	grid = []
	var group = 0
	for x in range(num_rooms):
		grid.append([])
		for y in range(num_rooms):
			grid[x].append([])
			for z in range(num_rooms):
				var cell = get_default_cell()
				cell.group = group
				group = group + 1
				grid [x][y].append(cell)

func connect_cells(cell1, cell2, wall):
	var dead_group = cell2.group
	for x in range(num_rooms):
		for y in range(num_rooms):
			for z in range(num_rooms):
				var temp = grid[x][y][z]
				if temp.group == dead_group:
					temp.group = cell1.group
	cell1.wall[wall] = false
	

# This is a fairly dumb maze builder but it does work.
# it would be more efficient if it kept a cache of viable walls and chose
# from those at random
func build_maze():
	var num_groups = num_rooms * num_rooms * num_rooms
	while num_groups > 1:
		var ix = randi_range(0, num_rooms - 1)
		var iy = randi_range(0, num_rooms - 1)
		var iz = randi_range(0, num_rooms - 1)
		var cell = grid[ix][iy][iz]
		var dir = randi_range(0, 2)
		var cell2 = null
		if dir == 0 and ix < num_rooms - 1:
			cell2 = grid[ix+1][iy][iz]
		elif dir == 1 and iy < num_rooms - 1:
			cell2 = grid[ix][iy+1][iz]
		elif dir == 2 and iz < num_rooms - 1:
			cell2 = grid[ix][iy][iz+1]
		if cell2 != null and cell.group != cell2.group:
			connect_cells(cell, cell2, dir)
			num_groups = num_groups - 1

# Called when the node enters the scene tree for the first time.
func _ready():
	skwox = $Skwoxel
	room_scene = load("res://room.tscn")
	create_grid()
	build_maze()
	for x in range(0,num_rooms):
		for y in range(0,num_rooms):
			for z in range(0,num_rooms):
				create_room(Vector3(horizontal_gap * x, vertical_gap * y, horizontal_gap * z), x, y, z)
	skwox.generate()

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass

func create_room(pos, x, y, z):
	var trans = SkwoxelFieldTranslate.new()
	trans.set_translation(pos)
	var room = room_scene.instantiate()
	var cell = grid[x][y][z]
	var ramps = []
	ramps.append(room.find_child("ramp_up_1"))
	ramps.append(room.find_child("ramp_up_2"))
	ramps.append(room.find_child("ramp_up_3"))
	if not cell.wall[1]:
		var ramp = randi_range(0,2)
		for rnum in range(0,3):
			if rnum == ramp:
				#ramps[rnum].set_curve(ramps[rnum].get_curve().duplicate())
				pass
			else:
				room.remove_child(ramps[rnum])
	else:
		for rnum in range(0,3):
			room.remove_child(ramps[rnum])
	
	if not cell.wall[0]:
		var end = Vector3(horizontal_gap, 0.0, 0.0)
		var dir = Vector3(20.0, 0.0, 0.0)
		room.add_child(create_path(dir, end))
	
	if not cell.wall[2]:
		var end = Vector3(0.0, 0.0, horizontal_gap)
		var dir = Vector3(0.0, 0.0, 20.0)
		room.add_child(create_path(dir, end))
	
	if y > 0:
		var below = grid[x][y-1][z]
		if not below.wall[1]:
			var middle = room.find_child("full_floor")
			if middle != null:
				room.remove_child(middle)
	
	trans.add_child(room)
	skwox.add_child(trans)

func create_path(dir, end) :
		var angle = randf_range(-0.5, 0.5)
		var sdir = dir.rotated(Vector3(0,1,0),angle)
		angle = randf_range(-0.5, 0.5)
		var edir = -dir.rotated(Vector3(0,1,0),angle)
		var epos = end + edir
		var walk : SkwoxelFieldCurve = SkwoxelFieldCurve.new()
		var curve : Curve3D = Curve3D.new()
		curve.add_point(sdir, sdir, sdir)
		curve.add_point(epos, edir)
		walk.set_curve(curve)
		walk.set_start_radius(4.0)
		walk.set_end_radius(4.0)
		walk.set_inner_strength(2.0)
		walk.set_outer_strength(-2.0)
		walk.sliced = true
		return walk;
