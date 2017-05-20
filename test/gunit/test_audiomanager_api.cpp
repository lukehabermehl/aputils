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

TEST_F(AudioManagerAPITestFixture, test_get_devices)
{
	AudioDeviceInfoRef deviceInfo = audioManager.getDevices();
	EXPECT_TRUE((bool)deviceInfo); //Expect not null

	while (deviceInfo) {
		EXPECT_TRUE(deviceInfo->name != "");
		EXPECT_TRUE(deviceInfo->index >= 0);
		deviceInfo = deviceInfo->next;
	}

}

TEST_F(AudioManagerAPITestFixture, test_input_mode)
{
	audioManager.setInputMode(AudioInputModeFile);
	EXPECT_EQ(-1, audioManager.getInputDevice());
	AudioDeviceInfoRef deviceInfo = audioManager.getDevices();
	audioManager.setInputMode(AudioInputModeDevice);
	audioManager.setInputDevice(deviceInfo->index);
	EXPECT_EQ(deviceInfo->index, audioManager.getInputDevice());
}

TEST_F(AudioManagerAPITestFixture, test_event_sink)
{
	APUPtr<TestEventSink> sink = new TestEventSink();
	audioManager.subscribe(sink);
	EXPECT_EQ(2, sink->refCount);
	audioManager.unsubscribe(sink);
	EXPECT_EQ(1, sink->refCount);
}
