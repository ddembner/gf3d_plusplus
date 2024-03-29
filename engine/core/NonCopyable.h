#pragma once

class NonCopyable
{
protected:
	NonCopyable() = default;
	virtual ~NonCopyable() = default;
public:
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
	NonCopyable(NonCopyable&&) noexcept = default;
	NonCopyable& operator=(NonCopyable&&) noexcept = default;
};