#include "Engine/Engine.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include <SDL2/SDL.h>

#include "View/GLDisplay.hpp"
#include "View/Camera.h"

using SDLEngine::Engine;
using std::runtime_error;
using std::string;
using View::GLDisplay;

/**
 * @brief The game engine main loop
 */
auto Engine::run() -> void {
    auto &engine  = Engine::get();
    auto &display = GLDisplay::get();

    auto frameCount    = 0l;
    auto lastFpsUpdate = 0.0;

    auto time      = engine.getTime();
    auto oldTime   = 0.0;
    auto deltaTime = 0.0;

    while (engine.getIsRunning()) {
        ++frameCount;
        oldTime   = time;
        time      = engine.getTime();
        deltaTime = time - oldTime;

        if (time - lastFpsUpdate >= FPS_UPDATE_INTERVAL) {
            engine.fps    = frameCount / (time - lastFpsUpdate);
            lastFpsUpdate = time;
            frameCount    = 0;
        }

        engine.processInput();

        display.display();
        // run display and update methods here
    }
}

/**
 * @brief Game engine default constructor, sets up all variables and settings required for operation
 */
Engine::Engine() {
    // Start SDL.
    auto status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    if (status != 0) {
        throw runtime_error{string{"Unable to initialize SDL: "} + SDL_GetError()};
    }

    // Set OpenGL settings.
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
#else
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
#endif

    // Get display size.
    auto display = SDL_DisplayMode{};
    SDL_GetCurrentDisplayMode(0, &display);

    // Create window.
    this->window = Engine::Window{
        SDL_CreateWindow("Window Title", display.w / 4, display.h / 4,
                         display.w / 2, display.h / 2,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI),
        &SDL_DestroyWindow};

    if (this->window.get() == nullptr) {
        throw runtime_error{string{"Unable to create window: "} + SDL_GetError()};
    }

    // Create OpenGL context.
    this->context = Engine::Context{SDL_GL_CreateContext(this->window.get()),
                                    &SDL_GL_DeleteContext};

    if (this->context.get() == nullptr) {
        throw runtime_error{string{"Unable to create OpenGL context: "} +
                            SDL_GetError()};
    }

    SDL_GL_MakeCurrent(this->window.get(), this->context.get());

    // Enable Vsync.
    constexpr auto ENABLE_VSYNC = 1;
    SDL_GL_SetSwapInterval(ENABLE_VSYNC);

    // Capture the mouse.
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

/**
 * @brief Engine default destructor
 * Safely closes Engine and frees memory
 */
Engine::~Engine() {
    SDL_Quit();
}

/**
 * @brief Returns the current instance of the engine
 * @return The current engine instance
 */
auto Engine::get() -> Engine & {
    static auto instance = Engine{};

    return instance;
}

/**
 * @brief Checks to see if the engine is currently running
 * @return A boolean, returns true if the engine is running
 */
auto Engine::getIsRunning() const -> bool {
    return this->isRunning;
}

/**
 * @brief Handles SDL2 events regarding keyboard key presses, works by sending
 * the events to the currently set game state
 * @param event The SDL2 event being read from
 */
auto Engine::handleKeyPress(SDL_Event &event) -> void {
    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE: {
            SDL_Quit();
            exit(0);
        } break;
        default: break;
    }
}

/**
 * @brief Handles SDL2 events regarding keyboard key releases, works by sending
 * the events to the currently set game state
 * @param event The SDL2 event being read from
 */
auto Engine::handleKeyRelease(SDL_Event &event) -> void {

    switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_P: {
            // Do Foo
        } break;
        default: break;
    }
}

/**
 * @brief Handles SDL2 events regarding mouse movement, works by sending
 * the events to the currently set game state
 * @param event The SDL2 event being read from
 */
auto Engine::handleMouseMovement(SDL_Event &event) -> void {
    this->mouse.x = static_cast<float>(event.motion.xrel);
    this->mouse.y = static_cast<float>(event.motion.yrel);

}

/**
 * @brief Handles SDL2 events regarding mouse button presses, works by sending
 * the events to the currently set game state
 * @param event The SDL2 event being read from
 */
auto Engine::handleMouseButtonPress(SDL_Event &event) -> void {
    // int numClicks =
    //     event.button.clicks; // Number of clicks received as event   e.g. 1 =
    //                          // single click, 2 = double click
    // int releaseXPos = event.button.x; // X-position of mouse when pressed
    // int releaseYPos = event.button.y; // Y-position of mouse when pressed
    switch (event.button.button) {
        case SDL_BUTTON_LEFT: break;
        case SDL_BUTTON_RIGHT: break;
        case SDL_BUTTON_MIDDLE: break;
        default: break;
    }
}

/**
 * @brief Handles SDL2 events regarding mouse button releases, works by sending
 * the events to the currently set game state
 * @param event The SDL2 event being read from
 */
auto Engine::handleMouseButtonRelease(SDL_Event &event) -> void {
    // int numClicks =
    //     event.button.clicks; // Number of clicks received as event   e.g. 1 =
    //                          // single click, 2 = double click
    // int releaseXPos = event.button.x; // X-position of mouse when pressed
    // int releaseYPos = event.button.y; // Y-position of mouse when pressed

    switch (event.button.button) {
        case SDL_BUTTON_LEFT: break;
        case SDL_BUTTON_RIGHT: break;
        case SDL_BUTTON_MIDDLE: break;
        default: break;
    }
}

/**
 * @brief Handles SDL2 events regarding mouse wheel motion, works by sending
 * the events to the currently set game state
 * @param event The SDL2 event being read from
 */
auto Engine::handleMouseWheelMotion([[maybe_unused]] SDL_Event &event) -> void {
    // int amountScrolledX = event.wheel.x; // Amount scrolled left or right
    // int amountScrolledY = event.wheel.y; // Amount scrolled up or down
}

/**
 * @brief Parses all SDL2 events and sends them to the correct function
 * to process them based on their event type
 */
auto Engine::processInput() -> void {
    auto event = SDL_Event{};

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN: {
                this->handleKeyPress(event);
            } break;
            case SDL_KEYUP: {
                this->handleKeyRelease(event);
            } break;
            case SDL_MOUSEBUTTONDOWN: {
                this->handleMouseButtonPress(event);
            } break;
            case SDL_MOUSEBUTTONUP: {
                this->handleMouseButtonRelease(event);
            } break;
            case SDL_MOUSEMOTION: {
                this->handleMouseMovement(event);
            } break;
            case SDL_MOUSEWHEEL: {
                this->handleMouseWheelMotion(event);
            } break;
            case SDL_QUIT: {
                this->isRunning = false;
            } break;
            default: break;
        }
    }
}


auto Engine::getTime() const -> double {
    return static_cast<double>(SDL_GetPerformanceCounter()) /
           static_cast<double>(SDL_GetPerformanceFrequency());
}