extends RigidBody3D

var age
const life_span = 30

# Called when the node enters the scene tree for the first time.
func _ready():
	age = 0


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	age += delta
	if age > life_span:
		get_parent().remove_child(self)

