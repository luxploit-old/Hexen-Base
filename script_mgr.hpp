#pragma once
#include "script.hpp"
#include <mutex>

class script_mgr {
public:
	void add_script(std::unique_ptr<script> script);
	void remove_all_scripts();
	void tick();
private:
	std::recursive_mutex m_mutex;
	std::vector<std::unique_ptr<script>> m_scripts;
};