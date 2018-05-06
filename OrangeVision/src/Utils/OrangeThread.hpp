#pragma once
#ifndef ORANGE_THREAD_HPP
#define ORANGE_THREAD_HPP
#include <atomic>
#include <condition_variable>

template <class T>
class OrangeThread {
public:

	OrangeThread(std::shared_ptr<T> functor) {
		m_functor = functor;
		m_thread = std::make_unique<std::thread>(&OrangeThread::update, this);
	}

	~OrangeThread() {
		release();
	}



	void start() {
		isRunning.store(true, std::memory_order_release);
		m_signal.notify_all();
	}

	void stop() {
		isRunning.store(false, std::memory_order_release);
	}	

private:

	void update() {
		while (isAlive.load(std::memory_order_acquire)) {
			if (isRunning.load(std::memory_order_acquire)) {
				m_functor->update();
			} else {
				std::unique_lock<std::mutex> lock(m_signalLock);
				m_signal.wait_for(lock, std::chrono::milliseconds(200), [this]() {return isRunning.load(std::memory_order_acquire); });
			}
		}
	}

	void release() {
		isAlive.store(false, std::memory_order_release);
		m_thread->join();
	}

	std::mutex m_signalLock;
	std::condition_variable m_signal;
	std::atomic<bool> isAlive = true;
	std::atomic<bool> isRunning = true;
	std::unique_ptr<std::thread> m_thread;
	std::shared_ptr<T> m_functor;
};

#endif