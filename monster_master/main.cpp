#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <queue>


class Player {
private:
    int speed;
    std::pair<float, float> position;
    int size;

public:
    int get_speed() {
        return this->speed;
    }
    void set_speed(int new_speed) {
        this->speed = new_speed;
    }

    std::pair<float, float> get_position() {
        return this->position;
    }
    void set_position(std::pair<float, float> new_pos) {
        this->position = new_pos;
    }

    int get_size() {
        return this->size;
    }
    void set_size(int new_size) {
        this->size = new_size;
    }

    void move(sf::RenderWindow& window, std::vector<int> move_dir, std::vector<std::vector<int>> map, int tile_size) {
        std::pair<float, float> pos = this->get_position();
        float pos_x = pos.first;
        float pos_y = pos.second;
        int speed = this->speed;

        int player_left_chor =int( pos_x / tile_size);
        int player_right_chor = int((pos_x + 2*this->get_size()) / tile_size);
        int player_top_chor = int(pos_y / tile_size);
        int player_bottom_chor = int((pos_y + 2*this->get_size()) / tile_size);

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

        std::pair<float, float> new_pos = { pos_x, pos_y };
        this->set_position(new_pos);
    }


    void move_to(std::pair<int, int> aim, std::vector<std::vector<int>> map) {
        std::pair<float, float> pos = this->get_position();
        int speed = this->get_speed();
        float distance = float(std::sqrt(std::pow(aim.first - pos.first, 2) + std::pow(aim.second - pos.second, 2)));
        
        if (distance < speed) {
            this->set_position(aim);
        }
        else {
            float aim_cos = (aim.first - pos.first) / distance;
            float aim_sin = (aim.second - pos.second) / distance;

            float new_pos_x = aim_cos * speed + pos.first;
            float new_pos_y = aim_sin * speed + pos.second;

            std::pair<float, float> new_pos = { new_pos_x, new_pos_y };
            this->set_position(new_pos);
        }
    
    }


};

bool check_line_collision(std::pair<float, float> point_1, std::pair<float, float> point_2, std::vector<std::vector<int>> map, int tile_size) {
    float distance = float(std::sqrt(std::pow(point_1.first - point_2.first, 2) + std::pow(point_1.second - point_2.second, 2)));
    float cos = (point_2.first - point_1.first) / distance;
    float sin = (point_2.second - point_1.second) / distance;

    /*
    Здесь надо потренировать воображение, так как вертикальная ось координат идёт сверху вниз, а не снизу вверх
    Из-за этого положительные значения синуса находятся внизу, а отрицательные вверху, а не наоборот (как в курсе геометрии)
    */

    if (cos >= 0) {
        int left_border = int(point_1.first / tile_size);
        int right_border = int(point_2.first / tile_size) + 1;

        for (int i = left_border+1; i < right_border; i++) {
            float i_y_chor = sin * (i*tile_size - point_1.first)/cos + point_1.second;
            int i_top = int(i_y_chor / tile_size);
            if (map[i_top][i]) {
                return true;
            }
        }
    }
    else if (cos < 0) {
        int right_border = int(point_1.first / tile_size) + 1;
        int left_border = int(point_2.first / tile_size);

        for (int i = right_border - 1; i > left_border; i--) {
            float i_y_chor = sin * (i * tile_size - point_1.first) / cos + point_1.second;
            int i_top = int(i_y_chor / tile_size);
            if (map[i_top][i]) {
                return true;
            }
        }
    }
    if (sin >= 0) {
        int top_border = int(point_1.second / tile_size);
        int bottom_border = int(point_2.second / tile_size) + 1;

        for (int i = top_border + 1; i < bottom_border; i++) {
            float i_x_chor = cos * (i * tile_size - point_1.second) / sin + point_1.first;
            int i_left = int(i_x_chor / tile_size);
            if (map[i][i_left]) {
                return true;
            }
        }
    }
    else if (sin < 0) {
        int bottom_border = int(point_1.second / tile_size);
        int top_border = int(point_2.second / tile_size) + 1;

        for (int i = bottom_border + 1; i > top_border; i--) {
            float i_x_chor = cos * (i * tile_size - point_1.second) / sin + point_1.first;
            int i_left = int(i_x_chor / tile_size);
            if (map[i][i_left]) {
                return true;
            }
        }
    }
    return false;
    /*
    В этом кусочке скрыта маленькая неточность:
    Мы имеем координаты левого-верхнего угла перемещаемого объекта, и смотрим, чтобы именно левый-верхний угол не пересекался
    со стенами. Правый-нижний угол без проблем сможет пройти сквозь стены

    К сожалению, я временно оставлю её неисправленной, так как ПО без багов - не ПО :)
    */
}




class Adopted_Monster {
private:
    int speed;
    std::pair<int, int> position;
    int size;

public:
    int get_speed() {
        return this->speed;
    }
    void set_speed(int new_speed) {
        this->speed = new_speed;
    }

    std::pair<int, int> get_position() {
        return this->position;
    }
    void set_position(std::pair<int, int> new_pos) {
        this->position = new_pos;
    }

    int get_size() {
        return this->size;
    }
    void set_size(int new_size) {
        this->size = new_size;
    }

    
};

void draw_way(sf::RenderWindow& window, std::vector<std::pair<float, float>> way) {
    for (int cell_num = 0; cell_num < way.size() - 1; cell_num++) {
        sf::Vertex step_line[] = {
            sf::Vertex(sf::Vector2f(way[cell_num].first, way[cell_num].second), sf::Color::Blue),
            sf::Vertex(sf::Vector2f(way[cell_num+1].first, way[cell_num+1].second), sf::Color::Blue)
        };
        // step_line->color = sf::Color::Blue;
        window.draw(step_line, 2, sf::Lines);
    }
}


void draw_map(
    sf::RenderWindow& window,
    std::vector<std::vector<int>> map,
    float tile_size,
    std::pair<float, float> pl_pos
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
            rect.setPosition(tile_size * chor_x - pl_pos.first + width/2, tile_size * chor_y - pl_pos.second + height/2);
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

    std::pair<float, float> pos = { win_width / 2, win_height / 2 };
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

    std::vector<std::pair<float, float>> way = { {1, 1}, {2, 2}, {2, 3} };
    std::pair<float, float> aim = { 0, 0 };

    sf::CircleShape shape(pl.get_size());
    shape.setPosition(pos.first, pos.second);
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
                std::pair<float, float> player_pos = pl.get_position();
                float aim_x = mouse_pos.x + player_pos.first - win_width / 2;
                float aim_y = mouse_pos.y + player_pos.second - win_height / 2;
                aim = { aim_x, aim_y };

                if (check_line_collision(player_pos, aim, map, tile_size)) {
                    aim = { 0, 0 };
                }
                
            }

        }

        pl.move(window, move_dir, map, tile_size);

        std::pair<float, float> pos = pl.get_position();

        draw_map(window, map, tile_size, pl.get_position());
        
        if (pos == aim) {
            aim.first = 0;
            aim.second = 0;
        }
        if (aim.first != 0 || aim.second != 0) {
            pl.move_to(aim, map);
        }

        window.draw(shape);
        window.display();
    }
    return 0;
}
