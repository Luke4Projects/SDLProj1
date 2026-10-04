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
