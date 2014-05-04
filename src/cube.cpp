#include "cube.hpp"

#include <iostream>
#include <random>

namespace pc {

    PhotoCube::PhotoCube(unsigned int width, unsigned int height, unsigned int x_count, unsigned int y_count) 
        : width_(width), height_(height), x_count_(x_count), y_count_(y_count) 
    {
        std::cout << width_ << " x " << height_ << ", " << x_count_ << " x " << y_count << std::endl;

        auto quad_w = width_ / x_count,
             quad_h = height_ / y_count;

        face_.resize(x_count_ * y_count_ * 4);
        face_.setPrimitiveType(sf::Quads);

        bool s = true;

        if (!texture_.loadFromFile("../resources/trafalgar_square.jpg")) {
            throw std::runtime_error("Failed to load image");
        }

        auto img_size = texture_.getSize();
        auto tex_quad_w = img_size.x / x_count,
             tex_quad_h = img_size.y / y_count;

        std::cout << "Tex Quad w: " << tex_quad_w << std::endl;

        squares_.resize(x_count * y_count * 4);
        squares_.setPrimitiveType(sf::Quads);
        //square_transforms_.resize(x_count * y_count);

        std::vector<sf::Vertex*> texture_pts(x_count*y_count);

        for (unsigned int i = 0; i < x_count; i++) {
            for (unsigned int j = 0; j < y_count; j++) {
                auto index = i*y_count*4+j*4;
                auto x = quad_w * i, y = quad_h * j;
                face_[index].position = sf::Vector2f(x,y);
                face_[index+1].position = sf::Vector2f(x+quad_w,y);
                face_[index+2].position = sf::Vector2f(x+quad_w,y+quad_h);
                face_[index+3].position = sf::Vector2f(x,y+quad_h);

                squares_[index].position = sf::Vector2f(x,y);
                squares_[index+1].position = sf::Vector2f(x+quad_w,y);
                squares_[index+2].position = sf::Vector2f(x+quad_w,y+quad_h);
                squares_[index+3].position = sf::Vector2f(x,y+quad_h);

                //auto color = s ? sf::Color::Green : sf::Color::Yellow;
                //s = !s;
                //face_[index].color = color;
                //face_[index+1].color = color;
                //face_[index+2].color = color;
                //face_[index+3].color = color;
                x = tex_quad_w * i, y = tex_quad_h * j;
                face_[index].texCoords = sf::Vector2f(x,y);
                face_[index+1].texCoords = sf::Vector2f(x+tex_quad_w,y);
                face_[index+2].texCoords = sf::Vector2f(x+tex_quad_w,y+tex_quad_h); 
                face_[index+3].texCoords = sf::Vector2f(x,y+tex_quad_h); 

                texture_pts[i*y_count+j] = &face_[index];

                //square_transforms_[i*y_count+j].translate(x, 0);
            }
        }

        std::shuffle(std::begin(texture_pts), std::end(texture_pts), std::default_random_engine());


        for (unsigned int i = 0; i < x_count; i++) {
            for (unsigned int j = 0; j < y_count; j++) {
                auto index = i*y_count*4+j*4;
                auto tex_index = i*y_count+j;
                auto ptr = texture_pts[tex_index];
                if (i == 0 && j == 0) continue;
                squares_[index].texCoords = (*ptr++).texCoords;
                squares_[index+1].texCoords = (*ptr++).texCoords;
                squares_[index+2].texCoords = (*ptr++).texCoords;
                squares_[index+3].texCoords = (*ptr++).texCoords;
            }
        }

        empty_square_ = &squares_[0];

        // Draw grid
        grid_.resize((x_count_ + 1 )* (y_count_ + 1) * 4);
        grid_.setPrimitiveType(sf::Quads);

        unsigned int line_h = 1, line_w = 1;
        for (unsigned int i = 0; i <= x_count; i++) {
            auto index = i*4;
            unsigned int x = 0, y = quad_w * i;
            grid_[index].position = sf::Vector2f(x,y);
            grid_[index+1].position = sf::Vector2f(x+width_,y);
            grid_[index+2].position = sf::Vector2f(x+width_,y+line_h);
            grid_[index+3].position = sf::Vector2f(x,y+line_h);

            auto color = s ? sf::Color::Green : sf::Color::Yellow;
            s = !s;
            grid_[index].color = color;
            grid_[index+1].color = color;
            grid_[index+2].color = color;
            grid_[index+3].color = color;
        }

        for (unsigned int i = 0; i <= y_count; i++) {
            auto index = i*y_count*4 + (1+x_count)*4;
            unsigned int x = quad_w * i, y = 0;
            grid_[index].position = sf::Vector2f(x,y);
            grid_[index+1].position = sf::Vector2f(x+line_w,y);
            grid_[index+2].position = sf::Vector2f(x+line_w,y+height_+line_w);
            grid_[index+3].position = sf::Vector2f(x,y+height_+line_w);

            auto color = s ? sf::Color::Green : sf::Color::Yellow;
            s = !s;
            grid_[index].color = color;
            grid_[index+1].color = color;
            grid_[index+2].color = color;
            grid_[index+3].color = color;
        }



    }

