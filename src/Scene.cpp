#include "Scene.hpp"

#include <string>

#include <atlas/core/Log.hpp>
#include <atlas/core/GLFW.hpp>

Scene::Scene() :
        mDragging(false),
        mPaused(true),
        mPrevTime(0.0)
{
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_DEPTH_TEST);
        glShadeModel(GL_FLAT);
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
Scene::~Scene() { }

void Scene::mousePressEvent(int b, int a, int m, double x, double y)
{
        USING_ATLAS_CORE_NS;
#ifdef PROG_DEBUG
        Log::log(Log::SeverityLevel::DEBUG, "Mouse Press Event: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
#endif

        USING_ATLAS_MATH_NS;
        if(b == GLFW_MOUSE_BUTTON_MIDDLE)
        {
                if(a == GLFW_PRESS)
                {
                        mDragging = true;
                        ArcCamera::CameraMovements movements;
                        switch(m)
                        {
                                case GLFW_MOD_CONTROL:
                                        movements = ArcCamera::CameraMovements::DOLLY;
                                        break;
                                case GLFW_MOD_SHIFT:
                                        movements = ArcCamera::CameraMovements::TRACK;
                                        break;
                                default:
                                        movements= ArcCamera::CameraMovements::TUMBLE;
                                        break;
                        }
                        mCamera.mouseDown(Point2(x, y), movements);
                }
                else
                {
                        mDragging = false;
                        mCamera.mouseUp();
                }
        }
}

void Scene::mouseMoveEvent(double x, double y)
{
        USING_ATLAS_MATH_NS;
        if (mDragging)
        {
                USING_ATLAS_CORE_NS;

                switch(_movementType)
                {
                        case Scene::MoveType::OBJECT:
                                break;
                        default:
                                mCamera.mouseDrag(Point2(x, y));
                                break;
                }
        }

        if(mDragging) mCamera.mouseDrag(Point2(x, y));

}

void Scene::scrollEvent(double x, double y)
{
        USING_ATLAS_CORE_NS;
        USING_ATLAS_MATH_NS;
#ifdef PROG_DEBUG
        Log::log(Log::SeverityLevel::DEBUG, "Mouse Scroll Event: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
#endif
        mCamera.mouseScroll(Point2(x, y));
}

void Scene::keyPressEvent(int key, int scancode, int action, int modes)
{
        if (action == GLFW_PRESS)
        {
                if (key == GLFW_KEY_SPACE)
                {
                        mPaused = !mPaused;
                }
        }
}

void Scene::screenResizeEvent(int width, int height)
{
        _width = width;
        _height = height;
        atlas::utils::Scene::screenResizeEvent(width, height);
        mCamera.setDims(width, height);
}

void Scene::updateScene(double time)
{
        if(!mPaused)
        {
                mTime.deltaTime = static_cast<float>(time) - mTime.currentTime;
                mTime.totalTime += static_cast<float>(time);
                mTime.currentTime = static_cast<float>(time);
                mTime.deltaTime *= 2.f;
        }
}

void Scene::renderScene()
{
        const float grey = 0.431;
        glClearColor(grey, grey, grey, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        mView = mCamera.getCameraMatrix();
        mGrid.renderGeometry(mProjection, mView);
        mSpline.renderGeometry(mProjection, mView);
}

glm::vec3 Scene::getCameraPos()
{
        const glm::mat4 view_inverse = glm::inverse(mCamera.getCameraMatrix());
        glm::vec3 ret_vec = glm::vec3(view_inverse[3].x, view_inverse[3].y, view_inverse[3].z) / view_inverse[3].w;
        return ret_vec;
}

int Scene::pickScene(double mx, double my)
{

        USING_ATLAS_CORE_NS;

        // std::vector<glm::vec3> test_points;
        // //if( _selector == Scene::SelectionType::SPLINE)
        // {
        //         test_points = mSpline.getPoints();
        // }
        //else return -1;

        //if (test_points.size() == 0) return -1;

        glm::vec4 viewport = glm::vec4(0.f, 0.f, _width, _height);
        const glm::vec3 ray_start = glm::unProject(glm::vec3(mx, my, 0.f),
                        mCamera.getCameraMatrix(),
                        mProjection, viewport);
        const glm::vec3 ray_end = glm::unProject(glm::vec3(mx, my, 1.f),
                        mCamera.getCameraMatrix(),
                        mProjection, viewport);

        //mLastPick[0] = ray_start;
        //mLastPick[1] = ray_end;

        // okay neat
        const glm::vec3 b = ray_end - ray_start;



//#ifdef PROG_DEBUG
       Log::log(Log::SeverityLevel::DEBUG, "From " +
                       std::to_string(ray_start.x) + ", " +
                       std::to_string(ray_start.y) + ", " +
                       std::to_string(ray_start.z) + " to " +
                       std::to_string(ray_end.x) + ", " +
                       std::to_string(ray_end.y) + ", " +
                       std::to_string(ray_end.z)
                       );
//#endif


        //glm::vec3 nearest;
        //float closest = 10000.f;
        //size_t selected_index = 0;

        // for (auto it = test_points.begin();
        //                 it != test_points.end(); it++)
        // {
        //         glm::vec3 a = *it - ray_start;
        //         glm::vec3 rejection = a - glm::dot(a, b) /  glm::dot(b, b) * b;
        //         float len = glm::length(rejection);
        //         if (len < closest)
        //         {
        //                 closest = len;
        //                 selected_index = it - test_points.begin();
        //                 nearest = test_points[selected_index];
        //         }
        // }

        //Log::log(Log::SeverityLevel::DEBUG,
        //                "Closest[" + std::to_string(closest) + "]: (" +
        //                std::to_string(nearest.x) + ", " +
        //                std::to_string(nearest.y) + ", " +
        //                std::to_string(nearest.z) + ")");

        return 0;
}
