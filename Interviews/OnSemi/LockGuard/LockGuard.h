
#pragma once

#include <mutex>


namespace Lock {

/**
 * std::lock_guard re-implementation - without using template (uses std::mutex internally)
 *
 * (Task 1)
 */
class LockGuard {
public:
	explicit LockGuard(std::mutex &mutex)
		: m_mutext(mutex)
	{
		m_mutext.lock();
	}

	~LockGuard()
	{
		m_mutext.unlock();
	}

private:
	std::mutex &m_mutext;
};

} // Lock namespace
