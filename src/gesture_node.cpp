//===============================================================================//
// Name			: gesture_node.cpp
// Author(s)	: Yeshasvi TVS & Prashanth Ramadoss
// Affiliation	: University of Genova, Italy - dept. DIBRIS
// Version		: 1.0
// Description	: Gesture node for baxter 
//===============================================================================//



#include <ros/ros.h>
#include <geometry_msgs/Pose.h>
#include <std_msgs/Char.h>
#include <sensor_msgs/JointState.h>
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/PlanningScene.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <stdio.h>
#include <iostream>


using namespace std;


bool left_success;
bool right_success;
double left_planning_time;
double right_planning_time;
bool action;
int arm_at;
bool arm_pointing;

geometry_msgs::Pose targetPose; //Target pose variable
ros::Subscriber gesture_sub; //Gesture subscriber
char p;

//Subscriber callback function
void gestureCallback(std_msgs::Char gesture_rec)
{
	//cout<<"Gesture received"<<endl;
	p = gesture_rec.data;
	
	
}


int main(int argc, char** argv)
{

	ros::init(argc,argv,"gesture");
	ros::NodeHandle nh;
	ros::AsyncSpinner spinner(1); // Use 4 threads
    	spinner.start();
    	action = false; 
    	
    	gesture_sub = nh.subscribe<std_msgs::Char>("/gesture_detected",1,gestureCallback);
   	
	//Move group variables
	moveit::planning_interface::MoveGroup left_group("left_arm");
	moveit::planning_interface::MoveGroup right_group("right_arm");
	
	//Planning parameters 
	//left_group.setNumPlanningAttempts(4);
	//left_group.setPlanningTime(5.0);
	
	//right_group.setNumPlanningAttempts(4);
	//right_group.setPlanningTime(5.0);
	
	
	//Changing default planner to new one
	//left_group.setPlannerId("SBLkConfigDefault");
	//right_group.setPlannerId("SBLkConfigDefault");

	//Plan variables
	moveit::planning_interface::MoveGroup::Plan left_plan;
	moveit::planning_interface::MoveGroup::Plan right_plan;
	
	//moveit::planning_interface::PlanningSceneInterface planning_scene_interface;

	left_planning_time = 0;
	right_planning_time = 0;
	arm_pointing = false;
	
		
	ros::Rate rate(10);
	while(ros::ok()) //TODO check if this is getting executed 
	{
	
		
		std::cout << "Inside ROS OK!" << std::endl;//Debug Code
  			
		
		
  		cout<<"Waiting for gesture..." << endl;
  	
 		if(p == 65)//char A
 		{ 
 			std::cout<<"Received Gesture A "<<std::endl;//Debug Code
 			
 			targetPose.position.x=0.77;	//Set the end effector pose values for each gesture
			targetPose.position.y=0.18;
			targetPose.position.z=-0.08;
			targetPose.orientation.x=-0.3351;
			targetPose.orientation.y=0.9179;
			targetPose.orientation.z=0.1863;
			targetPose.orientation.w=0.1016;
			arm_at = p;
			p=0;
			action = true;
			
		}
  	
		else if(p == 66)//char B
 		{ 
 			std::cout<<"Received Gesture B "<<std::endl;//Debug Code
 			targetPose.position.x=0.79;	//Set the end effector pose values for each gesture
			targetPose.position.y=-0.02;
			targetPose.position.z=-0.15;
			targetPose.orientation.x=0.0268;
			targetPose.orientation.y=0.9981;
			targetPose.orientation.z=-0.0050;
			targetPose.orientation.w=0.0543;
			arm_at = p;
			p=0;
			action = true;
 	 	}
		else if(p == 67)//char C
 		{ 
 			std::cout<<"Received Gesture C "<<std::endl;//Debug Code
 			targetPose.position.x=0.72;	//Set the end effector pose values for each gesture
			targetPose.position.y=-0.23;
			targetPose.position.z=-0.15;
			targetPose.orientation.x=-0.4123;
			targetPose.orientation.y=0.8899;
			targetPose.orientation.z=-0.1255;
			targetPose.orientation.w=0.1487;
			arm_at = p;
			p=0;
			action = true;
 	 	}


		if(action == true ){
		
  			left_group.setPoseTarget(targetPose);
  			//left_group.setGoalPositionTolerance(0.1);
			//left_group.setGoalOrientationTolerance(0.4); 
			
  			right_group.setPoseTarget(targetPose);
  			//right_group.setGoalPositionTolerance(0.1);
			//right_group.setGoalOrientationTolerance(0.4); 
  		
  			left_planning_time = ros::Time::now().toSec();
			left_success = left_group.plan(left_plan);
			left_planning_time = ros::Time::now().toSec() - left_planning_time ;
			
			std::cout << "Left plan success : " << left_success << std::endl;//Debug Code
			std::cout << "Left planning time "<< left_planning_time << std::endl;//Debug Code
		
  			right_planning_time = ros::Time::now().toSec();
			right_success = right_group.plan(right_plan);
			right_planning_time = ros::Time::now().toSec() - right_planning_time ;
			
			std::cout << "Right plan success : " << right_success << std::endl;//Debug Code
			std::cout << "Right planning time "<< right_planning_time << std::endl;//Debug Code
			
		
			if( left_success == false && right_success == false ){
		
				std::cout << "Moving right arm to Random pose "<<std::endl;
				right_group.setRandomTarget();
				right_group.move();
			}
			else if(right_planning_time < left_planning_time){

				if(right_success == true ){ 
				
					right_group.move(); 
					action = false;
				
				}
				else { 
					left_group.move(); 
					action = false;
				}
			
			}
			else{ 
				
  			  	if(left_success == true ){ 
  			  		left_group.move(); 
  			  		action = false;
  			  	}
				else { 
					right_group.move(); 
					action = false;
				}
  			}
  		
  		
  		
		}
	rate.sleep() ;
	}
	ros::waitForShutdown();
	return 0;
}		
