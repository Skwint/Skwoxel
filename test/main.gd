extends Node3D

var ball_scene
var time_since_last_spawn = 0
var spawn_time = 0.5

# Called when the node enters the scene tree for the first time.
func _ready():
	$Skwoxel.generate();
	ball_scene = load("res://ball.tscn")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	time_since_last_spawn += delta;
	if time_since_last_spawn > spawn_time:
		time_since_last_spawn = 0
		var ball = ball_scene.instantiate();
		ball.position.x = randi_range(-100,100)
		ball.position.y = 100
		ball.position.z = randi_range(-100,100)
		add_child(ball)
	
