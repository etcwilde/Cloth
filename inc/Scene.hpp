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
#include "Grid.hpp"
#include "Spline.hpp"
#include "ShaderPaths.hpp"

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
                enum SelectionType { SPLINE };
                enum MoveType { CAMERA, OBJECT };

                glm::vec3 getCameraPos();

                // Returns the index of the thing
                int pickScene(double mx, double my);

                bool mDragging;
                bool mPaused;
                bool mPrevTime;

                int _width;
                int _height;

                MoveType _movementType;
                SelectionType _selector;
                size_t _selected_idx;


                ArcCamera mCamera;
                Grid mGrid;
                Spline mSpline;

                // For displaying the last pick
                GLuint mPickArray;
                GLuint mPickBuffer;

                glm::vec3 mLastPick[2];

                std::unique_ptr<atlas::gl::Shader> mShaderPtr;
                std::map<std::string, GLint> mUniforms;
                typedef std::pair<std::string, GLint> UniformKey;
};

#endif//__SCENE_HPP