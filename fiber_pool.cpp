#include "ClassPointers.hpp"
#include "fiber_pool.hpp"
#include "script.hpp"
#include "script_mgr.hpp"

void fiber_pool::add_number_of_fibers(std::size_t num_fibers) {
	for (std::size_t i = 0; i < num_fibers; ++i) {
		ClassPointers::cScripts->add_script(std::make_unique<script>(&fiber_func));
	}
}

void fiber_pool::queue_job(std::function<void()> func) {
	if (func) {
		std::lock_guard lock(m_mutex);
		m_jobs.push(std::move(func));
	}
}

void fiber_pool::fiber_tick() {
	std::unique_lock lock(m_mutex);
	if (!m_jobs.empty()) {
		auto job = std::move(m_jobs.top());
		m_jobs.pop();
		lock.unlock();

		std::invoke(std::move(job));
	}
}

void fiber_pool::fiber_func() {
	while (true) {
		ClassPointers::cPool->fiber_tick();
		script::get_current()->yield();
	}
}