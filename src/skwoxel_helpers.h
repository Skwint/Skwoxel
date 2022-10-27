#ifndef SKWOXEL_HELPERS_H
#define SKWOXEL_HELPERS_H

#define SKWOXEL_BIND_SET_GET_METHOD(cc, mm) \
{ \
	ClassDB::bind_method(D_METHOD("get_" #mm), &cc::get_##mm); \
	ClassDB::bind_method(D_METHOD("set_" #mm, #mm), &cc::set_##mm); \
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

#endif
