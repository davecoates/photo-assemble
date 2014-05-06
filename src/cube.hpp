#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "tile.hpp"


namespace pc {


    class PhotoCube : public sf::Drawable,  public sf::Transformable {


        private:
            std::vector<Tile> tiles_;

            sf::VertexArray preview_;

            Tile *empty_tile_;

            sf::Texture texture_;

            unsigned int width_, height_;
            unsigned int x_count_, y_count_;

            virtual void draw(sf::RenderTarget& target, sf::RenderStates) const;

            bool swap_empty_tile(const Direction &dir);

            sf::Vector2f get_slide_transform(const Direction &dir);

            void swap_neighbours(Tile* target, Tile *find, Tile *replace);

        public:
            PhotoCube(unsigned int width, unsigned int height, unsigned int x_count, unsigned int y_count);

            void update(const sf::Time &t);

            void swipe_left();
            void swipe_right();
            void swipe_up();
            void swipe_down();

    };

}
