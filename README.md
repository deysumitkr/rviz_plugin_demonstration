# Rviz Plugin - A panel for robot demonstrations
A panel for rviz to obtain demonstrations. Primarily developed with PR2 in consideration but certainly not constrained to any particular platform.

### Steps
* Clone this repo in `src` directory of catkin workspace
* Build the catkin workspace: `$ catkin_make` from root of catkin workspace
Now the panel should be available for use.
* run rviz: `$ rviz`
* To bring up the panel: `Panels > Add New Panel > Demonstration Panel`

### How to use
* Subscribe to topic `/rviz/demonstration_panel`
* It uses a custom message type `lfd` defined within this package. Details regarding using custom ros messages can be found [here].

### Screenshot
![Panel Screenshot][screenshot]
[screenshot]: screenshot.png

[here]: http://wiki.ros.org/ROS/Tutorials/DefiningCustomMessages



