#include "box.h"

namespace qdg {
    Box::Box(sf::Color fill, sf::Color hover, sf::Vector2f size) :
        fill_color(fill), hover_color(hover),
        box(size)
        {
            box.setOutlineColor(fill);
            box.setFillColor(color_grey);
            box.setOutlineThickness(5.f);
            text.setFillColor(sf::Color::Black);
        }

    void Box::fill() {
        state = State::Filled;
        box.setFillColor(fill_color);
    }

    void Box::hover() {
        state = State::Hovered;
        box.setFillColor(hover_color);
    }

    void Box::blank() {
        state = State::Blank;
        box.setFillColor(color_grey);
    }

    bool Box::check_hover(const sf::Vector2f mouse_pos) {
        if (bounds().contains(mouse_pos) && state != State::Filled) {
            hover();
            return true;
        }
        else if (state != State::Filled) {
            blank();
        }
        return false;
    }

    bool Box::check_clicked(const sf::Vector2f mouse_pos) {
        if (bounds().contains(mouse_pos)) {
            switch(state) {
                case State::Filled:
                    blank();
                    return true;
                default:
                    fill();
                    return true;
            }
        }

        return false;
    }


    void Box::set_font(sf::Font *fnt) {
        font= fnt;
        text.setFont(*font);
    }

    void Box::set_label(const std::string& label) {
        text.setString(label);
        recenter_label();
    }

    void Box::set_pos(sf::Vector2f pos) {
        box.setPosition(pos);
        recenter_label();

    }

    void Box::recenter_label() {
        auto box_rect = box.getGlobalBounds();
        auto rect = text.getGlobalBounds();
        text.setOrigin(rect.width / 2.f, rect.height);
        text.setPosition(box_rect.left + box_rect.width / 2.f, 
                          box_rect.top + box_rect.height / 2.f);
    }


    void Box::draw(sf::RenderWindow& window) {
        window.draw(box);
        window.draw(text);
    }

    sf::FloatRect Box::bounds() {
        return box.getGlobalBounds();
    }
}
