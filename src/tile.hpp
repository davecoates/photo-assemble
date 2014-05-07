#pragma once

#include <SFML/Graphics.hpp>

namespace pc {

    enum Direction {
        LEFT = 0,
        RIGHT = 1,
        UP = 2,
        DOWN = 3
    };

    class Tile : sf::Drawable {

        typedef unsigned int uint;

        private:
            sf::VertexArray quad_;
            sf::VertexArray border_;

            // Neighbours when in correct order
            Tile* neighbours_[4];

            // Transform this tile from it's original position to it's current
            // position
            sf::Transform transform_;

            // Width and height of tile and width and height of texture 
            uint width_, height_, tex_width_, tex_height_;

            bool is_home_ = false;

            // Is this tile in it's correct position?
            bool check_is_home();

        public:

            // Neighbours based on current order
            Tile* current_neighbours_[4];

            Tile();

            // Init this tile
            // @param sf::Vector2f pos position of quad
            // @param uint w width of quad
            // @param uint h height of quad
            // @param sf::Vector2f pos position within texture 
            // @param uint tex_w width of texture
            // @param uint tex_h height of texture
            void init(sf::Vector2f pos, uint w, uint h, sf::Vector2f tex_pos, uint tex_w, uint tex_h);

            // Translate tile to a specific position
            void translate_to(sf::Vector2f pos);

            // Translate tile by specific amount
            void translate(sf::Vector2f t);

            bool is_home() { return is_home_; }

            void draw_border(sf::RenderTarget& target, sf::RenderStates) const;

            void set_neighbour(Direction dir, Tile* n) {
                current_neighbours_[dir] = n;
            }

            void update_neighbour(Tile *find, Tile *replace);

            // Calls update_neighbour on each of this tiles current neighbours.
            // This is to tell them that this tile has changed it's position so
            // they need to point at the new tile instead
            void swap_neighbours(Tile *new_tile);

            void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };

}
