#include "tile.hpp"
#include <iostream>

namespace pc {

    Tile::Tile() {
        quad_.setPrimitiveType(sf::Quads);
        quad_.resize(4);

        border_.setPrimitiveType(sf::Quads);
        border_.resize(16);

        for (unsigned int i = 0;i < 4;i++) {
            neighbours_[i] = nullptr;
            current_neighbours_[i] = nullptr;
        }
    }

    bool Tile::check_is_home() {
        auto t = transform_.transformPoint(quad_[0].position.x, quad_[0].position.y); 

        is_home_ = t == quad_[0].position;
        return is_home_;
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

        auto border_size = 2;
        auto border_color = sf::Color::Red;

        auto init_border = [this, border_size, border_color] 
            (sf::Vertex* border, sf::Vector2f pos, bool vertical) 
        {
            border[0].position = pos;
            sf::Vector2f v1, v2;
            if (vertical) {
                v1 = sf::Vector2f(0, height_);
                v2 = sf::Vector2f(border_size, 0);
            } else {
                v1 = sf::Vector2f(width_, 0);
                v2 = sf::Vector2f(0, border_size);
            }
            border[1].position = pos + v1;
            border[2].position = border[1].position + v2;
            border[3].position = pos + v2;

            for (uint i = 0; i < 4; i++) {
                border[i].color = border_color;
            }
        };

        auto border_pos = pos - sf::Vector2f(border_size/2.f, border_size/2.f);

        init_border(&border_[0], border_pos, false);
        init_border(&border_[4], border_pos, true);
        init_border(&border_[8], border_pos + sf::Vector2f(width_, 0), true);
        init_border(&border_[12], border_pos + sf::Vector2f(0, height_), false);

        for (uint i = 0; i < 16;i++) {
            auto &b = border_[i];
            std::cout << i << ": " << b.position.x << ", " << b.position.y << std::endl;
        }
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
        if (!current_neighbours_[LEFT] || !(is_home_ && current_neighbours_[LEFT]->is_home())) {
            target.draw(&border_[4], 4, sf::PrimitiveType::Quads, states);
        }
        if (!current_neighbours_[UP] || !(is_home_ && current_neighbours_[UP]->is_home())) {
            target.draw(&border_[0], 4, sf::PrimitiveType::Quads, states);
        }
        if (!current_neighbours_[RIGHT] || !(is_home_ && current_neighbours_[DOWN]->is_home())) {
            target.draw(&border_[8], 4, sf::PrimitiveType::Quads, states);
        }
        if (!current_neighbours_[DOWN] || !(is_home_ && current_neighbours_[DOWN]->is_home())) {
            target.draw(&border_[12], 4, sf::PrimitiveType::Quads, states);
        }

    }

}
