#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>

#include "rviz_plugin.h"
#include "rviz_plugins/lfd.h"

//===================================

bool EXIT = false;
bool SIGNAL_EXECUTE = false;
bool SIGNAL_PLAN = false;

bool GRIPPER_OPEN = false;
bool GRIPPER_CLOSE = false;

bool HEAD_TILT_ACT = false;
float HEAD_TILT_ANGLE = 0.0;

bool HEAD_PAN_ACT = false;
float HEAD_PAN_ANGLE = 0.0;

//===================================

Demonstration_Panel::Demonstration_Panel( QWidget* parent ) : rviz::Panel( parent ) {
	QVBoxLayout* container_layout = new QVBoxLayout;
	//container_layout->addWidget( new QLabel( "Right Arm Demonstration:" ));
	
	
	//Row 1 - Plan and Execute
	QPushButton* btn_plan = new QPushButton(tr("Animate Plan"));
	QPushButton* btn_exec = new QPushButton(tr("Execute Move"));
	
	QHBoxLayout* btn_row_layout = new QHBoxLayout;
	btn_row_layout->addWidget(btn_plan);
	btn_row_layout->addWidget(btn_exec);
	container_layout->addLayout(btn_row_layout);
	
	
	//Row 2 - Gripper
	QPushButton* btn_release = new QPushButton(tr("Gripper Open"));
	QPushButton* btn_grasp = new QPushButton(tr("Gripper Close"));
	
	QHBoxLayout* btn_row_layout2 = new QHBoxLayout;
	btn_row_layout2->addWidget(btn_release);
	btn_row_layout2->addWidget(btn_grasp);
	container_layout->addLayout(btn_row_layout2);
	
	
	//Row 3 - Head Tilt 
	QHBoxLayout* head_tilt_row_layout = new QHBoxLayout;
	head_tilt_row_layout->addWidget( new QLabel( "Head Tilt:" ));
	QSlider* head_tilt_angle = new QSlider(Qt::Horizontal);
	QPushButton* btn_set_head_tilt = new QPushButton(tr("Set Tilt"));
	head_tilt_angle->setRange(0, 100);
	head_tilt_row_layout->addWidget(head_tilt_angle);
	head_tilt_row_layout->addWidget(btn_set_head_tilt);
	container_layout->addLayout(head_tilt_row_layout);
	
	
	//Row 4 - Head Pan
	QHBoxLayout* head_pan_row_layout = new QHBoxLayout;
	head_pan_row_layout->addWidget( new QLabel( "Head Pan:" ));
	QSlider* head_pan_angle = new QSlider(Qt::Horizontal);
	QPushButton* btn_set_head_pan = new QPushButton(tr("Set Pan"));
	head_pan_angle->setRange(0, 100);
	head_pan_row_layout->addWidget(head_pan_angle);
	head_pan_row_layout->addWidget(btn_set_head_pan);
	container_layout->addLayout(head_pan_row_layout);
	
	
	//Row 5 - Node Shutdown
	QPushButton* btn_exit = new QPushButton(tr("Node Shutdown"));
	container_layout->addWidget(btn_exit);
	
	setLayout(container_layout);
		
	
	QTimer* output_timer = new QTimer( this );
	
	connect(btn_plan, SIGNAL (released()), this, SLOT (triggerPlanning()));
	connect(btn_exec, SIGNAL (released()), this, SLOT (triggerExecution()));
	connect(btn_release, SIGNAL (released()), this, SLOT (openGripper()));
	connect(btn_grasp, SIGNAL (released()), this, SLOT (closeGripper()));
	
	connect(head_tilt_angle, SIGNAL(valueChanged(int)), this, SLOT(setTiltAngle(int)));
	connect(btn_set_head_tilt, SIGNAL (released()), this, SLOT (setHeadTilt()));
	
	connect(head_pan_angle, SIGNAL(valueChanged(int)), this, SLOT(setPanAngle(int)));
	connect(btn_set_head_pan, SIGNAL (released()), this, SLOT (setHeadPan()));
	
	connect(btn_exit, SIGNAL (released()), this, SLOT (triggerExit()));
	
	connect( output_timer, SIGNAL( timeout() ), this, SLOT( updateTopic() ));

	output_timer->start( 100 );
	
	pub = node_handle.advertise<rviz_plugins::lfd>("/rviz/demonstration_panel", 10);
	subs = node_handle.subscribe("/rviz/demonstration_panel", 10, rviz_plugin_callback);
	}
	
void rviz_plugin_callback(const rviz_plugins::lfdConstPtr& msg){
	EXIT = msg->exit;
	SIGNAL_PLAN = msg->plan;
	SIGNAL_EXECUTE = msg->execute;
	GRIPPER_OPEN = msg->gripper_open;
	GRIPPER_CLOSE = msg->gripper_close;
	HEAD_TILT_ANGLE = msg->head_tilt_angle;
	HEAD_TILT_ACT = msg->head_tilt_action;
	HEAD_PAN_ANGLE = msg->head_pan_angle;
	HEAD_PAN_ACT = msg->head_pan_action;
}
	
void Demonstration_Panel::triggerPlanning(){
	SIGNAL_PLAN = true;
	ROS_INFO("Planning Animation");
}

void Demonstration_Panel::triggerExecution(){
	SIGNAL_EXECUTE = true;
	ROS_INFO("Executing Action");
}

void Demonstration_Panel::triggerExit(){
	EXIT = true;
	ROS_INFO("Exiting Node");
}

void Demonstration_Panel::openGripper(){
	GRIPPER_OPEN = true;
	ROS_INFO("Gripper Open");
}

void Demonstration_Panel::closeGripper(){
	GRIPPER_CLOSE = true;
	ROS_INFO("Gripper Close");
}

void Demonstration_Panel::setTiltAngle(int angle){
	HEAD_TILT_ANGLE = (((float(angle) - 0.0)/(100.0-0.0))*(1.29 + 0.37)) - 0.37; 
	//std::cout << angle << " -> "<< newAngle << "\n";
}

void Demonstration_Panel::setHeadTilt(){
	HEAD_TILT_ACT = true;
	ROS_INFO("Move Head");
}

void Demonstration_Panel::setPanAngle(int angle){
	HEAD_PAN_ANGLE = (((float(angle) - 0.0)/(100.0-0.0))*(1.29 + 0.37)) - 0.37; 
	//std::cout << angle << " -> "<< newAngle << "\n";
}

void Demonstration_Panel::setHeadPan(){
	HEAD_PAN_ACT = true;
	ROS_INFO("Move Head");
}

void Demonstration_Panel::updateTopic(){
	if(ros::ok()){
		//std::cout << SIGNAL_PLAN << SIGNAL_EXECUTE << EXIT << "\n";
	}
	rviz_plugins::lfd msg;
	msg.exit = EXIT;
	msg.plan = SIGNAL_PLAN;
	msg.execute = SIGNAL_EXECUTE;
	msg.gripper_open = GRIPPER_OPEN;
	msg.gripper_close = GRIPPER_CLOSE;
	msg.head_tilt_angle = HEAD_TILT_ANGLE;
	msg.head_tilt_action = HEAD_TILT_ACT;
	msg.head_pan_angle = HEAD_PAN_ANGLE;
	msg.head_pan_action = HEAD_PAN_ACT;
	pub.publish(msg);
}

//==================================
	
// Tell pluginlib about this class.  Every class which should be
// loadable by pluginlib::ClassLoader must have these two lines
// compiled in its .cpp file, outside of any namespace scope.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS( Demonstration_Panel, rviz::Panel )
