#pragma once
#include <chrono>
class Timer
{
public:
	Timer()
	{
		start = std::chrono::steady_clock::now();
	}
	~Timer() = default;
public:
	void Reset()
	{
		start = std::chrono::steady_clock::now();
	}
	long long ElapsedMilliseconds()
	{
		const auto temp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(std::chrono::steady_clock::now() - start)).count();
		Reset();
		return temp;
	}
	long long ElapsedSeconds()
	{
		const auto temp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::duration<float>(std::chrono::steady_clock::now() - start)).count();
		Reset();
		return temp;
	}

	long long ElapsedNanoseconds()
	{
		const auto temp = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration<float>(std::chrono::steady_clock::now() - start)).count();
		Reset();
		return temp;
	}

	void Stop()
	{
		end = std::chrono::steady_clock::now();
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::steady_clock::time_point end;
};