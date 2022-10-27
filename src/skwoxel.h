/* godot-cpp integration testing project.
 *
 * This is free and unencumbered software released into the public domain.
 */

#ifndef SKWOXEL_SKWOXEL_H
#define SKWOXEL_SKWOXEL_H

#include <godot_cpp/classes/Node3d.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/core/binder_common.hpp>

using namespace godot;

namespace skwoxel
{
	class Skwoxel : public Node3D {
		GDCLASS(Skwoxel, Node3D);

	protected:
		static void _bind_methods();

		void _notification(int p_what);
		bool _set(const StringName& p_name, const Variant& p_value);
		bool _get(const StringName& p_name, Variant& r_ret) const;
		void _get_property_list(List<PropertyInfo>* p_list) const;
		bool _property_can_revert(const StringName& p_name) const;
		bool _property_get_revert(const StringName& p_name, Variant& r_property) const;

		String _to_string() const;

		void allocate_fields();
		void delete_fields();
		inline int size_x() { return upper_bounds.x + 1 - lower_bounds.x; }
		inline int size_y() { return upper_bounds.y + 1 - lower_bounds.y; }
		inline int size_z() { return upper_bounds.z + 1 - lower_bounds.z; }
		inline float& field_at_local_unsafe(int x, int y, int z);
		inline float& field_at_global_unsafe(int x, int y, int z);
		inline float& field_at_local(int x, int y, int z);
		inline float& field_at_global(int x, int y, int z);

	private:
		Vector3i lower_bounds;
		Vector3i upper_bounds;
		float* field_strengths;

	public:
		Skwoxel();
		~Skwoxel();

		void clear_fields();
		void generate_fields();

		// Property.
		void set_lower_bounds(const Vector3i& bounds);
		Vector3i get_lower_bounds() const;
		void set_upper_bounds(const Vector3i& bounds);
		Vector3i get_upper_bounds() const;
	};

	inline float& Skwoxel::field_at_local_unsafe(int x, int y, int z)
	{
		return *(field_strengths + (z * size_y() + y) * size_x() + x);
	}

	inline float& Skwoxel::field_at_global_unsafe(int x, int y, int z)
	{
		return field_at_local_unsafe(x - lower_bounds.x, y - lower_bounds.y, z - lower_bounds.z);
	}

	inline float& Skwoxel::field_at_local(int x, int y, int z)
	{
		x = godot::Math::clamp(x, 0, size_x() - 1);
		y = godot::Math::clamp(y, 0, size_y() - 1);
		z = godot::Math::clamp(z, 0, size_z() - 1);
		return field_at_local_unsafe(x, y, z);
	}

	inline float& Skwoxel::field_at_global(int x, int y, int z)
	{
		return field_at_local(x - lower_bounds.x, y - lower_bounds.y, z - lower_bounds.z);
	}

}

#endif // SKWOXEL_SKWOXEL_H
