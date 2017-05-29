#include "gtest/gtest.h"
#include "autil_manager.hpp"

class AudioManagerAPITestFixture : public ::testing::Test 
{
public:
	AudioManager audioManager;
protected:
	void SetUp() {}
	void TearDown() {}
};

class TestEventSink 
: public APUHostEventSink
{
public:
	TestEventSink() {
		refCount = 0;
	}
	virtual void handleEvent(const char *eventName, APUHostInterface *sender) {
		/* empty */
	}

	int refCount;
	int addRef() { return ++refCount; }
	int decRef() { return --refCount; }
};

// TEST_F(AudioManagerAPITestFixture, test_get_devices)
// {
// 	APUPtr<APUEnumerable<AudioDevice> > devices = audioManager.getDevices();
// 	ASSERT_TRUE((bool)devices); //Expect not null
// 	devices->reset();
// 	ASSERT_TRUE((bool)devices->getCurrent());
// 	do {
// 		APUPtr<AudioDevice> dev = devices->getCurrent();
// 		EXPECT_TRUE(dev->getName() != "");
// 		EXPECT_TRUE(dev->getIndex() >= 0);
// 	} while (devices->moveNext());
// }

// TEST_F(AudioManagerAPITestFixture, test_input_mode)
// {
// 	audioManager.setInputMode(AudioInputModeFile);
// 	EXPECT_EQ(-1, audioManager.getInputDevice());
// 	APUPtr<APUEnumerable<AudioDevice> > devices = audioManager.getDevices();
// 	devices->reset();
// 	audioManager.setInputMode(AudioInputModeDevice);
// 	audioManager.setInputDevice(devices->getCurrent()->getIndex());
// 	EXPECT_EQ(devices->getCurrent()->getIndex(), audioManager.getInputDevice());
// }

TEST_F(AudioManagerAPITestFixture, test_event_sink)
{
	APUPtr<TestEventSink> sink = new TestEventSink();
	audioManager.subscribe(sink);
	EXPECT_EQ(2, sink->refCount);
	audioManager.unsubscribe(sink);
	EXPECT_EQ(1, sink->refCount);
}
