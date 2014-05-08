#include "cube.hpp"
#include "tile.hpp"

#include <iostream>
#include <random>

namespace pc {

    PhotoCube::PhotoCube(unsigned int width, unsigned int height, unsigned int x_count, unsigned int y_count) 
        : width_(width), height_(height), x_count_(x_count), y_count_(y_count) 
    {
        std::cout << width_ << " x " << height_ << ", " << x_count_ << " x " << y_count << std::endl;

        auto quad_w = width_ / x_count,
             quad_h = height_ / y_count;

        std::cout << "quad: " << quad_w << ", " << quad_h << std::endl;

        if (!texture_.loadFromFile("../resources/trafalgar_square.jpg")) {
            throw std::runtime_error("Failed to load image");
        }
        texture_.setSmooth(true);

        auto img_size = texture_.getSize();

        if (img_size.x > img_size.y) {
            texture_.loadFromFile("../resources/trafalgar_square.jpg", sf::IntRect(0, 0, img_size.y, img_size.y));
            img_size = texture_.getSize();
            std::cout << img_size.x << " " << img_size.y << "<===\n";
        }

        preview_.setPrimitiveType(sf::Quads);
        preview_.resize(4);

        preview_[0].position = sf::Vector2f(0, 0);
        preview_[1].position = sf::Vector2f(1, 0);
        preview_[2].position = sf::Vector2f(1, 1);
        preview_[3].position = sf::Vector2f(0, 1);

        preview_[0].texCoords = sf::Vector2f(0, 0);
        preview_[1].texCoords = sf::Vector2f(img_size.x, 0);
        preview_[2].texCoords = sf::Vector2f(img_size.x, img_size.y);
        preview_[3].texCoords = sf::Vector2f(0, img_size.y);

        auto tex_quad_w = img_size.x / x_count,
             tex_quad_h = img_size.y / y_count;

        std::cout << "Tex Quad w: " << tex_quad_w << std::endl;

        tiles_.resize(x_count * y_count);

        for (unsigned int i = 0; i < x_count; i++) {
            for (unsigned int j = 0; j < y_count; j++) {
                auto index = i*x_count + j;
                auto &tile = tiles_[index];
                auto pos = sf::Vector2f(quad_w * i, quad_h * j);
                auto tex_pos = sf::Vector2f(tex_quad_w * i, tex_quad_h * j); 
                tile.init(pos, quad_w, quad_h, tex_pos, tex_quad_w, tex_quad_h);
            }
        }

        empty_tile_ = &tiles_[0];

        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(std::begin(tiles_), std::end(tiles_), g);


        for (unsigned int i = 0; i < x_count; i++) {
            for (unsigned int j = 0; j < y_count; j++) {
                auto index = i*x_count + j;;
                auto &tile = tiles_[index];

                if (j > 0) {
                    tile.set_neighbour(LEFT, &tiles_[index-1]);
                }
                if (j < x_count - 1) {
                    tile.set_neighbour(RIGHT, &tiles_[index+1]);
                }
                if (i > 0) {
                    tile.set_neighbour(UP, &tiles_[index-x_count_]);
                }
                if (i < x_count - 1) {
                    tile.set_neighbour(DOWN, &tiles_[index+x_count_]);
                }

                // This is the randomised position of the tile
                auto x = quad_w * j, y = quad_h * i;
                // This is the actual position of the tile
                tile.translate_to(sf::Vector2f(x, y));
            }
        }
    }

    bool PhotoCube::is_complete() 
    {
        for (unsigned int i = 0; i < x_count_ * y_count_;i++) {
            if (!tiles_[i].is_home()) {
                return false;
            }
        }
        return true;
    }

    void PhotoCube::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = &texture_;
        for (unsigned int i = 0; i < x_count_ * y_count_;i++) {
            const auto &tile = tiles_[i];
            sf::RenderStates state;
            state.transform = states.transform;
            if (&tile != empty_tile_) {
                state.texture = &texture_;
            }
            tile.draw(target, state);
            //target.draw(tile.quad_, state);
        }

        states.transform = sf::Transform();
        states.transform.scale(200, 200);
        target.draw(preview_, states);
    }

    sf::Vector2f PhotoCube::get_slide_transform(const Direction &dir) {
        float quad_w = width_ / x_count_,
             quad_h = height_ / y_count_;
        switch (dir) {
            case LEFT:
                return sf::Vector2f(quad_w, 0);
            case RIGHT:
                return sf::Vector2f(-quad_w, 0);
            case UP:
                return sf::Vector2f(0, quad_h);
            case DOWN:
                return sf::Vector2f(0, -quad_h);
        }
    }


    void PhotoCube::swap_neighbours(Tile* target, Tile *find, Tile *replace) {
        if (target == nullptr) return;

        for (unsigned int i = 0; i < 4; i++) {   
            if (target->current_neighbours_[i] == find) {
                target->current_neighbours_[i] = replace;
                return;
            }
        }
    }

    bool PhotoCube::swap_empty_tile(const Direction &dir) {
        auto src = empty_tile_->current_neighbours_[dir];
        //std::cout << "Src: " << src << " " << empty_tile_ << std::endl;
        for (unsigned int i = 0; i < 4; i++) {
            //std::cout << i << ": " << empty_tile_->current_neighbours_[i] << std::endl;
        }
        if (src == nullptr) {
            return false;
        }
        auto t = get_slide_transform(dir);
        src->translate(sf::Vector2f(t.x, t.y));

        empty_tile_->translate(-sf::Vector2f(t.x, t.y));

        empty_tile_->swap_neighbours(src);
        src->swap_neighbours(empty_tile_);
        for (unsigned int i = 0; i < 4; i++) {
            // Update all neighbouring pointers for both tiles
            //swap_neighbours(empty_tile_->current_neighbours_[i], empty_tile_, src);
            //swap_neighbours(src->current_neighbours_[i], src, empty_tile_);

            // Update all neighbouring pointers for both tiles
            auto ptr = empty_tile_->current_neighbours_[i];
            empty_tile_->current_neighbours_[i] = src->current_neighbours_[i];
            if (empty_tile_->current_neighbours_[i] == empty_tile_) {
                empty_tile_->current_neighbours_[i] = src;
            }
            if (ptr == src) {
                ptr = empty_tile_;
            }
            src->current_neighbours_[i] = ptr;
        }

        return true;
    }

    void PhotoCube::swipe_left() {
        // Swipe left means swap empty square with square to the right
        swap_empty_tile(RIGHT);
    }

    void PhotoCube::swipe_right() {
        swap_empty_tile(LEFT);
    }

    void PhotoCube::swipe_up() {
        swap_empty_tile(DOWN);
    }

    void PhotoCube::swipe_down() {
        swap_empty_tile(UP);
    }


}
