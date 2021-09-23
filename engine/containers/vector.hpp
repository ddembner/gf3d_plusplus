#pragma once
#include "defines.hpp"
#include "core/gf3d_assert.hpp"

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
		for (u64 i = 0; i < mSize; i++) {
			mData[i].~T();
		}
		std::free(mData);
		mData = nullptr;
	}

	inline T* data() const noexcept { return mData; }
	inline u64 capacity() const noexcept { return mCapacity; }
	inline u64 size() const noexcept { return mSize; }
	inline u64 max_size() const noexcept { return 0xffffffffffffffff / sizeof(T); }

	void reserve(const u64 newCapacity);
	void clear() noexcept;
	void push_back(const T& value);
	void push_back(const T&& value);

	template<typename... Args>
	T& emplace_back(Args&&... args)
	{
		if(mSize >= mCapacity) {
			reallocate(mCapacity + mCapacity / 2);
		}

		mData[mSize] = T(std::forward<Args>(args)...);

		mSize++;
		return mData[mSize - 1];
	}

	T& operator[](u64 index) noexcept;

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
	T* newData = reinterpret_cast<T*>(std::malloc(sizeof(T) * newCapacity));

	for (u64 i = 0; i < mSize; i++) {
		newData[i] = std::move(mData[i]);
	}

	mData = newData;
	mCapacity = newCapacity;
}

template<class T>
inline void vector<T>::reserve(const u64 newCapacity)
{
	if (newCapacity > capacity()) {
		GFASSERT(newCapacity < max_size(), "Cannot allocate larger than max allocation size");
	}
	
	reallocate(newCapacity);
}

template<class T>
inline void vector<T>::clear() noexcept
{
	for (u32 i = 0; i < mSize; i++) {
		mData[i].~T();
	}
	
	mSize = 0;
}

template<class T>
inline void vector<T>::push_back(const T& value)
{
	
}

template<class T>
inline T& vector<T>::operator[](u64 index) noexcept
{
	GFASSERT(index < mSize, "Index out of range");
	return mData[index];
}

} // namespace gf3d