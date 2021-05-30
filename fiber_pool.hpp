#pragma once
#include <stack>
#include <functional>


class fiber_pool
{
public:
	void add_number_of_fibers(size_t nums);
	void queue_job(std::function<void()> func);

	void fiber_tick();
	static void fiber_func();
private:
	std::recursive_mutex m_mutex;
	std::stack<std::function<void()>> m_jobs;
};