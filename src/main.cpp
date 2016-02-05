#include <atlas/utils/Application.hpp>

#include "Scene.hpp"

int main()
{

        APPLICATION.createWindow(800, 800, "Cloth");
        // TODO: There appears to be an issue in switching between scenes in Atlas
        APPLICATION.addScene(new Scene);
        APPLICATION.runApplication();
        return 0;
}
