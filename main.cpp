#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "header/render.hpp"
#include "header/game.hpp"

int main(int argc, char* argv) {

    Game game;

    bool running = true;
    while (running)
    {
        running = game.update();
    }

    game.quit();
}
