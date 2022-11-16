#include "skwoxel_field_trigger.h"

#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	static const char* SKWOXEL_SIGNAL_TRIGGER = "skwoxel_trigger";

	SkwoxelFieldTrigger::SkwoxelFieldTrigger() : SkwoxelField()
	{

	}

	SkwoxelFieldTrigger::~SkwoxelFieldTrigger()
	{
	}

	void SkwoxelFieldTrigger::_notification(int p_what) {
	}

	bool SkwoxelFieldTrigger::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(point);
		return false;
	}

	bool SkwoxelFieldTrigger::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(point);
		return false;
	}

	String SkwoxelFieldTrigger::_to_string() const {
		return "[Skwoxel field : TRIGGER]";
	}

	void SkwoxelFieldTrigger::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldTrigger::_property_can_revert(const StringName& p_name) const {
		return true;
	}

	bool SkwoxelFieldTrigger::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldTrigger::_bind_methods()
	{
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTrigger, point);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, point);
		ADD_SIGNAL(MethodInfo(SKWOXEL_SIGNAL_TRIGGER, PropertyInfo(Variant::VECTOR3, "point")));
	}

	void SkwoxelFieldTrigger::pre_generate(bool randomize_seeds)
	{
		distance_squared = 99999999.0;
		closest = point;
		SkwoxelField::pre_generate(randomize_seeds);
	}

	real_t SkwoxelFieldTrigger::strength(const Vector3 & pos, const Vector3& untransformed) const
	{
		// This is the wrong point we need the point it came from ...
		real_t distsq = (pos - point).length_squared();
		if (distsq < distance_squared)
		{
			closest = untransformed;
			distance_squared = distsq;
		}
		return 0.0;
	}

	void SkwoxelFieldTrigger::post_generate()
	{
		emit_signal(SKWOXEL_SIGNAL_TRIGGER, closest);
		SkwoxelField::post_generate();
	}
}
