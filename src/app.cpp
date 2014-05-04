#include "app.hpp"
#include "cube.hpp"
#include <iostream>

using namespace std;

namespace pc {

    ///////////////////////////////////////////////////////////////////////////
    void Application::init(const string title, uint width, uint height, uint depth, bool fullscreen) 
    {
        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        auto style = sf::Style::Default;
        if (fullscreen) {
            style = sf::Style::Fullscreen;
        }
        sf::VideoMode vm(width, height, depth);
        window_ = std::unique_ptr<sf::RenderWindow>(
                new sf::RenderWindow(vm, title, style, settings));
        window_->setFramerateLimit(60);
    }

    ///////////////////////////////////////////////////////////////////////////
    void Application::run()
    {
        unsigned int width = 800, height = 600;
        sf::View view;
        sf::FloatRect viewRect(0, 0, 800, 600);
        view.reset(viewRect);
        window_->setView(view);

        sf::Clock clock;

        auto cube_w = 300, cube_h= 300;

        PhotoCube cube(cube_w, cube_h, 5, 5);

        sf::Transform center_transform;
        center_transform.translate(width/2 - cube_w/2, height/2 - cube_h/2);

        auto background = sf::VertexArray(sf::Quads, 4);
        background[0].position = sf::Vector2f(0,0);
        background[1].position = sf::Vector2f(800,0);
        background[2].position = sf::Vector2f(800,600);
        background[3].position = sf::Vector2f(0,600);

        auto top_color = sf::Color(161,229,253);
        auto bottom_color = sf::Color(0,188,255);

        background[0].color = top_color;
        background[1].color = top_color;
        background[2].color = bottom_color;
        background[3].color = bottom_color;

        sf::Vector2i drag_start, drag_end, dir;
        bool is_dragging = false;

        while (window_->isOpen())
        {
            sf::Event event;
            while (window_->pollEvent(event))
            {

                switch (event.type) {
                    case sf::Event::Closed:
                        window_->close();
                        break;
                    case sf::Event::MouseButtonPressed:
                        is_dragging = true;
                        drag_start.x = event.mouseButton.x;
                        drag_start.y = event.mouseButton.y;
                        break;
                    case sf::Event::MouseButtonReleased:
                        is_dragging = false;
                        drag_end.x = event.mouseButton.x;
                        drag_end.y = event.mouseButton.y;
                        dir = drag_end - drag_start;
                        if (abs(dir.x) > abs(dir.y)) {
                            if (dir.x < 0) {
                                cube.swipe_left(drag_start, drag_end);
                            } else {
                                cube.swipe_right(drag_start, drag_end);
                            }
                        } else {
                            if (dir.y < 0) {
                                cube.swipe_up(drag_start, drag_end);
                            } else {
                                cube.swipe_down(drag_start, drag_end);
                            }
                        }
                        break;

                    default:
                        break;
                }

            }

            window_->clear(sf::Color::White);

            window_->draw(background);
            window_->draw(cube, center_transform);

            window_->display();
        }

    }


}
