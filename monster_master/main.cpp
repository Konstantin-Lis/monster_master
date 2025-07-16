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
}




class Adopted_Monster {
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
    void set_position(std::pair<int, int> new_pos) {
        this->position = new_pos;
    }

    int get_size() {
        return this->size;
    }
    void set_size(int new_size) {
        this->size = new_size;
    }

    void move_to(std::pair<float, float> aim, std::vector<std::vector<int>> map) {
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


void draw_map(
    sf::RenderWindow& window,
    std::vector<std::vector<int>> map,
    float tile_size,
    std::pair<float, float> pl_pos)
{
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
    std::pair<float, float> aim_lt = { 0, 0 };

    Player pl;
    pl.set_position(pos);
    pl.set_speed(speed);
    pl.set_size(20);

    Adopted_Monster m1;
    std::pair<float, float> m1_pos = { win_width / 2 + 100, win_height / 2 + 100 };
    int m1_speed = 7;
    int m1_size = 30;
    m1.set_position(m1_pos);
    m1.set_speed(m1_speed);
    m1.set_size(m1_size);


    sf::CircleShape pl_shape(pl.get_size());
    pl_shape.setPosition(pos.first, pos.second);
    pl_shape.setFillColor(sf::Color::Magenta);
    
    sf::CircleShape m1_shape(m1_size);
    m1_shape.setPosition(m1_pos.first, m1_pos.second);
    m1_shape.setFillColor(sf::Color::Green);


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
                /*
                Функция check_line_collision определяет непосредственно пересечения линии,
                поэтому сверяем все 4 угла начального и конечного положения
                Иначе может быть "хождение сквозь текстуры"
                */

                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                int pl_size = pl.get_size();
                std::pair<float, float> pl_lt_pos = pl.get_position();

                std::pair<float, float> m1_lt_pos = m1.get_position();
                std::pair<float, float> m1_rt_pos = { m1_lt_pos.first + 2 * m1_size, m1_lt_pos.second };
                std::pair<float, float> m1_lb_pos = { m1_lt_pos.first, m1_lt_pos.second + 2 * m1_size };
                std::pair<float, float> m1_rb_pos = { m1_lt_pos.first + 2 * m1_size, m1_lt_pos.second + 2 * m1_size };

                float aim_center_x = mouse_pos.x + pl_lt_pos.first - win_width / 2;
                float aim_center_y = mouse_pos.y + pl_lt_pos.second - win_height / 2;
                aim_lt = { aim_center_x - m1_size, aim_center_y - m1_size };

                std::pair<float, float> aim_rt = { aim_center_x + pl_size, aim_center_y - pl_size };
                std::pair<float, float> aim_lb = { aim_center_x - pl_size, aim_center_y + pl_size };
                std::pair<float, float> aim_rb = { aim_center_x + pl_size, aim_center_y + pl_size };

                if (check_line_collision(m1_lt_pos, aim_lt, map, tile_size) ||
                    check_line_collision(m1_rt_pos, aim_rt, map, tile_size) ||
                    check_line_collision(m1_lb_pos, aim_lb, map, tile_size) ||
                    check_line_collision(m1_rb_pos, aim_rb, map, tile_size)) {
                    aim_lt = { 0, 0 };
                }
                
            }

        }

        pl.move(window, move_dir, map, tile_size);

        std::pair<float, float> pos = pl.get_position();
        std::pair<float, float> m1_pos = m1.get_position();

        draw_map(window, map, tile_size, pl.get_position());
        
        if (m1_pos == aim_lt) {
            aim_lt.first = 0;
            aim_lt.second = 0;
        }
        if (aim_lt.first != 0 || aim_lt.second != 0) {
            m1.move_to(aim_lt, map);
        }

        float m1_window_x = m1_pos.first - pos.first + win_width / static_cast<float>(2);
        float m1_window_y = m1_pos.second - pos.second + win_height / static_cast<float>(2);
        m1_shape.setPosition(m1_window_x, m1_window_y);

        window.draw(pl_shape);
        window.draw(m1_shape);
        window.display();
    }
    return 0;
}
