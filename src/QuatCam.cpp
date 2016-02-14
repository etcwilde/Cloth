#include "QuatCam.hpp"

QuatCamera::QuatCamera():
        mRotationMatrix(glm::mat4(1.f)),
        mPositionMatrix(glm::translate(glm::mat4(1.f), glm::vec3(4, -3, 4)))
{
        std::vector<glm::mat4> rotations;
        //rotations.push_back(glm::orientate4(glm::vec3(0, 0.01, 0)));
        //rotations.push_back(glm::orientate4(glm::vec3(0.05, 0, 0)));
        rotations.push_back(glm::orientate4(glm::vec3(0, 0, 0)));
        rotations.push_back(glm::orientate4(glm::vec3(0, 0.01, 0.01)));
        rotations.push_back(glm::orientate4(glm::vec3(0.04, 0, 0.01)));

        // For each orientation, add a quaternion
        for (auto o: rotations) mRotations.push_back(glm::quat_cast(o));
}

QuatCamera::~QuatCamera() { }

void QuatCamera::updateCamera(atlas::utils::Time const& t)
{
        size_t current_quat, next_quat;
        current_quat = std::floor(t.currentTime);
        float sample = t.currentTime - current_quat;
        current_quat %= mRotations.size();
        next_quat = (current_quat + 1) % mRotations.size();

        // Slerp
        glm::quat s = slerp(mRotations[current_quat], mRotations[next_quat], sample);
        mRotationMatrix = glm::mat4_cast(s) * mRotationMatrix; }

atlas::math::Matrix4 QuatCamera::getCameraMatrix()
{
        return mRotationMatrix * mPositionMatrix;
}

glm::quat QuatCamera::slerp(glm::quat a, glm::quat b, float t)
{
        float theta = acos(glm::dot(a, b));
        float div;
        if (atlas::core::isOne(theta)) div = 1.f;
        else div = 1.f/sin(theta);
        return (a * static_cast<float>(sin((1.f - t) * theta)) + b * static_cast<float>(sin(t * theta))) * div;
}
