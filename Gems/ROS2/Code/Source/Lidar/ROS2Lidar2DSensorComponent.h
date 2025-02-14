/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */
#pragma once

#include <Atom/RPI.Public/AuxGeom/AuxGeomDraw.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Lidar/LidarRaycaster.h>
#include <Lidar/LidarTemplate.h>
#include <Lidar/LidarTemplateUtils.h>
#include <ROS2/Lidar/LidarRegistrarBus.h>
#include <ROS2/Lidar/LidarSystemBus.h>
#include <ROS2/Sensor/ROS2SensorComponent.h>
#include <rclcpp/publisher.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>

namespace ROS2
{
    //! Lidar 2D sensor Component.
    //! Lidars (Light Detection and Ranging) emit laser light and measure it after reflection.
    //! Lidar Component allows customization of lidar type and behavior and encapsulates both simulation
    //! and data publishing. It requires ROS2FrameComponent.
    class ROS2Lidar2DSensorComponent : public ROS2SensorComponent
    {
    public:
        AZ_COMPONENT(ROS2Lidar2DSensorComponent, "{F4C2D970-1D69-40F2-9D4D-B52DCFDD2704}", ROS2SensorComponent);
        ROS2Lidar2DSensorComponent();
        ~ROS2Lidar2DSensorComponent() = default;
        static void Reflect(AZ::ReflectContext* context);
        //////////////////////////////////////////////////////////////////////////
        // Component overrides
        void Activate() override;
        void Deactivate() override;
        //////////////////////////////////////////////////////////////////////////

    private:
        //////////////////////////////////////////////////////////////////////////
        // ROS2SensorComponent overrides
        void FrequencyTick() override;
        void Visualise() override;

        bool IsConfigurationVisible() const;
        bool IsIgnoredLayerConfigurationVisible() const;
        bool IsEntityExclusionVisible() const;
        bool IsMaxPointsConfigurationVisible() const;

        AZ::Crc32 OnLidarModelSelected();
        AZ::Crc32 OnLidarImplementationSelected();
        void FetchLidarImplementationFeatures();
        AZStd::vector<AZStd::string> FetchLidarSystemList();
        void ConnectToLidarRaycaster();
        void ConfigureLidarRaycaster();

        LidarSystemFeatures m_lidarSystemFeatures;
        LidarTemplate::LidarModel m_lidarModel = LidarTemplate::LidarModel::Custom2DLidar;
        LidarTemplate m_lidarParameters = LidarTemplateUtils::GetTemplate(LidarTemplate::LidarModel::Custom2DLidar);
        AZStd::vector<AZ::Vector3> m_lastRotations;

        AZStd::string m_lidarSystem;
        // A structure that maps each lidar implementation busId to the busId of a raycaster created by this LidarSensorComponent.
        AZStd::unordered_map<AZStd::string, LidarId> m_implementationToRaycasterMap;
        LidarId m_lidarRaycasterId;
        std::shared_ptr<rclcpp::Publisher<sensor_msgs::msg::LaserScan>> m_laserScanPublisher;

        // Used only when visualisation is on - points differ since they are in global transform as opposed to local
        AZStd::vector<AZ::Vector3> m_visualisationPoints;
        AZ::RPI::AuxGeomDrawPtr m_drawQueue;

        RaycastResult m_lastScanResults;

        AZ::u32 m_ignoredLayerIndex = 0;
        bool m_ignoreLayer = false;
        AZStd::vector<AZ::EntityId> m_excludedEntities;

        bool m_addPointsAtMax = false;
    };
} // namespace ROS2
