extends Node3D

# Called when the node enters the scene tree for the first time.
func _ready():
	var twister : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister
	var num = twister.num_points
	for i in range(0,num):
		var r : Vector2 = Vector2(randf_range(-10.0, 10.0), randf_range(-10.0, 10.0))
		twister.set_point(i, r)
	$Skwoxel.generate()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
