#include "SplineCam.hpp"

#include <cmath>

SplineCamera::SplineCamera(Spline const& s) :
        mSpline(s),
        mPositionVector(mSpline.sampleCurve(0, 0)),
        mUpVector(glm::vec3(0, 1, 0)),
        mTraversalTime(10.f) { }

void SplineCamera::updateCamera(atlas::utils::Time const& t) { mCurrentTime = t; }

void SplineCamera::resetCamera()
{
        mCurrentTime = atlas::utils::Time();
        mUpVector = glm::vec3(0, 1, 0);
        mPositionVector = mSpline.sampleCurve(0, 0.f);
}

atlas::math::Matrix4 SplineCamera::getCameraMatrix()
{
        size_t segment = std::floor(mCurrentTime.currentTime);
        float sample = mCurrentTime.currentTime - segment;
        segment %= mSpline.segments();
        // Use this if we want to look forward along spline
        //glm::vec3 forward_sample = mSpline.sampleCurve(segment + 1, sample);
        mRotationMatrix = glm::lookAt(mPositionVector, glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));
        mPositionVector = mSpline.sampleCurve(segment, sample);;
        return mRotationMatrix;
}
