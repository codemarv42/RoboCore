extends Node3D


# Called when the node enters the scene tree for the first time.
func _ready():
	var sizes = [[90,10,5],[20,10,5],[75,10,5],[15,10,5],[50,10,5],[120,10,5]]
	var transforms = [Vector3(0,0,-5),Vector3(90,0,5),Vector3(90,0,5+45),Vector3(90,0,120),Vector3(50,0,120),Vector3(0,0,120)]
	var rotates = [0,90,90,0,180,180,270]
	var waende = AnimatableBody3D.new()
	waende.set_name("waende")
	add_child(waende)
	for i in range(6):
		var collision = CollisionShape3D.new()
		var shape = BoxShape3D.new()
		var size = Vector3()
		size[0] = sizes[i][0]
		size[1] = sizes[i][1]
		size[2] = sizes[i][2]
		shape.set_size(0.001*size)
		collision.shape = shape
		collision.rotate(Vector3(0,1,0),rotates[i])
		collision.translate(transforms[i])
		add_child(collision)
		var debug_mesh = shape.get_debug_mesh()
		var mesh_instance = MeshInstance3D.new()
		mesh_instance.rotate(Vector3(0,1,0),rotates[i])
		mesh_instance.translate(transforms[i])
		mesh_instance.mesh = debug_mesh
		var material = StandardMaterial3D.new()
		material.flags_unshaded = true
		material.albedo_color = Color(1,1,1,1)
		mesh_instance.material_override = material
		add_child(mesh_instance)
	var schild = Label3D.new()
	schild.text = "HIER"
	show()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass
