#pragma once

#include <SFML/Graphics.hpp>

#include <vector>


namespace pc {

    class PhotoCube : public sf::Drawable,  public sf::Transformable {

        enum struct Direction {
            LEFT,
            RIGHT,
            UP,
            DOWN
        };

        private:
            sf::VertexArray face_;
            sf::VertexArray grid_;
            sf::VertexArray squares_;

            sf::Vertex *empty_square_;

            //std::vector<sf::Transform> square_transforms_;

            sf::Texture texture_;

            unsigned int width_, height_;
            unsigned int x_count_, y_count_;

            virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

            sf::Vertex* get_neighbour_square(sf::Vertex *square, const Direction &dir);
            bool swap_empty_square(const Direction &dir);

        public:
            PhotoCube(unsigned int width, unsigned int height, unsigned int x_count, unsigned int y_count);

            void update(const sf::Time &t);

            void swipe_left();
            void swipe_right();
            void swipe_up();
            void swipe_down();

    };

}
