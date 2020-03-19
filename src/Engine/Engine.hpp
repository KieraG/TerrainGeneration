#pragma once

#include <memory>

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace SDLEngine {
    class Engine {
      public:
        using Window  = std::shared_ptr<SDL_Window>;
        using Context = std::shared_ptr<void>;

        static constexpr auto FPS_UPDATE_INTERVAL = 0.5;

        /* Mouse movement. */
        glm::vec2 mouse = {};

        /* SDL handles. */
        Window window   = nullptr;
        Context context = nullptr;

        /**
         * @brief The current FPS
         */
        double fps = 0.0;

      private:
        bool isRunning = true;

        auto getTime() const -> double;

        Engine();

      public:
        Engine(Engine &&)      = default;
        Engine(const Engine &) = delete;
        ~Engine();

        static auto get() -> Engine &;
        static auto run() -> void;

        /**
         * @brief Overloaded assignment operator, set to default overload
         */
        auto operator=(Engine &&) -> Engine & = default;

        /**
         * @brief Overloaded const assignment operator, set to delete overload
         */
        auto operator=(const Engine &) -> Engine & = delete;

        auto handleMouseMovement(SDL_Event &event) -> void;

        auto handleMouseButtonPress(SDL_Event &event) -> void;

        auto handleMouseButtonRelease(SDL_Event &event) -> void;

        auto handleMouseWheelMotion(SDL_Event &event) -> void;

        auto handleKeyPress(SDL_Event &event) -> void;

        auto handleKeyRelease(SDL_Event &event) -> void;

        auto getIsRunning() const -> bool;

        auto processInput() -> void;
    };
}