#ifndef __CAMERA_HPP
#define __CAMERA_HPP

#include <atlas/utils/Camera.hpp>

#include <algorithm>
#include <memory>
#include <cmath>

class ArcBall
{
        protected:
                void _mapToSphere(atlas::math::Point2 pt, glm::vec3& vec);

        public:
                ArcBall() :
                        _startVec(0.f),
                        _endVec(0.f),
                        _width(1.f),
                        _height(1.f)
                { }
                ArcBall(float width, float height):
                        _startVec(0.f),
                        _endVec(0.f),
                        _width(width),
                        _height(height) {}

                ~ArcBall() {}

                inline void setBounds(float width, float height)
                {
                        assert((width > 1.f) && (height > 1.f));
                        _width = 1.f / ((width - 1.f) * 0.5f);
                        _height = 1.f / ((height - 1.f) * 0.5f);
                }

                void mouseDown(const atlas::math::Point2& pt);
                void mouseDrag(const atlas::math::Point2& pt, glm::quat& rot);

        protected:
                glm::vec3 _startVec;
                glm::vec3 _endVec;
                glm::quat _lastRot;
                float _width;
                float _height;
};



class ArcCamera : public atlas::utils::Camera
{
        public:
                ArcCamera();
                ~ArcCamera();

                void mouseDown(atlas::math::Point2 const& pt, atlas::utils::Camera::CameraMovements movement) override;
                void mouseDrag(atlas::math::Point2 const& pt) override;
                void mouseScroll(atlas::math::Point2 const& pt);
                void mouseUp() override;
                void resetCamera() override;

                void setDims(int width, int height);

                atlas::math::Matrix4 getCameraMatrix() override;

        private:

                struct ArcCameraImpl;
                std::unique_ptr<ArcCameraImpl> mImpl;

                bool _cacheSet;
                atlas::math::Matrix4 _cachedView;
};

#endif//__CAMERA_HPP
