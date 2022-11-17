#include "skwoxel_field_translate.h"

#include "random.h"
#include "skwoxel_helpers.h"

using namespace godot;

namespace skwoxel
{
	void SkwoxelFieldTranslate::_notification(int p_what) {
	}

	bool SkwoxelFieldTranslate::_set(const StringName& p_name, const Variant& p_value) {
		String name = p_name;
		SKWOXEL_SET_METHOD(translation);

		return false;
	}

	bool SkwoxelFieldTranslate::_get(const StringName& p_name, Variant& r_ret) const {
		String name = p_name;
		SKWOXEL_GET_METHOD(translation);

		return false;
	}

	String SkwoxelFieldTranslate::_to_string() const {
		return "[Skwoxel field : TRANSLATE]";
	}

	void SkwoxelFieldTranslate::_get_property_list(List<PropertyInfo>* list) const {
	}

	bool SkwoxelFieldTranslate::_property_can_revert(const StringName& p_name) const {
		return false;
	}

	bool SkwoxelFieldTranslate::_property_get_revert(const StringName& p_name, Variant& r_property) const {
		return false;
	}

	void SkwoxelFieldTranslate::_bind_methods() {
		// Methods.
		SKWOXEL_BIND_SET_GET_METHOD(SkwoxelFieldTranslate, translation);
		SKWOXEL_ADD_PROPERTY(Variant::VECTOR3, translation);
	}

	SkwoxelFieldTranslate::SkwoxelFieldTranslate() :
		SkwoxelFieldAdd(),
		translation(0.0, 0.0, 0.0)
	{

	}

	SkwoxelFieldTranslate::~SkwoxelFieldTranslate()
	{

	}

	void SkwoxelFieldTranslate::trigger(const Vector3& pos, const Vector3& untransformed)
	{
		SkwoxelFieldAdd::trigger(pos - translation, untransformed);
	}

	real_t SkwoxelFieldTranslate::strength(const Vector3& pos) const
	{
		return SkwoxelFieldAdd::strength(pos - translation);
	}
}
