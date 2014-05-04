#pragma once

#include <SFML/Graphics.hpp>

namespace pc {

    class Application {
        public:
            virtual void        init(const std::string title, unsigned int width, unsigned int height, unsigned int depth, bool fullscreen);
            virtual void        run();

        protected:
            std::unique_ptr<sf::RenderWindow>    window_;
    };

}
