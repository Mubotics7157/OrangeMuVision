#pragma once
#ifndef ORANGE_THREAD_HPP
#define ORANGE_THREAD_HPP
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <memory>
#include "Updateable.hpp"
#include "ConcurrentStream.hpp"

namespace ov {
	class OrangeThread {
	public:
		OrangeThread(const std::vector<std::shared_ptr<Updateable>>& updater);
		OrangeThread(std::vector<std::shared_ptr<Updateable>>&& updater);
		~OrangeThread();
		void start();
		void stop();
		void setUpdateable(const std::vector<std::shared_ptr<Updateable>>& updater);
		void setUpdateable(std::vector<std::shared_ptr<Updateable>>&& updater);
	private:
		void update();
		void release();
		std::mutex m_signalLock;
		std::condition_variable m_signal;
		std::atomic<bool> isAlive{true};
		std::atomic<bool> isRunning{true};
		std::unique_ptr<std::thread> m_thread{ nullptr };
		ConcurrentStream<std::vector<std::shared_ptr<Updateable>>> m_updaters;
	};
}
#endif