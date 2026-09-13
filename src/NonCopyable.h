#pragma once

class NonCopyable
{
public:

	NonCopyable() = default;

	//Disable copy
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

	//Disbale move
	NonCopyable(NonCopyable&&) = delete;
	NonCopyable& operator=(NonCopyable&&) = delete;
};