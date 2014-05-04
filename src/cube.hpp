#pragma once

#include <SFML/Graphics.hpp>

#include <vector>


namespace pc {

    class PhotoCube : public sf::Drawable,  public sf::Transformable {
        private:
            sf::VertexArray face_;
            sf::VertexArray grid_;
            sf::VertexArray squares_;

            //std::vector<sf::Transform> square_transforms_;

            sf::Texture texture_;

            unsigned int width_, height_;
            unsigned int x_count_, y_count_;

            virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

        public:
            PhotoCube(unsigned int width, unsigned int height, unsigned int x_count, unsigned int y_count);

            void update(const sf::Time &t);

            void swipe_left(sf::Vector2i start, sf::Vector2i end);
            void swipe_right(sf::Vector2i start, sf::Vector2i end);
            void swipe_up(sf::Vector2i start, sf::Vector2i end);
            void swipe_down(sf::Vector2i start, sf::Vector2i end);

    };

}
