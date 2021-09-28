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

	typedef T* iterator;
	typedef const T* const_iterator;

	inline T* data() const noexcept { return mData; }
	inline u64 capacity() const noexcept { return mCapacity; }
	inline u64 size() const noexcept { return mSize; }
	inline u64 max_size() const noexcept { return 0xffffffffffffffff / sizeof(T); }
	inline iterator begin() noexcept { return &mData[0]; }
	inline iterator end() noexcept { return &mData[mSize]; }
	inline iterator begin() const noexcept { return &mData[0]; }
	inline iterator end() const noexcept { return &mData[mSize]; }

	void reserve(const u64 newCapacity);
	void resize(const u64 newSize);
	void clear() noexcept;
	void push_back(const T& value);
	void push_back(T&& value);
	void pop_back();
	void shrink_to_fit();
	void swap(vector<T>& other);
	T& operator[](u64 index) noexcept;

	template<typename... Args>
	T& emplace_back(Args&&... args)
	{
		u64 newSize = mSize + 1;
		
		if (newSize > mCapacity) {

			u64 growth = mCapacity + mCapacity / 2;
			if (growth < newSize)
				reallocate(newSize);
			else
				reallocate(growth);
		}

		new(&mData[mSize]) T(static_cast<Args&&>(args)...);
		mSize = newSize;
		return mData[mSize - 1];
	}


private:
	void reallocate(const u64 newCapacity);

private:
	template<typename... Args>
	inline void construct_copy_in_place(T& object, Args&&... args)
	{
		::new (reinterpret_cast<void*>(&object)) T(static_cast<Args&&>(args)...);
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
		::new (&newData[i]) T(static_cast<T&&>(mData[i]));
		mData[i].~T();
	}

	::operator delete(mData);

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
inline void vector<T>::resize(const u64 newSize)
{
	if (newSize < mSize) { // trim
		T* newData = reinterpret_cast<T*>(::operator new(sizeof(T) * newSize));
		
		for (u64 i = 0; i < newSize; i++) // Move elements needed
			::new (&newData[i]) T(static_cast<T&&>(mData[i]));

		clear();

		::operator delete(mData, sizeof(T) * mCapacity);

		mData = newData;
		mCapacity = newSize;
		mSize = newSize;
		return;
	}

	if (newSize > mSize) { // append to current vector

		if (newSize > mCapacity) {
			reallocate(newSize); // reallocates and moves old data into new vector
		}
		for (; mSize < newSize; mSize++) {
			::new (&mData[mSize]) T();
		}
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
inline void vector<T>::shrink_to_fit()
{
	if (mCapacity > mSize) {
		reallocate(mSize);
	}
}

template<class T>
inline void vector<T>::swap(vector<T>& other)
{
	if (this != &other) {

		vector<T> temp = other;
		other.mData = mData;
		other.mCapacity = mCapacity;
		other.mSize = mSize;

		mSize = temp.mSize;
		mCapacity = temp.mCapacity;
		mData = temp.mData;

		temp.mData = nullptr;
		temp.mSize = 0;
		temp.mCapacity = 0;
	}
}

template<class T>
inline T& vector<T>::operator[](u64 index) noexcept
{
	GFASSERT(index < mSize, "Index out of range");
	return mData[index];
}

} // namespace gf3d