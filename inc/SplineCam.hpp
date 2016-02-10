#ifndef __SPLINE_CAM_HPP
#define __SPLINE_CAM_HPP

#include <atlas/utils/Camera.hpp>
#include <atlas/utils/Time.hpp>

#include "Spline.hpp"

class SplineCamera : public atlas::utils::Camera
{
        public:
                SplineCamera(Spline const& s);
                ~SplineCamera() {}

                atlas::math::Matrix4 getCameraMatrix() override;

                void updateCamera(atlas::utils::Time const& t);

                void resetCamera() override;
        private:
                Spline mSpline;

                glm::vec3 mPositionVector;
                glm::vec3 mUpVector;
                atlas::math::Matrix4 mPositionMatrix;
                atlas::math::Matrix4 mRotationMatrix;

                float mTraversalTime;
                atlas::utils::Time mCurrentTime;

};

#endif//__SPLINE_CAM_HPP
