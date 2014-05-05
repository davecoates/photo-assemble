#pragma once

#include <SFML/Graphics.hpp>

#include <vector>


namespace pc {

        typedef const unsigned short int Direction;
        static constexpr Direction LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3;
    class PhotoCube : public sf::Drawable,  public sf::Transformable {


        struct Tile {
            sf::VertexArray quad_;

            // Neighbours when in correct order
            Tile* neighbours_[4];

            // Neighbours based on current order
            Tile* current_neighbours_[4];

            sf::Transform transform_;


            Tile();
        };

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
