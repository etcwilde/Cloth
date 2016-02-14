#ifndef __QUAT_CAM_HPP
#define __QUAT_CAM_HPP

// The quaternion camera
// Linearly interpolates between points, slerps between orientations -- could get
// interesting

#include <atlas/utils/Camera.hpp>
#include <atlas/utils/Time.hpp>
#include <atlas/core/Float.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <vector>

class QuatCamera : public atlas::utils::Camera
{
        public:
                QuatCamera();
                ~QuatCamera();

                atlas::math::Matrix4 getCameraMatrix() override;

                void updateCamera(atlas::utils::Time const& t);
        private:

                static glm::quat slerp(glm::quat a, glm::quat b, float t);

                std::vector<glm::vec3> mPositions;
                std::vector<glm::quat> mRotations;

                glm::mat4 mRotationMatrix;
                glm::mat4 mPositionMatrix;
};


#endif//__QUAT_CAM_HPP
