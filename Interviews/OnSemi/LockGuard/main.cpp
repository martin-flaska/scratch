#include <iostream>
#include "LockGuard.h"
#include <thread>
#include <chrono>
#include <condition_variable>

namespace {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string g2_s = ":";
std::mutex g2_m;

void thread_task2(const std::string &s)
{
	Lock::LockGuard lg(g2_m);

	g2_s.append("<").append(s).append(">");

	std::cout << "thread_task: g_s: " << g2_s << std::endl;
}

// Task 2:
//   * Implement c++11 threading example
//   * 2-3 threads running
void task2()
{
	std::cout << "Task 2 start..." << std::endl;

	std::thread (thread_task2, "Thread 1").detach();
    std::thread (thread_task2, "Thread 2").detach();
	std::thread (thread_task2, "Thread 3").detach();

	std::this_thread::sleep_for (std::chrono::seconds(2));

	std::cout << "Task 2 end." << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Task 3: Use c++11 std::condition_signal in solution from #3 to synchronize threads
// 	-- Psuedo Code --
// 	main:
// 	1) start: thread 1, thread 2, thread 3
// 	2) all threads block on condition_signal
// 	3) main signals condition_signal
// 	4) join all threads to prevent exit main // gimmee
//
// 	thread 1:
// 		break from condition_signal due to step 3
// 		sleep (random(1,5))
// 		signal thread 2
// 		wait on condition_signal
//
// 	example output:
// 		main: starting all threads
// 		thread1: starting, waiting.
// 		thread2: starting, waiting.
// 		thread3: starting, waiting.
// 		main: starting thread 1.
// 		thread1: signal received, doing work ....
// 		thread1: done with work, signal next thread
// 		thread2: signal received, doing work ....
// 		thread2: done with work, signal next thread
//
// 		on and on and on until you CTRL-C the program
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Notes/My comments:
// * IDK nothing about "std::condition_signal", but "std::condition_variable"
//   https://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=condition_signal&button=
// * "Use c++11 std::condition_signal in solution from #3", what solution #3? did you mean, Task 2 or Task 1 instead?
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::mutex g3_m;
std::condition_variable g3_cv;
int ready = 0; // number of the thread to be ready (1-3, 0: none)

void thread_task3(int n)
{
	while (true) {
		std::cout << "Thread " << n << " starting, waiting." << std::endl;
		std::unique_lock<std::mutex> lk(g3_m);
		g3_cv.wait(lk, [&n] {
			//std::cout << "Thread " << n << " wait check. ready: " << ready << std::endl;
			return (ready == n);
		});

		std::cout << "Thread " << n << " signal received, doing work..." << std::endl;

		// sleep 1-5 secs
		std::this_thread::sleep_for(std::chrono::seconds(1 + std::rand() % 4));

		ready = 1 + (n % 3); // number of the next thread (1, 2, 3, 1, 2, 3, ...)

		std::cout << "Thread " << n << " done with work, signal next thread: " << ready << std::endl;

		lk.unlock();
		g3_cv.notify_all();
	}
}

void task3()
{
	std::cout << "Task 3 start..." << std::endl;
	std::thread thread1(thread_task3, 1);
	std::thread thread2(thread_task3, 2);
	std::thread thread3(thread_task3, 3);

	std::this_thread::sleep_for(std::chrono::seconds(1));

    {
		// Task 4:
		// 4) Use your implementation of c++11 lock_guard (NOT std::lock_guard)
		// to protect a common resource in your threading example from #3 and #4
        Lock::LockGuard lk(g3_m);
		ready = 1; // Thread 1
        std::cout << "Task 3: signals ready for the Thread 1" << std::endl;
    }
    g3_cv.notify_all();

	std::cout << "Task 3: Waiting for finishing all threads..." << std::endl;

	thread1.join();
	thread2.join();
	thread3.join();

	std::cout << "Task 3 end." << std::endl; // TODO: handlle ctrl+c event to correctly end the task 3 (as well as all threads)
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

} // end of anonymous ns


int main(int argc, char **argv)
{
	task2();

	task3();

    return 0;
}
