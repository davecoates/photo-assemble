#include <SFML/Graphics.hpp>
#include "app.hpp"

using namespace pc;

int main()
{
    auto app = std::unique_ptr<Application>(new Application());
    app->init("Robo High", 800, 600, 32, false);
    app->run();

    return 0;
}
