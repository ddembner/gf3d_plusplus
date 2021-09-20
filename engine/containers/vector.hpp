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
		mData = reinterpret_cast<T*>(std::malloc(sizeof(T) * mCapacity));
		for (u64 i = 0; i < mSize; i++) {
			new (mData + i) T();
		}
	}

	~vector()
	{
		std::free(mData);
		mData = nullptr;
		mCapacity = 0;
		mSize = 0;
	}

	inline T* data() const noexcept { return mData; }
	inline u64 capacity() const noexcept { return mCapacity; }
	inline u64 size() const noexcept { return mSize; }
	inline u64 max_size() const noexcept { return 0xffffffffffffffff / sizeof(T); }

	void reserve(const u64 newCapacity);

	T& vector<T>::operator[](u64 index) noexcept;

private:
	void reallocate(const u64 newCapacity);

private:
	T* mData;
	u64 mCapacity;
	u64 mSize;
};

template<class T>
inline void vector<T>::reallocate(const u64 newCapacity)
{
	mData = reinterpret_cast<T*>(std::malloc(sizeof(T) * newCapacity));
	mCapacity = newCapacity;
}

template<class T>
inline void vector<T>::reserve(const u64 newCapacity)
{
	if (newCapacity > capacity()) {
		if (newCapacity > max_size()) {
			LOGGER_ERROR("Cannot allocate larger than max allocation size");
			return;
		}
	}
	
	reallocate(newCapacity);
}

template<class T>
inline T& vector<T>::operator[](u64 index) noexcept
{
	return mData[index];
}

} // namespace gf3d