#include "uros/micro_ros__mini_skidi.h"

TaskHandle_t MicroROSTask;
rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;
MiniSkidi* miniSkidiPtr;
AgentState agentState = WAITING_AGENT;

bool createPublishers() {
    RCCHECK(rclc_publisher_init_default(
        &skidiControlPublisher,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Bool),
        "/skidi_control"));

    return true;
}

bool createSubscribers() {
    RCCHECK(rclc_subscription_init_best_effort(
        &joySubscriber,
        &node,
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Joy),
        "joy"));

    // Initialize subscriber message memory
    micro_ros_utilities_memory_conf_t conf = {
        .max_ros2_type_sequence_capacity = 20,
        .max_basic_type_sequence_capacity = 20,
    };
    micro_ros_utilities_create_message_memory(
        ROSIDL_GET_MSG_TYPE_SUPPORT(sensor_msgs, msg, Joy),
        &joystickMsg,
        conf);
    
    return true;
}

bool addSubscribers() {    
    RCCHECK(rclc_executor_add_subscription(
        &executor, 
        &joySubscriber, 
        &joystickMsg, 
        &joystickCallback, 
        ON_NEW_DATA));
    
    return true;
}

bool createServices() {
    RCCHECK(rclc_parameter_server_init_with_option(
        &parameterService, 
        &node,
        &parameterServiceOpts));
    
    return true;
}

bool addServices() {
    RCCHECK(rclc_executor_add_parameter_server(
        &executor, 
        &parameterService, 
        onParameterChangedCallback));
    
    return true; 
}

bool createTimers() {
    RCCHECK(rclc_timer_init_default(
        &skidiControlTimer,
        &support,
        RCL_MS_TO_NS(1000),
        skidiControlCallback));

    return true;
}

bool addTimers() {
    RCCHECK(rclc_executor_add_timer(
        &executor, 
        &skidiControlTimer));

    return true;
}

bool createEntities() {
    // Get the default memory allocator provided by rcl
    allocator = rcl_get_default_allocator();

    // Initialize rclc_support with default allocator
    RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

    // Initialize a ROS node with the name "mini_skidi_node"
    RCCHECK(rclc_node_init_default(&node, "mini_skidi_node", "", &support));

    // Create application components
    createPublishers();
    createSubscribers();
    createServices();
    createTimers();

    // Create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 10+RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES, &allocator));
    addSubscribers();
    addServices();
    addTimers();

    // initializeParameterService();

    return true;
}

void destroyEntities() {
    rmw_context_t * rmw_context = rcl_context_get_rmw_context(&support.context);
    (void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

    RCSOFTCHECK(rclc_parameter_server_fini(&parameterService, &node));
    RCSOFTCHECK(rclc_executor_fini(&executor));
    RCSOFTCHECK(rcl_node_fini(&node));
    RCSOFTCHECK(rclc_support_fini(&support));
}

void microROSTaskCallback(void* parameter) {
    miniSkidiPtr = (MiniSkidi*) parameter;

    for(;;) {
        // Handle Micro-ROS tasking
        switch (agentState) {
            case WAITING_AGENT:
                EXECUTE_EVERY_N_MS(500, agentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);
                break;

            case AGENT_AVAILABLE:
                agentState = createEntities() ? AGENT_CONNECTED : WAITING_AGENT; // Check if entities are properly created
                if (agentState == AGENT_CONNECTED) { // Update system state
                    miniSkidiPtr->enableMotors();
                    digitalWrite(LED_BUILTIN, HIGH);
                }
                if (agentState == WAITING_AGENT) { // If entities are not properly created, destroy them
                    destroyEntities();
                    miniSkidiPtr->disableMotors();
                    digitalWrite(LED_BUILTIN, LOW);
                };
                break;
            
            case AGENT_CONNECTED:
                EXECUTE_EVERY_N_MS(200, agentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
                if (agentState == AGENT_CONNECTED) {
                    // Execute pending tasks in the executor. This will handle all ROS communications.
                    RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100)));
                }
                break;

            case AGENT_DISCONNECTED:
                destroyEntities();
                agentState = WAITING_AGENT;
                miniSkidiPtr->disableMotors();
                digitalWrite(LED_BUILTIN, LOW);
                break;
                
            default:
                break;
        }
    }
}