    void PhotoCube::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        //auto transform = this->getTransform();
        //states.shader = &shader;
        //states.transform = transform;
        //target.draw(shape_, states);
        states.texture = &texture_;
        //for (unsigned int i = 0; i < x_count_ * y_count_;i++) {
            //auto s = states;
            //s.transform.combine(square_transforms_[i]);
            //target.draw(squares_[i], 4, sf::Quads, s);
        //}
        target.draw(squares_, states);
        target.draw(grid_, states);

        states.transform.scale(0.5, 0.5);
        states.transform.translate(-1.f * (width_), -1.f * (height_));
        target.draw(face_, states);
    }

    static void copy_and_clear_texcoords(sf::Vertex *dst, sf::Vertex *src) {
        for (unsigned int i = 0;i <= 3;i++) {
            (dst+i)->texCoords.x = (src+i)->texCoords.x;
            (dst+i)->texCoords.y = (src+i)->texCoords.y;
            (src+i)->texCoords.x = 0;;
            (src+i)->texCoords.y = 0;;
        }
    }

    bool PhotoCube::swap_empty_square(const Direction &dir) {
        auto src = get_neighbour_square(empty_square_, dir);
        if (src == nullptr) {
            return false;
        }
        copy_and_clear_texcoords(empty_square_, src);
        empty_square_ = src;
        return true;
    }

    // Get neighbour of square in direction dir
    // Returns nullptr if no such neighbour
    sf::Vertex* PhotoCube::get_neighbour_square(sf::Vertex *square, const Direction &dir) {
        int offset;
        auto quad_w = width_ / x_count_,
             quad_h = height_ / y_count_;
        switch (dir) {
            case Direction::RIGHT:
                if (square->position.x >= (width_ - quad_w)) {
                    return nullptr;
                }
                offset = y_count_;
                break;
            case Direction::LEFT:
                if (square->position.x <= 0) {
                    return nullptr;
                }
                offset = -y_count_;
                break;
            case Direction::DOWN:
                if (square->position.y >= (height_ - quad_h)) {
                    return nullptr;
                }
                offset = 1;
                break;
            case Direction::UP:
                if (square->position.y <= 0) {
                    return nullptr;
                }
                offset = -1;
                break;
        }

        // Multiple by number of vertices per square
        offset *= 4;

        return square + offset;
    }

    void PhotoCube::swipe_left() {
        // Swipe left means swap empty square with square to the right
        swap_empty_square(Direction::RIGHT);
    }

    void PhotoCube::swipe_right() {
        swap_empty_square(Direction::LEFT);
    }

    void PhotoCube::swipe_up() {
        swap_empty_square(Direction::DOWN);
    }

    void PhotoCube::swipe_down() {
        swap_empty_square(Direction::UP);
    }


}
