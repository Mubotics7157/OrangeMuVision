#pragma once
#ifndef ORANGE_THREAD_HPP
#define ORANGE_THREAD_HPP
#include <atomic>
#include <condition_variable>
#include <vector>
#include "Threading\Updateable.hpp"

class OrangeThread {
public:
	OrangeThread(const std::vector<std::shared_ptr<Updateable>>& updater);
	OrangeThread(std::vector<std::shared_ptr<Updateable>>&& updater);
	~OrangeThread();
	void start();
	void stop();
private:
	void update();
	void release();
	std::mutex m_signalLock;
	std::condition_variable m_signal;
	std::atomic<bool> isAlive = true;
	std::atomic<bool> isRunning = true;
	std::unique_ptr<std::thread> m_thread{ nullptr };
	std::vector<std::shared_ptr<Updateable>> m_updaters;
};

#endif