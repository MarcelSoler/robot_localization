/*
 * Copyright (c) 2016, TNO IVS Helmond.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided
 * with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef ROBOT_LOCALIZATION_ROS_LISTENER_H
#define ROBOT_LOCALIZATION_ROS_LISTENER_H

#include "robot_localization/robot_localization_estimator.h"

#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/AccelWithCovarianceStamped.h>

namespace RobotLocalization
{

//! @brief RosRobotLocalizationListener class
//!
//! This class wraps the RobotLocalizationEstimator. It listens to
//! topics over which the (filtered) robot state is published (odom
//! and accel) and pushes them into its instance of the
//! RobotLocalizationEstimator. It exposes a getState method to
//! offer the user the estimated state at a requested time. This
//! class offers the option to run this listener without the need to
//! to run a separate node. If you do wish to run this functionality
//! in a separate node, consider the robot localization listener
//! node.
//!
class RosRobotLocalizationListener
{
  public:
    //! @brief Constructor
    //!
    //! The RosRobotLocalizationListener constructor initializes
    //! nodehandles, subscribers, a filter for synchronized listening
    //! to the topics it subscribes to, and an instance of the
    //! RobotLocalizationEstimator.
    //!
    //! @param[in] ns - namespace to place the subscribers and
    //! parameters in.
    //!
    explicit RosRobotLocalizationListener(const std::string& ns="");

    //! @brief Destructor
    //!
    //! Empty destructor
    //!
    ~RosRobotLocalizationListener();

    //! @brief Get a state from the localization estimator
    //!
    //! Requests the predicted state and covariance at a given time
    //! from the Robot Localization Estimator.
    //!
    //! @param[in] time - time of the requested state
    //! @param[out] state - state at the requested time
    //! @param[out] covariance - covariance at the requested time
    //!
    //! @return false if buffer is empty, true otherwise
    //!
    bool getState(const double time, Eigen::VectorXd& state, Eigen::MatrixXd& covariance);

    //! @brief Get a state from the localization estimator
    //!
    //! Overload of getState method for using ros::Time.
    //!
    //! @param[in] ros_time - ros time of the requested state
    //! @param[out] state - state at the requested time
    //! @param[out] covariance - covariance at the requested time
    //!
    //! @return false if buffer is empty, true otherwise
    //!
    bool getState(const ros::Time& ros_time, Eigen::VectorXd& state, Eigen::MatrixXd& covariance);

  private:
    //! @brief The core state estimator that facilitates inter- and
    //! extrapolation between buffered states.
    //!
    RobotLocalizationEstimator estimator_;

    //! @brief A public handle to the ROS node
    //!
    ros::NodeHandle nh_;

    //! @brief A private handle to the ROS node
    //!
    ros::NodeHandle nh_p_;

    //! @brief Subscriber to the odometry state topic (output of a filter node)
    //!
    message_filters::Subscriber<nav_msgs::Odometry> odom_sub_;

    //! @brief Subscriber to the acceleration state topic (output of a filter node)
    //!
    message_filters::Subscriber<geometry_msgs::AccelWithCovarianceStamped> accel_sub_;

    //! @brief Waits for both an Odometry and an Accel message before calling a single callback function
    //!
    message_filters::TimeSynchronizer<nav_msgs::Odometry, geometry_msgs::AccelWithCovarianceStamped> sync_;

    //! @brief Callback for odom and accel
    //!
    //! Puts the information from the odom and accel messages in a
    //! Robot Localization Estimator state and sets the most recent
    //! state of the estimator.
    //!
    //! @param[in] odometry message
    //! @param[in] accel message
    //!
    void odomAndAccelCallback(const nav_msgs::Odometry& odom, const geometry_msgs::AccelWithCovarianceStamped& accel);

};

}  // namespace RobotLocalization

#endif  // ROBOT_LOCALIZATION_ROS_FILTER_H
