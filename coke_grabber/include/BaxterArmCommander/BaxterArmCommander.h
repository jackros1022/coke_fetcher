//
// Created by sxt437 on 4/30/16.
//

#ifndef COKE_GRABBER_BAXTERARMCOMMANDER_H
#define COKE_GRABBER_BAXTERARMCOMMANDER_H

#include <ros/ros.h>
#include <eigen3/Eigen/Eigen>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>
#include <geometry_msgs/PoseStamped.h>
#include <moveit/move_group_interface/move_group.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <BaxterGripper/BaxterGripper.h>

using namespace std;  //just to avoid requiring std::,  ...
using namespace Eigen;
typedef Matrix<double, 7, 1> Vector7d;

class BaxterArmCommander {
public:
    BaxterArmCommander(ros::NodeHandle &nodehandle);

    bool rightArmBack();                        //move arm back to a pre pose and open gripper, return successful or not
    Vector7d rightGetJoints();                  //return the angle (in rad) of each joint
    geometry_msgs::Pose rightGetPose();         //return pose of right arm end effector
    bool rightPlan(geometry_msgs::Pose pose);   //plan a trajectory to point end effector to a certain pose, return success or failed
    bool rightPlan(Vector7d joints);            //another version that accept joint angles
    //another version that accept this three parameter of the object, this three values can be calculated from Newman's code
    bool rightPlan(Vector3f plane_normal, Vector3f major_axis, Vector3f centroid);
    bool rightPlanOffset(Vector3d offset);      //from current pose, add a offset
    void rightExecute();                        //execute planned pose
    bool rightMove(geometry_msgs::Pose pose);   //combine plan and execute together
    bool rightMove(Vector7d joints);            //same, specify joints
    bool rightMove(Vector3f plane_normal, Vector3f major_axis, Vector3f centroid);  //same, specify the three parameters
    bool rightMoveOffset(Vector3d offset);
    void rightGrab();                           //close gripper
    void rightRelease();                        //open gripper
    void rightShowPath();                       //visualize trajectory that planned in rviz, must call plan first

    bool leftArmBack();                         //everything is just the same as right one
    Vector7d leftGetJoints();
    geometry_msgs::Pose leftGetPose();
    bool leftPlan(geometry_msgs::Pose pose);
    bool leftPlan(Vector7d joints);
    bool leftPlanOffset(Vector3d offset);
    bool leftPlan(Vector3f plane_normal, Vector3f major_axis, Vector3f centroid);
    void leftExecute();
    bool leftMove(geometry_msgs::Pose pose);
    bool leftMove(Vector7d joints);
    bool leftMove(Vector3f plane_normal, Vector3f major_axis, Vector3f centroid);
    bool leftMoveOffset(Vector3d offset);
    void leftGrab();
    void leftRelease();
    void leftShowPath();

    bool ArmBack();                             //move both back to pre pose
    bool grabCoke(geometry_msgs::Pose coke_pose);//a whole sequence of movement for grabbing coke

    Eigen::Affine3d transformPoseToEigenAffine3d(geometry_msgs::Pose pose);
    geometry_msgs::Pose transformEigenAffine3dToPose(Eigen::Affine3d e);
    geometry_msgs::Pose normalToPose(Vector3f plane_normal, Vector3f major_axis, Vector3f centroid);
    geometry_msgs::Pose addPosOffset(geometry_msgs::Pose pose, Vector3d offset);
    geometry_msgs::Pose subPosOffset(geometry_msgs::Pose pose, Vector3d offset);
    geometry_msgs::Pose addPose(geometry_msgs::Pose pose_a, geometry_msgs::Pose pose_b);
    std::vector<double> quat2euler(geometry_msgs::Quaternion quaternion);   //this may be useful

private:
    ros::NodeHandle nh_;
    BaxterGripper right_gripper;

    //ros::AsyncSpinner spinner;
    moveit::planning_interface::MoveGroup right_arm, left_arm;
    moveit::planning_interface::PlanningSceneInterface planning_scene_interface;
    moveit::planning_interface::MoveGroup::Plan right_plan, left_plan;
    ros::Publisher display_publisher;
    moveit_msgs::DisplayTrajectory display_trajectory;

    geometry_msgs::Pose right_arm_back_pose;
    geometry_msgs::Pose left_arm_back_pose;

    Vector3d pick_offset;
    Vector3d hold_offset;
    Vector3d pre_grab_offset;
    Vector3d grab_offset;

    geometry_msgs::Pose global_pose_offset;
    Vector7d global_joints_offset;
};


#endif //COKE_GRABBER_BAXTERARMCOMMANDER_H
