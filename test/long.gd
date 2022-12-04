extends Node3D

# Called when the node enters the scene tree for the first time.
func _ready():
	var twister : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister
	var twister2 : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister2
	var twist : SkwoxelFieldTwist = $Skwoxel/SkwoxelFieldTwist
	var twisted : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwist/SkwoxelFieldTwister
	var num = twister.num_points
	for i in range(0,num):
		var r : Vector2 = Vector2(randf_range(-10.0, 10.0), randf_range(-10.0, 10.0))
		twister.set_point(i, r)
		var theta = TAU * i / num
		var pos : Vector2 = 50.0 * Vector2(cos(theta), sin(theta))
		var tight : Vector2 = 20.0 * Vector2(cos(theta), -sin(theta))
		twister2.set_point(i, pos)
		twist.set_point(i, pos)
		twisted.set_point(i, tight)
	$Skwoxel.generate()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
