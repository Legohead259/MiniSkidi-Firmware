#ifndef MICRO_ROS__MINI_SKIDI_H
#define MICRO_ROS__MINI_SKIDI_H

#include "micro_ros__definitions.h"
#include "micro_ros__parameters.h"
#include "micro_ros__pubsub.h"
#include <mini_skidi/MiniSkidi.h>

extern TaskHandle_t MicroROSTask;

bool createPublishers();
bool createSubscribers();
bool addSubscribers();
bool createServices();
bool addServices();
bool createTimers();
bool addTimers();
bool createEntities();
void destroyEntities();
void microROSTaskCallback(void* parameters);

#endif // MICRO_ROS__MINI_SKIDI_H