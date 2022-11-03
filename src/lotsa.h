#ifndef SKWOXEL_LOTSA_H
#define SKWOXEL_LOTSA_H

namespace skwoxel
{
	// This is an extremely simple block of data similar to a vector
	// It is here because we avoid STL in godot, but I want to avoid
	// all of the overheads (and weirdness) of cowdata.
	template <typename T>
	class lotsa
	{
	public:
		lotsa() :
			mData(0),
			mCapacity(0),
			mSize(0)
		{
		}
		~lotsa()
		{
			delete[] mData;
		}
		void clear()
		{
			delete[] mData;
			mData = 0;
			mCapacity = 0;
			mSize = 0;
		}
		T& operator[](int index) { return *(mData + index); }
		const T& operator[](int index) const { return *(mData + index); }
		uint32_t size() { return mSize; }
		uint32_t capacity() { return mCapacity; }
		void resize(uint32_t p_size) { if (p_size > mCapacity) reserve(p_size); mSize = p_size; }
		void reserve(uint32_t p_capacity)
		{
			if (mCapacity < p_capacity)
			{
				T* newData = new T[p_capacity];
				delete[] mData;
				mData = newData;
				mCapacity = p_capacity;
			}
		}
		void push_back(const T& val)
		{
			if (mSize == mCapacity)
			{
				if (mCapacity == 0)
				{
					reserve(100);
				}
				else
				{
					reserve(mCapacity * 2);
				}
			}
			*(mData + mSize) = val;
			++mSize;
		}

	private:
		T* mData;
		uint32_t mCapacity;
		uint32_t mSize;
	};
}

#endif
