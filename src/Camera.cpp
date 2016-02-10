#include "Camera.hpp"
#include <atlas/core/Float.hpp>
#include <string>

void ArcBall::_mapToSphere(atlas::math::Point2 pt, glm::vec3& vec)
{
        USING_ATLAS_CORE_NS;
        // Scale point coordinates
        pt.x = (pt.x * _width) - 1.f;
        pt.y = 1.f - (pt.y * _height);
        float len = glm::length(pt);
        // Beyond the sphere
        if(len > 1.f)
        {
                float norm = 1.f / std::sqrt(len);
                vec.x = pt.x * norm;
                vec.y = pt.y * norm;
                vec.z = 0.f;
        }
        else
        {
                vec.x = pt.x;
                vec.y = pt.y;
                vec.z = std::sqrt(1.f - len);
        }
}

void ArcBall::mouseDown(const atlas::math::Point2& pt)
{
        USING_ATLAS_CORE_NS;
        _mapToSphere(pt, _startVec);
}

void ArcBall::mouseDrag(const atlas::math::Point2& pt, glm::quat& rot)
{
        USING_ATLAS_CORE_NS;
        _mapToSphere(pt, _endVec);
        glm::vec3 perp;
        perp = glm::cross(_startVec, _endVec);
        // FIXME: Replace with atlas is_zero
        if(!isZero(glm::length(perp)))
        {
                rot.x = perp.x;
                rot.y = perp.y;
                rot.z = perp.z;
                rot.w = glm::dot(_startVec, _endVec);
        }
        else rot.x = rot.y = rot.z = rot.w = 0.f;
        _startVec = _endVec;
}

struct ArcCamera::ArcCameraImpl
{
        ArcCameraImpl() :
                dolly(100.f),
                tumbleVector(45.f),
                trackVector(0.f),
                lastPos(0.f),
                movement(ArcCamera::CameraMovements::IDLE)
        {

                resetMatrix();
        }

        void resetAll()
        {
                USING_ATLAS_MATH_NS;
                dolly = 100.f;
                trackVector = Vector(0.f);
                tumbleVector = Vector(45.f);
                resetMatrix();
        }

        // TODO: Update the tumble matrix
        void resetMatrix()
        {
                USING_ATLAS_MATH_NS;
                dollyMatrix = glm::translate(Matrix4(1.f), Vector(0.f, 0.f, -1.f * dolly));
                trackMatrix = Matrix4(1.f);
                tumbleMatrix =
                        glm::rotate(Matrix4(1.f), glm::radians(tumbleVector.x), Vector(1, 0, 0)) *
                        glm::rotate(Matrix4(1.f), glm::radians(tumbleVector.y), Vector(1, 0, 0));
        }

        inline void setDims(int width, int height)
        {
                ball.setBounds(static_cast<float>(width), static_cast<float>(height));
                USING_ATLAS_CORE_NS;
        }

        float dolly;
        atlas::math::Vector tumbleVector;
        atlas::math::Vector trackVector;
        atlas::math::Point2 lastPos;
        ArcCamera::CameraMovements movement;

        atlas::math::Matrix4 dollyMatrix;
        atlas::math::Matrix4 tumbleMatrix;
        atlas::math::Matrix4 trackMatrix;
        ArcBall ball;
};

ArcCamera::ArcCamera() : mImpl(new ArcCameraImpl) {}
ArcCamera::~ArcCamera() {}

void ArcCamera::mouseDown(atlas::math::Point2 const& p, ArcCamera::CameraMovements m)
{
        USING_ATLAS_CORE_NS;
        mImpl->movement = m;
        mImpl->lastPos = p;
        if  (m == ArcCamera::CameraMovements::TUMBLE)
        {
                mImpl->ball.mouseDown(p);
        }
        _cacheSet = false;
}

void ArcCamera::mouseDrag(atlas::math::Point2 const& p)
{
        USING_ATLAS_MATH_NS;
        Point2 delta = p - mImpl->lastPos;
        switch(mImpl->movement)
        {
                case ArcCamera::CameraMovements::DOLLY:
                        mImpl->dolly -= 0.08f * (delta.y + delta.x);
                        mImpl->dollyMatrix =
                                glm::translate(Matrix4(1.f), Vector(0, 0, -1.f * mImpl->dolly));
                        break;
                case ArcCamera::CameraMovements::TUMBLE:
                        {
                                // Okay, Lets do this with an arcball
                                glm::quat qRot;
                                mImpl->ball.mouseDrag(p, qRot);
                                mImpl->tumbleMatrix = glm::mat4_cast(qRot) * mImpl->tumbleMatrix;
                        }
                        break;
                case ArcCamera::CameraMovements::TRACK:
                        mImpl->trackVector.x += 0.05f * delta.x;
                        mImpl->trackVector.y -= 0.05f * delta.y;
                        mImpl->trackMatrix = glm::translate(Matrix4(1.f), mImpl->trackVector) ;

                        break;

                case ArcCamera::CameraMovements::IDLE:
                        break;
        }
        mImpl->lastPos = p;
        _cacheSet = false;
}

void ArcCamera::mouseScroll(atlas::math::Point2 const& p)
{
        USING_ATLAS_MATH_NS;
        mImpl->dolly -= p.y;
        mImpl->dollyMatrix = glm::translate(Matrix4(1.f), Vector(0, 0, -1.f * mImpl->dolly));
        _cacheSet = false;
}

void ArcCamera::mouseUp()
{
        mImpl->movement = ArcCamera::CameraMovements::IDLE;
}

void ArcCamera::resetCamera()
{
        mImpl->resetAll();
        _cacheSet = false;
}

void ArcCamera::setDims(int width, int height)
{
        mImpl->setDims(width, height);
        _cacheSet = false;
}

atlas::math::Matrix4 ArcCamera::getCameraMatrix()
{
        if (_cacheSet) return _cachedView;
        _cacheSet = true;
        _cachedView = mImpl->dollyMatrix * mImpl->trackMatrix * mImpl->tumbleMatrix;
        return _cachedView;
}


