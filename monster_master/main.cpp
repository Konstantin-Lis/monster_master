#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <tuple>

class Player {
private:
    int speed;
    std::vector<int> position;
    int size;

public:
    int get_speed() {
        return this->speed;
    }
    void set_speed(int new_speed) {
        this->speed = new_speed;
    }

    std::vector<int> get_position() {
        return this->position;
    }
    void set_position(std::vector<int> new_pos) {
        this->position = new_pos;
    }

    int get_size() {
        return this->size;
    }
    void set_size(int new_size) {
        this->size = new_size;
    }

    void move(sf::RenderWindow& window, std::vector<int> move_dir, std::vector<std::vector<int>> map, int tile_size) {
        std::vector<int> pos = this->get_position();
        int pos_x = pos[0];
        int pos_y = pos[1];
        int speed = this->speed;

        int player_left_chor = pos_x / tile_size;
        int player_right_chor = (pos_x + 2*this->get_size()) / tile_size;
        int player_top_chor = pos_y / tile_size;
        int player_bottom_chor = (pos_y + 2*this->get_size()) / tile_size;

        if (move_dir[0] == 1) {
            pos_y -= speed;
            player_top_chor = pos_y / tile_size;
            if (map[player_top_chor][player_left_chor] || map[player_top_chor][player_right_chor]) {
                pos_y = (player_top_chor + 1) * tile_size + 1;
                player_top_chor++;
            }
        }
        if (move_dir[1] == 1) {
            pos_x -= speed;
            player_left_chor = pos_x / tile_size;
            if (map[player_top_chor][player_left_chor] || map[player_bottom_chor][player_left_chor]) {
                pos_x = (player_left_chor + 1) * tile_size + 1;
                player_left_chor++;
            }
        }
        if (move_dir[2] == 1) {
            pos_y += speed;
            player_bottom_chor = (pos_y + 2 * this->get_size()) / tile_size;
            if (map[player_bottom_chor][player_left_chor] || map[player_bottom_chor][player_right_chor]) {
                pos_y = player_bottom_chor * tile_size - 2*this->get_size() - 1;
                player_bottom_chor--;
            }
        }
        if (move_dir[3] == 1) {
            pos_x += speed;
            player_right_chor = (pos_x + 2 * this->get_size()) / tile_size;
            if (map[player_top_chor][player_right_chor] || map[player_bottom_chor][player_right_chor]) {
                pos_x = player_right_chor * tile_size - 2 * this->get_size() - 1;
                player_right_chor--;
            }
        }

        std::vector<int> new_pos = { pos_x, pos_y };
        this->set_position(new_pos);
    }
};


class Adopted_Monster {
private:
    int speed;
    std::vector<int> position;
    int size;

public:
    int get_speed() {
        return this->speed;
    }
    void set_speed(int new_speed) {
        this->speed = new_speed;
    }

    std::vector<int> get_position() {
        return this->position;
    }
    void set_position(std::vector<int> new_pos) {
        this->position = new_pos;
    }

    int get_size() {
        return this->size;
    }
    void set_size(int new_size) {
        this->size = new_size;
    }

    void move_to(sf::RenderWindow& window, std::vector<int> aim, std::vector<std::vector<int>> map, int tile_size) {
        std::vector<int> pos = this->get_position();
        
        // Задаём базовые хранилища информации
        std::vector<std::vector<int>> queue = {pos};
        std::unordered_map<std::vector<int>, std::vector<int>> child_parent;
        std::vector<std::vector<int>> result_way;

        // Ищем цель
        int point_num = 0;
        while (true) {
            //std::pair<int, int> actual_point = queue[point_num];
            std::vector<int> actual_point = queue[point_num];
        }



    }
};





void draw_map(
    sf::RenderWindow& window,
    std::vector<std::vector<int>> map,
    float tile_size,
    std::vector<int> pl_pos
) {
    int size_x = map[0].size();
    int size_y = map.size();

    sf::Vector2u size = window.getSize();
    float width = float(size.x);
    float height = float(size.y);

    window.clear();
    for (int chor_x = 0; chor_x < size_x; chor_x++) {
        for (int chor_y = 0; chor_y < size_y; chor_y++) {
            sf::RectangleShape rect(sf::Vector2f(tile_size, tile_size));
            if (map[chor_y][chor_x] == 1) {
                rect.setFillColor(sf::Color::Red);
            }
            else if (map[chor_y][chor_x] == 0) {
                rect.setFillColor(sf::Color::Yellow);
            }
            rect.setPosition(tile_size * chor_x - pl_pos[0] + width/2, tile_size * chor_y - pl_pos[1] + height/2);
            window.draw(rect);
        }
    }
}





int main()
{
    sf::RenderWindow window(sf::VideoMode(), L"Новый проект", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    int win_width = int(window.getSize().x);
    int win_height = int(window.getSize().y);

    int speed = 3;

    std::vector<int> pos = { win_width / 2, win_height / 2 };
    std::vector<int> move_dir = { 0, 0, 0, 0 };

    int tile_size = 80;
    std::vector<std::vector<int>> map = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    Player pl;
    pl.set_position(pos);
    pl.set_speed(speed);
    pl.set_size(20);

    sf::CircleShape shape(pl.get_size());
    shape.setPosition(pos[0], pos[1]);
    shape.setFillColor(sf::Color::Magenta);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                    window.close();
                }
                else if (event.key.scancode == sf::Keyboard::W) {
                    move_dir[0] = 0;
                }
                else if (event.key.scancode == sf::Keyboard::A) {
                    move_dir[1] = 0;
                }
                else if (event.key.scancode == sf::Keyboard::S) {
                    move_dir[2] = 0;
                }
                else if (event.key.scancode == sf::Keyboard::D) {
                    move_dir[3] = 0;
                }
            }

            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::W) {
                    move_dir[0] = 1;
                }
                else if (event.key.scancode == sf::Keyboard::A) {
                    move_dir[1] = 1;
                }
                else if (event.key.scancode == sf::Keyboard::S) {
                    move_dir[2] = 1;
                }
                else if (event.key.scancode == sf::Keyboard::D) {
                    move_dir[3] = 1;
                }
            }

            else if (event.type == sf::Event::MouseButtonReleased) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                std::vector<int> player_pos = pl.get_position();
                int new_player_x = mouse_pos.x + player_pos[0] - win_width / 2 - pl.get_size();
                int new_player_y = mouse_pos.y + player_pos[1] - win_height / 2 - pl.get_size();
                std::vector<int> new_pos = { new_player_x, new_player_y };
                pl.set_position(new_pos);
            }

        }

        pl.move(window, move_dir, map, tile_size);

        std::vector<int> pos = pl.get_position();

        draw_map(window, map, tile_size, pl.get_position());
        window.draw(shape);
        window.display();
    }
    return 0;
}

