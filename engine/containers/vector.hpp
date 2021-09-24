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
		mData = reinterpret_cast<T*>(::operator new(sizeof(T) * mCapacity));
		for (u64 i = 0; i < mSize; i++) {
			new (mData + i) T();
		}
	}

	vector(const u64& size, const T& value) : mCapacity(size), mSize(size)
	{
		mData = reinterpret_cast<T*>(::operator new(sizeof(T) * mCapacity));

		for (u64 i = 0; i < mCapacity; i++) {
			construct_copy_in_place(mData[i], value);
		}
	}

	~vector()
	{
		clear();
		::operator delete(mData, sizeof(T) * mCapacity);
		mData = nullptr;
	}

	inline T* data() const noexcept { return mData; }
	inline u64 capacity() const noexcept { return mCapacity; }
	inline u64 size() const noexcept { return mSize; }
	inline u64 max_size() const noexcept { return 0xffffffffffffffff / sizeof(T); }
	inline T* begin() const noexcept { return mData; } // TODO: Make iterator

	void reserve(const u64 newCapacity);
	void clear() noexcept;
	void push_back(const T& value);
	void push_back(T&& value);
	void pop_back();

	template<typename... Args>
	T& emplace_back(Args&&... args)
	{
		u64 newSize = mSize + 1;
		
		if (newSize >= mCapacity) {

			u64 growth = mCapacity + mCapacity / 2;
			if (growth < newSize)
				reallocate(newSize);
			else
				reallocate(growth);
		}

		new(&mData[mSize]) T(std::forward<Args>(args)...);
		mSize = newSize;
		return mData[mSize - 1];
	}

	T& operator[](u64 index) noexcept;

private:
	void reallocate(const u64 newCapacity);

private:
	template<typename... Args>
	inline void construct_copy_in_place(T& object, Args&&... args)
	{
		::new (reinterpret_cast<void*>(&object)) T(std::forward<Args>(args)...);
	}

private:
	T* mData;
	u64 mCapacity;
	u64 mSize;
};

template<class T>
inline void vector<T>::reallocate(const u64 newCapacity)
{
	T* newData = reinterpret_cast<T*>(::operator new(sizeof(T) * newCapacity));

	GFASSERT(newData, "Could not get contiguous memory for vector");

	for (u64 i = 0; i < mSize; i++) {
		newData[i] = std::move(mData[i]);
		mData[i].~T();
	}

	mData = newData;
	mCapacity = newCapacity;
}

template<class T>
inline void vector<T>::reserve(const u64 newCapacity)
{
	if (newCapacity > capacity()) {
		GFASSERT(newCapacity < max_size(), "Cannot allocate larger than max allocation size");
		reallocate(newCapacity);
	}
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
	emplace_back(value);
}

template<class T>
inline void vector<T>::push_back(T&& value)
{
	emplace_back(std::move(value));
}

template<class T>
inline void vector<T>::pop_back()
{
	GFASSERT(mSize > 0, "Attempting pop on empty vector");
	mData[mSize - 1].~T();
	mSize--;
}

template<class T>
inline T& vector<T>::operator[](u64 index) noexcept
{
	GFASSERT(index < mSize, "Index out of range");
	return mData[index];
}

} // namespace gf3d