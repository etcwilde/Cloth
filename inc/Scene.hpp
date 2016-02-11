#ifndef __SCENE_HPP
#define __SCENE_HPP

#include <atlas/utils/Scene.hpp>
#include <atlas/core/Log.hpp>
#include <atlas/gl/Shader.hpp>

#include <memory>
#include <map>
#include <utility>
#include <iostream>

#include "Camera.hpp"
#include "SplineCam.hpp"
#include "Grid.hpp"
#include "Spline.hpp"
#include "ShaderPaths.hpp"
#include "Cloth.hpp"

class Scene: public atlas::utils::Scene
{
        public:
                Scene();
                ~Scene();

                // Event Handlers
                void mousePressEvent(int b, int a, int m, double x, double y) override;
                void mouseMoveEvent(double x, double y) override;
                void scrollEvent(double x, double y) override;

                void keyPressEvent(int key, int scancode, int action, int mods) override;
                void screenResizeEvent(int width, int height) override;

                void updateScene(double time) override;
                void renderScene() override;


        private:

                // Not a whole lot of options right now
                enum MoveType { CAMERA, OBJECT };
                enum ViewType { USER_VIEW, SPLINE_VIEW };

                inline void stepScene()
                {
                        mTime.deltaTime = 0.5f;
                        mTime.totalTime += 0.5f;
                        mCloth.updateGeometry(mTime);
                }

                glm::vec3 getCameraPos();

                // Returns the index of the thing
                int pickScene(double mx, double my);

                bool mDragging;
                bool mPaused;
                bool mPrevTime;

                int _width;
                int _height;

                MoveType _movementType;
                ViewType _viewing;
                size_t _selected_idx;

                Spline mSpline;
                Grid mGrid;
                ArcCamera mCamera;
                SplineCamera mSplineCam;
                Cloth mCloth;

                // For displaying the last pick
                GLuint mPickArray;
                GLuint mPickBuffer;

                glm::vec3 mLastPick[2];

                std::unique_ptr<atlas::gl::Shader> mShaderPtr;
                std::map<std::string, GLint> mUniforms;
                typedef std::pair<std::string, GLint> UniformKey;
};

#endif//__SCENE_HPP
