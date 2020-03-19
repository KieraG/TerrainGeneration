#include "GLDisplay.hpp"

#include <SDL2/SDL.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>

#include "Engine/Engine.hpp"
#include "Engine/OpenGL.hpp"
#include "Terrain.h"

using View::GLDisplay;

GLDisplay::GLDisplay() {
    auto &engine = SDLEngine::Engine::get();

    SDL_GL_GetDrawableSize(engine.window.get(), &width, &height);
    GLDisplay::ratio = static_cast<double>(width) / static_cast<double>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluPerspective(60, GLDisplay::ratio, 1, 50000);
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    // gluLookAt(256, 256, 0, 256, 0, 256, 0, 1, 0);
    gluLookAt(0, 256, 0, 256, 0, 256, 0, 1, 0);

    // testTerrain.flatTerrain(128);
    testTerrain.genFaultFormation(256, 512, 0, 255, 0.1, 20, 0);
    // testTerrain.genFaultFormation(16, 128, 100, 200, 1, 1);
    // testTerrain.loadHeightfield("height128.raw", 128);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    float lightPosition[4] = {128, 500, 128, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);


    /*glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);*/

}

auto GLDisplay::display() -> void {
    if (firstRun) {
        testTerrain.createTriangles();
        firstRun = 0;
    }

    auto &engine = SDLEngine::Engine::get();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);



    glColor3f(1, 1, 1);
    glPushMatrix();
    testTerrain.render(0);
    glPopMatrix();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
    
    SDL_GL_SwapWindow(engine.window.get());
}

auto GLDisplay::update(double dt) -> void {
    // dt == delta time
}

auto GLDisplay::get() -> GLDisplay & {
    static auto instance = GLDisplay{};

    return instance;
}

auto GLDisplay::drawRectangle(float width, float height) -> void {
    glBegin(GL_LINE_LOOP);
    glVertex3f(-0.5f * width, 0.5f * height, 0);
    glVertex3f(0.5f * width, 0.5f * height, 0);
    glVertex3f(0.5f * width, -0.5f * height, 0);
    glVertex3f(-0.5f * width, -0.5f * height, 0);
    glEnd();
}