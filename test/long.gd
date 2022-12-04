extends Node3D

const num = 20

# Called when the node enters the scene tree for the first time.
func _ready():
	var twister : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister
	var twister2 : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister2
	var twister3 : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister3
	var twister4 : SkwoxelFieldTwister = $Skwoxel/SkwoxelFieldTwister4
	
	var curve = Curve2D.new()
	var curve2 = Curve2D.new()
	var curve3 = Curve2D.new()
	var curve4 = Curve2D.new()
	
	for i in range(0,num):
		var r = 80.0
		var theta = TAU * i / num
		var pos : Vector2 = Vector2(r, theta)
		curve.add_point(pos)
		pos.y += 0.25 * TAU
		curve2.add_point(pos)
		pos.y += 0.5 * TAU
		curve3.add_point(pos)
		pos.y += 0.75 * TAU
		curve4.add_point(pos)
	twister.curve = curve
	twister2.curve = curve2
	twister3.curve = curve3
	twister4.curve = curve4
	twister.auto_control_points()
	twister2.auto_control_points()
	twister3.auto_control_points()
	twister4.auto_control_points()
	$Skwoxel.generate()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta):
	pass
