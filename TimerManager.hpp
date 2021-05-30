#pragma once
#include <chrono>
#include <functional>
#include <map>

struct Timer {
	std::chrono::high_resolution_clock::time_point start;
	std::chrono::milliseconds delay{};
	std::function<void()> function;
};

class TimerManager
{
	std::map<std::string, Timer> m_TimerMap;
	std::chrono::high_resolution_clock::time_point m_CurrentTime;
	std::vector<std::string> m_CurrentTimers;
public:
	void updateTimers();
	bool addTimer(const std::string& name, std::chrono::milliseconds msdelay, const std::function<void()>& function);
	void removeTimer(const std::string& name);
};

