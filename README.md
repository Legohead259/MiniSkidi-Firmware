# MiniSkidi-Firmware

Version 1.0

Forked from: Professor Boots [3D Printed SkidSteer V2.0](https://github.com/ProfBoots/V2.0-3D-Printed-RC-SkidSteer)

## Programming the MiniSkidi

Plug into computer using USB cable.
If the programming computer is using the WSL2 application, see [the official guide](https://learn.microsoft.com/en-us/windows/wsl/connect-usb) about mounting a USB device to WSL2.
The Mini Skidi may appear in the USBIPD list as `CP2102 USB to UART Bridge Controller`
Once the device is mounted, upload the firmware to the MiniSkidi using the PlatformIO tools within VS Code or through CLI:

```[bash]
pio run -e mini_skidi_rev2
```

## Connecting to ROS2 Agent

Note: this assumes that the ROS2 workspace has already been installed and configured on the host machine.
For more information, refer to the README within the [MiniSkidi-ROS2 repository](https://github.com/Legohead259/MiniSkid-ROS2)
Within the WSL2 application, open the MiniSkidi ROS2 workspace.
If not already done, clone the latest version of the `micro_ros_setup` repository using:

```[bash]
git clone -b $ROS_DISTRO https://github.com/micro-ROS/micro_ros_setup.git src/micro_ros_setup
```

To update the repository, migrate into the `src/micro_ros_setup/` directory and execute:

```[bash]
git pull
```

Then, rebuild the ROS2 environment using the following command from the root directory of the ROS2 workspace:

```[bash]
colcon build && source install/local_setup.bash
```

### Creating the micro-ROS Agent

To create an agent to start talking to the ROS2 workspace, call the following within the `src/` folder of the workspace:

```[bash]
ros2 run micro_ros_setup create_agent_ws.sh
```

Then, build the agent packages and source ine installation:

```[bash]
ros2 run micro_ros_setup build_agent.sh && source install/local_setup.bash
```

### Running the micro-ROS Agent

In order to give the micro-ROS application access to the ROS2 dataspace, the micro-ROS agent needs to be brought up.

#### Serial (USB)

For the USB interface, use the command `ls /dev/ttyUSB*` to identify the name of the USB port connected to the MiniSkidi.
Then, call the following:

```[bash]
ros2 run micro_ros_agent micro_ros_agent serial --baudrate 115200 --dev [USB device name]
```

#### Network (UDP)

When the MiniSkidi connects to the ROS2 workspace over a UDP/IP network connection, call the following:

```[bash]
ros2 run micro_ros_agent micro_ros_agent udp4 --port [MiniSkidi Port] --address [MiniSkidi address]
```

The port and IP address for the network connection can be configured using the MiniSkidi parameter service.

### Checkpoint

At this point, we can check that the MiniSkidi is available to ROS2 by opening a new terminal in the WSL2 application and running the following command:

```[bash]
ros2 node list
```

If the output includes `/mini_skidi_node`, then the node is present.
We can further check functionality by calling `ros2 topic list` and `ros2 service list` to verify the expected topics and services are present.

### Connecting to FoxGlove Studio

Setup the Foxglove ROS2 bridge according to the [official guide](https://docs.foxglove.dev/docs/connecting-to-data/frameworks/ros2/).
Then, bring up the Foxglove bridge using the following command:

```[bash]
ros2 launch foxglove_bridge foxglove_bridge_launch.xml
```

This will open the Foxglove bridge using the port 8765.
From here, open a new terminal and find the WSL2 IP address using `ip addr | grep eth0`.
Copy the IP address in the `inet` field (e.g. 172.17.13.133) and navigate to the Foxglove web dashboard.
Sign into your Foxglove account and select `Open connection...` in the left-hand menu bar.
In the Foxglove WebSocket option, replace the URL with your WSL2 IP address and port (e.g. 172.17.13.133:8765).
You should now see a dashboard that is connected to the ROS2 environment and you should see activity within the Foxglove Bridge terminal.
