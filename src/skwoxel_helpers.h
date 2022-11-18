#ifndef SKWOXEL_HELPERS_H
#define SKWOXEL_HELPERS_H

#define SKWOXEL_BIND_SET_GET_METHOD(cc, mm) \
{ \
	ClassDB::bind_method(D_METHOD("get_" #mm), &cc::get_##mm); \
	ClassDB::bind_method(D_METHOD("set_" #mm, #mm), &cc::set_##mm); \
}

#define SKWOXEL_ADD_PROPERTY(type, mm) \
{ \
	ADD_PROPERTY(PropertyInfo(type, #mm), "set_"#mm, "get_"#mm); \
}

#define SKWOXEL_SET_METHOD(mm) \
{ \
	if (name == #mm) { \
		set_##mm(p_value); \
		return true; \
	} \
}

#define SKWOXEL_GET_METHOD(mm) \
{ \
	if (name == #mm) { \
		r_ret = get_##mm(); \
		return true; \
	} \
}

#define SKWOXEL_INLINE_SET_GET(type, mm) \
	type get_##mm() const { return mm;} \
	void set_#mm(type p_##mm) { mm = p_##mm; }

namespace skwoxel
{
	const real_t normal_offset = 0.1;


	// identical to the godot version at time of writing
	template <typename T>
	inline T clamp(T val, T min, T max)
	{
		return val < min ? min : val > max ? max : val;
	}

	// sort of like gopot smoothstep, except people keep changing it
	// this version does not check for the error condition of equal
	// values, it just divides by zero and dies.
	template <typename T>
	inline T smooth_step(T from, T to, T val)
	{
		T x = clamp((val - from) / (to - from), 0.0f, 1.0f);
		return x * x * (3.0 - 2.0 * x);
	}

	// smooth_step except more gradual at the edges and steeper near the middle
	template <typename T>
	inline T smoother_step(T from, T to, T val)
	{
		T x = clamp((val - from) / (to - from), 0.0, 1.0);
		return x * x * x * (x * (x * 6.0 - 15.0) + 10.0);
	}
}

#endif
