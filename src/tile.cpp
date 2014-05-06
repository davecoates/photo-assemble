#include "tile.hpp"

namespace pc {

    Tile::Tile() {
        quad_.setPrimitiveType(sf::Quads);
        quad_.resize(4);

        border_.setPrimitiveType(sf::Quads);
        border_.resize(16);

        // TODO: border

        for (unsigned int i = 0;i < 4;i++) {
            neighbours_[i] = nullptr;
            current_neighbours_[i] = nullptr;
        }
    }

    bool Tile::check_is_home() {
        auto t = transform_.transformPoint(quad_[0].position.x, quad_[0].position.y); 

        is_home = t == quad_[0].position;
        return is_home;
    }

    void Tile::init(sf::Vector2f pos, uint w, uint h, sf::Vector2f tex_pos, uint tex_w, uint tex_h) 
    {
        width_ = w;
        height_ = h;
        tex_width_ = tex_w;
        tex_height_ = tex_h;

        auto x = pos.x, y = pos.y;

        quad_[0].position = pos;
        quad_[1].position = sf::Vector2f(x+width_,y);
        quad_[2].position = sf::Vector2f(x+width_,y+height_);
        quad_[3].position = sf::Vector2f(x,y+height_);

        x = tex_pos.x, y = tex_pos.y;
        quad_[0].texCoords = sf::Vector2f(x,y);
        quad_[1].texCoords = sf::Vector2f(x+tex_w,y);
        quad_[2].texCoords = sf::Vector2f(x+tex_w,y+tex_h); 
        quad_[3].texCoords = sf::Vector2f(x,y+tex_h); 
    }

    void Tile::translate_to(sf::Vector2f pos) {
        translate(pos - quad_[0].position);
    }

    void Tile::translate(sf::Vector2f t) {
        transform_.translate(t.x, t.y);
        check_is_home();
    }

    void Tile::swap_neighbours(Tile* new_tile) {
        for (unsigned int i = 0; i < 4; i++) {   
            if (current_neighbours_[i] && current_neighbours_[i] != new_tile) {
                current_neighbours_[i]->update_neighbour(this, new_tile);
            }
        }
    }

    void Tile::update_neighbour(Tile *find, Tile *replace) {
        for (unsigned int i = 0; i < 4; i++) {   
            if (current_neighbours_[i] == find) {
                current_neighbours_[i] = replace;
                return;
            }
        }

    }

    void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform.combine(transform_);
        target.draw(quad_, states);
    }

}
