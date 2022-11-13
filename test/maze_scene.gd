extends Node3D

var skwox
const num_rooms = 3
const horizontal_gap = 30
const room_radius = 10.0

# Called when the node enters the scene tree for the first time.
func _ready():
	skwox = $Skwoxel
	for x in range(0,num_rooms):
		for y in range(0,num_rooms):
			for z in range(0,num_rooms):
				create_room(horizontal_gap * Vector3(x, y, z))
	skwox.generate()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func create_room(pos):
	var sphere = SkwoxelFieldSphere.new()
	sphere.set_radius(room_radius)
	sphere.set_centre(pos)
	sphere.set_inner_strength(1.0)
	skwox.add_child(sphere)
