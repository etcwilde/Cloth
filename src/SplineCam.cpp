#include "SplineCam.hpp"

#include <cmath>

SplineCamera::SplineCamera(Spline const& s) :
        mSpline(s),
        mPositionVector(mSpline.sampleCurve(0, 0)),
        mUpVector(glm::vec3(0, 1, 0)),
        mTraversalTime(10.f)
{
        // kay, sample the spline and make the position
#ifdef PROG_DEBUG
        USING_ATLAS_CORE_NS;
#include <atlas/core/Log.hpp>
        Log::log(Log::SeverityLevel::DEBUG, "Camera Position: " +
                        std::to_string(mPositionVector.x) + ", " +
                        std::to_string(mPositionVector.y) + ", " +
                        std::to_string(mPositionVector.z));
#endif
}

void SplineCamera::updateCamera(atlas::utils::Time const& t)
{
        mCurrentTime = t;
}

void SplineCamera::resetCamera()
{
        mCurrentTime = atlas::utils::Time();
        mUpVector = glm::vec3(0, 1, 0);
        mPositionVector = mSpline.sampleCurve(0, 0.f);
}

atlas::math::Matrix4 SplineCamera::getCameraMatrix()
{
        // TODO: See if this should be cached until the camera is updated
        size_t segment = std::floor(mCurrentTime.currentTime);
        float sample = mCurrentTime.currentTime - segment;
        segment %= mSpline.segments();
        glm::vec3 forward_sample = mSpline.sampleCurve(segment + 1, sample);
        mRotationMatrix = glm::lookAt(mPositionVector, forward_sample, glm::vec3(0, 1, 0));
        mPositionVector = mSpline.sampleCurve(segment, sample);;
        return mRotationMatrix;
}
