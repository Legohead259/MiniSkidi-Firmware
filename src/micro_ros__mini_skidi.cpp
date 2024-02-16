#include "micro_ros__mini_skidi.h"

rcl_allocator_t allocator;
rclc_support_t support;
rcl_node_t node;
rclc_executor_t executor;
TaskHandle_t MicroROSTask;
AgentState agentState = WAITING_AGENT;

bool createPublishers() {
    return true;
}

bool createServices() {
    return true;
}

bool addServices() {
    return true; 
}

bool createTimers() {
    return true;
}

bool addTimers() {
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
    createServices();
    createTimers();

    // Create executor
    RCCHECK(rclc_executor_init(&executor, &support.context, 10+RCLC_EXECUTOR_PARAMETER_SERVER_HANDLES, &allocator));
    addServices();
    addTimers();

    initializeParameterService();

    return true;
}

void destroyEntities() {
    rmw_context_t * rmw_context = rcl_context_get_rmw_context(&support.context);
    (void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);

    RCSOFTCHECK(rclc_executor_fini(&executor));
    RCSOFTCHECK(rcl_node_fini(&node));
    RCSOFTCHECK(rclc_support_fini(&support));
}

void microROSTaskCallback(void* parameters) {
    for(;;) {
        // Handle Micro-ROS tasking
        switch (agentState) {
            case WAITING_AGENT:
                EXECUTE_EVERY_N_MS(500, agentState = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_AVAILABLE : WAITING_AGENT;);
                break;

            case AGENT_AVAILABLE:
                agentState = createEntities() ? AGENT_CONNECTED : WAITING_AGENT; // Check if entities are properly created
                if (agentState == AGENT_CONNECTED) { // Update system state
                    motor.enable();
                }
                if (agentState == WAITING_AGENT) { // If entities are not properly created, destroy them
                    destroyEntities();
                    motor.disable();
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
                motor.disable();
                break;
                
            default:
                break;
        }
    }
}