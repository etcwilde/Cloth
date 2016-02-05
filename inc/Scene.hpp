#ifndef __SCENE_HPP
#define __SCENE_HPP

#include <atlas/utils/Scene.hpp>
#include <atlas/core/Log.hpp>

#include "Camera.hpp"
#include "Grid.hpp"

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

                bool mDragging;
                bool mPaused;
                bool mPrevTime;

                ArcCamera mCamera;
                Grid mGrid;
};


#endif//__SCENE_HPP
