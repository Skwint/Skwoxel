extends Node3D

# Called when the node enters the scene tree for the first time.
func _ready():
	var twister : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister
	var twister2 : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister2
	var num = twister.num_points
	for i in range(0,num):
		var r : Vector2 = Vector2(randf_range(-10.0, 10.0), randf_range(-10.0, 10.0))
		twister.set_point(i, r)
		var theta = TAU * i / num
		var pos : Vector2 = 30.0 * Vector2(cos(theta), sin(theta))
		twister2.set_point(i, pos)
	$Skwoxel.generate()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
