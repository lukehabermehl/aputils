#include "gtest/gtest.h"
#include "apu_thread_manager.hpp"

class ThreadManagerTestFixture : public ::testing::Test 
{
public:
	int GetInt() {
		int ret = 0;
		mtx.lock();
		ret = value;
		mtx.unlock();
		return value;
	}
	void Increment() {
		mtx.lock();
		value += 1;
		mtx.unlock();
	}

	static void ThreadWokerFunc(void *ctx) {
		ThreadManagerTestFixture *fixture = (ThreadManagerTestFixture *)ctx;
		fixture->Increment();
	}

protected:
	std::mutex mtx;
	int value;

	void SetUp() {
		value = 0;
	}
};

TEST_F(ThreadManagerTestFixture, test_thread_dispatch)
{
	int count = 4;
	for (int i=0; i<count; i++) {
		APUDispatchThread(ThreadManagerTestFixture::ThreadWokerFunc, this);
	}

	sleep(2);
	EXPECT_EQ(count, GetInt());
}

TEST_F(ThreadManagerTestFixture, test_shutdown)
{
	for (int i = 0; i < 4; i++) {
		APUDispatchThread(ThreadManagerTestFixture::ThreadWokerFunc, this);
	}

	APUThreadManager::sharedThreadManager()->shutdown();
	EXPECT_TRUE(APUThreadManager::sharedThreadManager()->isShutdown());
}
