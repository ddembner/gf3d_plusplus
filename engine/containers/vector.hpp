#pragma once
#include "defines.hpp"

namespace gf3d 
{
template<class T>
class vector
{
public:

	vector() : mData(nullptr), mCapacity(0), mSize(0)
	{

	}

	vector(const u64& size) : mCapacity(size), mSize(size)
	{
		mData = new T[size];
	}

	~vector()
	{
		delete[] mData;
		mData = nullptr;
		mCapacity = 0;
		mSize = 0;
	}

	inline T* data() const noexcept { return mData; }
	inline u64 capacity() const noexcept { return mCapacity; }
	inline u64 size() const noexcept { return mSize; }

private:
	T* mData;
	u64 mCapacity;
	u64 mSize;
};

} // namespace gf3d