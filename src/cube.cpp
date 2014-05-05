#include "cube.hpp"

#include <iostream>
#include <random>

namespace pc {

    PhotoCube::Tile::Tile() {
        quad_.setPrimitiveType(sf::Quads);
        quad_.resize(4);

        for (unsigned int i = 0;i < 4;i++) {
            neighbours_[i] = nullptr;
            current_neighbours_[i] = nullptr;
        }
    }

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

        auto img_size = texture_.getSize();

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
                auto x = quad_w * i, y = quad_h * j;
                auto &tile = tiles_[index];
                tile.quad_[0].position = sf::Vector2f(x,y);
                tile.quad_[1].position = sf::Vector2f(x+quad_w,y);
                tile.quad_[2].position = sf::Vector2f(x+quad_w,y+quad_h);
                tile.quad_[3].position = sf::Vector2f(x,y+quad_h);

                x = tex_quad_w * i, y = tex_quad_h * j;
                tile.quad_[0].texCoords = sf::Vector2f(x,y);
                tile.quad_[1].texCoords = sf::Vector2f(x+tex_quad_w,y);
                tile.quad_[2].texCoords = sf::Vector2f(x+tex_quad_w,y+tex_quad_h); 
                tile.quad_[3].texCoords = sf::Vector2f(x,y+tex_quad_h); 

                if (i > 0) {
                    tile.neighbours_[LEFT] = &tiles_[index-1];
                }
                if (i < x_count - 1) {
                    tile.neighbours_[RIGHT] = &tiles_[index+1];
                }
                if (j > 0) {
                    tile.neighbours_[UP] = &tiles_[index-y_count_];
                }
                if (j < x_count - 1) {
                    tile.neighbours_[DOWN] = &tiles_[index+y_count_];
                }
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

                // Setup current neighbours
                if (j > 0) {
                    tile.current_neighbours_[LEFT] = &tiles_[index-1];
                }
                if (j < x_count - 1) {
                    tile.current_neighbours_[RIGHT] = &tiles_[index+1];
                }
                if (i > 0) {
                    tile.current_neighbours_[UP] = &tiles_[index-x_count_];
                }
                if (i < x_count - 1) {
                    tile.current_neighbours_[DOWN] = &tiles_[index+x_count_];
                }

                // Don't need to transform empty tile
                //if (&tile == empty_tile_) continue;

                // This is the randomised position of the tile
                auto x = quad_w * j, y = quad_h * i;
                // This is the actual position of the tile
                auto position = tiles_[index].quad_[0].position;
                // This transforms the tile so it matches the randomised pos
                auto t_x = x - position.x, t_y = y - position.y;
                tile.transform_.translate(t_x, t_y);
            }
        }
    }

    void PhotoCube::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.texture = &texture_;
        for (unsigned int i = 0; i < x_count_ * y_count_;i++) {
            const auto &tile = tiles_[i];
            sf::RenderStates state;
            state.transform = tile.transform_;
            if (&tile != empty_tile_) {
                state.texture = &texture_;
            }
            target.draw(tile.quad_, state);
        }

        //states.transform.scale(10, 10);
        //states.transform.translate(-1.f * (width_), -1.f * (height_));
        //target.draw(preview_, states);
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
        src->transform_.translate(t.x, t.y);

        empty_tile_->transform_.translate(-t.x, -t.y);

        for (unsigned int i = 0; i < 4; i++) {
            // Update all neighbouring pointers for both tiles
            swap_neighbours(empty_tile_->current_neighbours_[i], empty_tile_, src);
            swap_neighbours(src->current_neighbours_[i], src, empty_tile_);

            // Swap tiles making sure they aren't their own neighbour
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
