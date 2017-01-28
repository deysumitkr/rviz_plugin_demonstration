#ifndef RVIZ_PLUGIN_H
#define RVIZ_PLUGIN_H

#include <ros/ros.h>
#include <rviz/panel.h>

#include "rviz_plugins/lfd.h"

extern bool EXIT, SIGNAL_PLAN, SIGNAL_EXECUTE, GRIPPER_OPEN, GRIPPER_CLOSE;

void rviz_plugin_callback(const rviz_plugins::lfdConstPtr& msg);

class Demonstration_Panel: public rviz::Panel{
	Q_OBJECT
	public:
	
	Demonstration_Panel( QWidget* parent = 0 );
	
	protected Q_SLOTS:
		void triggerPlanning();
		void triggerExecution();
		void triggerExit();
		void openGripper();
		void closeGripper();
		void updateTopic();
		void setTiltAngle(int);
		void setHeadTilt();
		void setPanAngle(int);
		void setHeadPan();
		
		
	protected:
		ros::NodeHandle node_handle;
		ros::Publisher pub;
		ros::Subscriber subs;
	};
	
#endif
