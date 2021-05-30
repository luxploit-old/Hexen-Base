#pragma once
#include "TimerManager.hpp"

void TimerManager::updateTimers() {
	m_CurrentTime = std::chrono::high_resolution_clock::now();
	for(auto& timer : m_CurrentTimers) {
		const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(m_CurrentTime - m_TimerMap[timer].start);
		if (elapsed > m_TimerMap[timer].delay) {
            if(m_TimerMap[timer].function)
			    m_TimerMap[timer].function();
			m_TimerMap[timer].start = m_CurrentTime;
		}
	}
}

bool TimerManager::addTimer(const  std::string& name, const std::chrono::milliseconds msdelay, const std::function<void()>& function) {
	if (m_TimerMap[name].function) {
	    m_TimerMap[name].delay = msdelay;
	    m_TimerMap[name].function = function;
	}
    else {
        m_TimerMap[name].delay = msdelay;
        m_TimerMap[name].function = function;
        m_TimerMap[name].start = std::chrono::high_resolution_clock::now();
        m_CurrentTimers.push_back(name);
    }
	return true;
}

void TimerManager::removeTimer(const std::string& name) {
	auto it = m_CurrentTimers.begin();
	for (size_t i = 0; i < m_CurrentTimers.size(); i++) {
		if (name == m_CurrentTimers[i]) {
			std::advance(it, i);
			m_CurrentTimers.erase(it);
			m_TimerMap[name].function = nullptr;
		}
	}
}