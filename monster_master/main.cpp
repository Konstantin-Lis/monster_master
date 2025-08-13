#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <tuple>

/*
ВНИМАНИЕ!!!
Координатой объекта является его левый верхний угол. И относительно него и идут все рассчёты
*/


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

        int player_left_chor = int(pos_x / tile_size);
        int player_right_chor = int((pos_x + 2 * this->get_size()) / tile_size);
        int player_top_chor = int(pos_y / tile_size);
        int player_bottom_chor = int((pos_y + 2 * this->get_size()) / tile_size);

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
                pos_y = player_bottom_chor * tile_size - 2 * this->get_size() - 1;
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


// Проверяет пересечение линии между точками со стенами на карте
bool check_line_collision(std::pair<float, float> point_1, std::pair<float, float> point_2, std::vector<std::vector<int>> map, int tile_size) {
    /*
    Возвращает true, если пересечение есть, и false, если нет
    */
    int map_x = int(point_1.first / tile_size);
    int map_y = int(point_1.second / tile_size);
    if (map[map_y][map_x]) {
        return true;
    }

    map_x = int(point_2.first / tile_size);
    map_y = int(point_2.second / tile_size);
    if (map[map_y][map_x]) {
        return true;
    }

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

        for (int i = left_border + 1; i < right_border; i++) {
            float i_y_chor = sin * (i * tile_size - point_1.first) / cos + point_1.second;
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


// Проверяет пересечение пути объекта до точки со стенами на карте
bool check_collisions_on_way(
    std::pair<float, float> point_1,
    std::pair<float, float> point_2,
    std::vector<std::vector<int>> map,
    int tile_size,
    int object_size)
{
    /*
    Возвращает true, если пересечение есть, и false, если нет
    */
    std::pair<float, float> point_1_lt = point_1;
    std::pair<float, float> point_1_rt = { point_1_lt.first + 2 * object_size, point_1_lt.second };
    std::pair<float, float> point_1_lb = { point_1_lt.first, point_1_lt.second + 2 * object_size };
    std::pair<float, float> point_1_rb = { point_1_lt.first + 2 * object_size, point_1_lt.second + 2 * object_size };

    std::pair<float, float> point_2_lt = { point_2.first - object_size, point_2.second - object_size };
    std::pair<float, float> point_2_rt = { point_2.first + object_size, point_2.second - object_size };
    std::pair<float, float> point_2_lb = { point_2.first - object_size, point_2.second + object_size };
    std::pair<float, float> point_2_rb = { point_2.first + object_size, point_2.second + object_size };

    if (check_line_collision(point_1_lt, point_2_lt, map, tile_size) ||
        check_line_collision(point_1_lt, point_2_rt, map, tile_size) ||
        check_line_collision(point_1_lt, point_2_lb, map, tile_size) ||
        check_line_collision(point_1_lt, point_2_rb, map, tile_size)) {
        return true;
    }
    return false;
}


// Проверяет факт пересечения двух объектов (квадратов, в которые они вписаны)
bool check_object_object_collision(
    std::pair<float, float> object_1_lt,
    int object_1_size,
    std::pair<float, float> object_2_lt,
    int object_2_size)
{
    /*
    Возвращает true, если пересечение есть, и false, если нет
    */
    float object_1_left = object_1_lt.first;
    float object_1_right = object_1_lt.first + 2* object_1_size;
    float object_1_top = object_1_lt.second;
    float object_1_bottom = object_1_lt.second + 2 * object_1_size;

    float object_2_left = object_2_lt.first;
    float object_2_right = object_2_lt.first + 2* object_2_size;
    float object_2_top = object_2_lt.second;
    float object_2_bottom = object_2_lt.second + 2 * object_2_size;

    if (object_1_left > object_2_right ||
        object_1_top > object_2_bottom ||
        object_1_right < object_2_left ||
        object_1_bottom < object_2_top) {
        return false;
    }

    return true;
}


class Adopted_Monster {
private:
    int speed;
    std::pair<float, float> position;
    int size;
    std::pair<float, float> aim_lt;

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

    std::pair<float, float> get_aim() {
        return this->aim_lt;
    }
    void set_aim(std::pair<int, int> new_aim) {
        this->aim_lt = new_aim;
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

    void move_to_aim(std::vector<std::vector<int>> map) {
        std::pair<float, float> pos = this->get_position();
        std::pair<float, float> aim = this->get_aim();
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


    std::pair<float, float> count_pos_on_window(sf::RenderWindow& window, std::pair<float, float> pl_pos) {
        std::pair<float, float> pos = this->get_position();
        int win_width = int(window.getSize().x);
        int win_height = int(window.getSize().y);

        float win_pos_x = pos.first - pl_pos.first + win_width / static_cast<float>(2);
        float win_pos_y = pos.second - pl_pos.second + win_height / static_cast<float>(2);

        std::pair<float, float> window_pos = { win_pos_x, win_pos_y };
        return window_pos;
    }

};


class Enemy: public Adopted_Monster {
private:
    int look_distation;
    std::pair<float, float> base_position;
public:
    int get_look_distation() {
        return this->look_distation;
    }
    void set_look_distation(int new_speed) {
        this->look_distation = new_speed;
    }

    std::pair<float, float> get_base_position() {
        return this->base_position;
    }
    void set_base_position(std::pair<float, float> new_base_pos) {
        this->base_position = new_base_pos;
    }


    // Проверяет, находится ли объъект в зоне видимости
    bool check_object_near(std::pair<float, float> object_pos, int object_size, std::vector<std::vector<int>> map, int tile_size) {
        std::pair<float, float> pos_lt = this->get_position();
        int e_size = this->get_size();
        int dist = this->get_look_distation();

        std::pair<float, float> object_lt = object_pos;
        std::pair<float, float> object_rt = { object_lt.first + 2 * object_size, object_lt.second };
        std::pair<float, float> object_lb = { object_lt.first, object_lt.second + 2 * object_size };
        std::pair<float, float> object_rb = { object_lt.first + 2 * object_size, object_lt.second + 2 * object_size };

        std::pair<float, float> pos_rt = { pos_lt.first + 2 * e_size, pos_lt.second };
        std::pair<float, float> pos_lb = { pos_lt.first, pos_lt.second + 2 * e_size };
        std::pair<float, float> pos_rb = { pos_lt.first + 2 * e_size, pos_lt.second + 2 * e_size };

        // Сначала проверим, есть ли объект в квадрате со стороной 2*радиус_видимости (простые вычисления)
        if (object_rt.first > pos_lt.first - dist &&
            object_lt.first < pos_rt.first + dist &&
            object_lb.second > pos_lt.second - dist &&
            object_lt.second < pos_lb.second + dist)
        {
            // Объект внутри квадрата
            // Далее проверяем, есть ли он в круге с радиусом радиус_видимости (более сложные вычисления)
            std::pair<float, float> object_center = { object_lt.first + object_size, object_lt.second + object_size };
            std::pair<float, float> pos_center = { pos_lt.first + e_size, pos_lt.second + e_size };
            if ((std::pow(object_center.first - pos_center.first, 2) + std::pow(object_center.second - pos_center.second, 2)) < std::pow(dist + e_size, 2))
            {
                // Объект внутри круга
                // Теперь проверка на коллизии со стенами
                if (check_line_collision(object_lt, pos_lt, map, tile_size) ||
                    check_line_collision(object_rt, pos_rt, map, tile_size) ||
                    check_line_collision(object_lb, pos_lb, map, tile_size) ||
                    check_line_collision(object_rb, pos_rb, map, tile_size)
                    )
                {
                    // Пересечения со стенами есть, значит, объект вне зоны видимости
                    return false;
                }
                else {
                    // Пересечений нет, объект видно
                    return true;
                }
            }
        }
        return false;
    }
};



void draw_object(sf::RenderWindow& window, std::pair<float, float> object_pos, int object_size, sf::Color color) {
    sf::CircleShape obj_shape(object_size);
    obj_shape.setPosition(object_pos.first, object_pos.second);
    obj_shape.setFillColor(color);

    window.draw(obj_shape);
}


void draw_map(
    sf::RenderWindow& window,
    std::vector<std::vector<int>> map,
    int tile_size,
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
            rect.setPosition(tile_size * chor_x - pl_pos.first + width / 2, tile_size * chor_y - pl_pos.second + height / 2);
            window.draw(rect);
        }
    }
}


void reverse_move_dir(std::vector<int>& move_dir) {
    for (int i = 0; i < 4; i++) {
        if (move_dir[i] == 0) {
            move_dir[i] = 1;
        }
        else if(move_dir[i] == 1) {
            move_dir[i] = 0;
        }
    }
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(), L"Новый проект", sf::Style::Fullscreen);
    window.setVerticalSyncEnabled(true);
    int win_width = int(window.getSize().x);
    int win_height = int(window.getSize().y);

    int speed = 5;

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
    int m1_speed = 3;
    int m1_size = 30;
    m1.set_position(m1_pos);
    m1.set_speed(m1_speed);
    m1.set_size(m1_size);
    m1.set_aim(m1_pos);

    Adopted_Monster m2;
    std::pair<float, float> m2_pos = { win_width / 2 + 200, win_height / 2 + 200 };
    int m2_speed = 3;
    int m2_size = 30;
    m2.set_position(m2_pos);
    m2.set_speed(m2_speed);
    m2.set_size(m2_size);
    m2.set_aim(m2_pos);

    Adopted_Monster m3;
    std::pair<float, float> m3_pos = { win_width / 2 + 300, win_height / 2 + 300 };
    int m3_speed = 3;
    int m3_size = 30;
    m3.set_position(m3_pos);
    m3.set_speed(m3_speed);
    m3.set_size(m3_size);
    m3.set_aim(m3_pos);

    std::vector<Adopted_Monster> adopted_monsters = { m1 };
    int active_monster_num = 0;

    Enemy e1;
    std::pair<float, float> e1_pos = { win_width / 2 + 300, win_height / 2 - 300 };
    int e1_speed = 1;
    int e1_size = 30;
    e1.set_position(e1_pos);
    e1.set_speed(e1_speed);
    e1.set_size(e1_size);
    e1.set_aim(e1_pos);
    e1.set_look_distation(300);
    e1.set_base_position(e1_pos);

    std::vector<Enemy> enemies = { e1 };


    sf::Color active_color = { 255, 127, 0 };
    sf::Color passive_color = { 0, 255, 0 };
    sf::Color enemy_color = { 0, 0, 255 };

    sf::CircleShape pl_shape(pl.get_size());
    pl_shape.setPosition(pos.first, pos.second);
    pl_shape.setFillColor(sf::Color::Magenta);


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

            else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                int pl_size = pl.get_size();
                std::pair<float, float> pl_lt_pos = pl.get_position();

                float aim_center_x = mouse_pos.x + pl_lt_pos.first - win_width / 2;
                float aim_center_y = mouse_pos.y + pl_lt_pos.second - win_height / 2;

                for (int monster_num = 0; monster_num < adopted_monsters.size(); monster_num++) {
                    std::pair<float, float> monster_pos = adopted_monsters[monster_num].get_position();
                    int m_size = adopted_monsters[monster_num].get_size();
                    if (aim_center_x >= monster_pos.first && aim_center_x <= monster_pos.first + 2 * m_size &&
                        aim_center_y >= monster_pos.second && aim_center_y <= monster_pos.second + 2 * m_size)
                    {
                        active_monster_num = monster_num;
                        break;
                    }
                }

                int monster_size = adopted_monsters[active_monster_num].get_size();
                std::pair<float, float> monster_lt_pos = adopted_monsters[active_monster_num].get_position();
                std::pair<float, float> aim_pos = { aim_center_x, aim_center_y };

                if (check_collisions_on_way(monster_lt_pos, aim_pos, map, tile_size, monster_size)) {
                    aim_lt = { 0, 0 };
                }
                else {
                    adopted_monsters[active_monster_num].set_aim({ aim_center_x - monster_size, aim_center_y - monster_size });
                }


            }

        }

        // Двигаем игрока + обработка столкновений (пересечений) с приручёнными монстрами
        pl.move(window, move_dir, map, tile_size);
        for (int i = 0; i < adopted_monsters.size(); i++) {
            if (check_object_object_collision(pl.get_position(), pl.get_size(), adopted_monsters[i].get_position(), adopted_monsters[i].get_size())) {
                reverse_move_dir(move_dir);
                pl.move(window, move_dir, map, tile_size);
                move_dir = { 0, 0, 0, 0 };
                break;
            }
        }

        std::pair<float, float> pos = pl.get_position();
        std::pair<float, float> active_monster_pos = adopted_monsters[active_monster_num].get_position();

        // Двигаем приручённых монстров + обработка столкновений (пересечений) с игроком или другими приручёнными монстрами
        for (int i = 0; i < adopted_monsters.size(); i++) {
            std::pair<float, float> pl_lt_pos = pl.get_position();
            int pl_size = pl.get_size();
            std::pair<float, float> monster_lt_pos = adopted_monsters[i].get_position();
            int monster_size = adopted_monsters[i].get_size();

            adopted_monsters[i].move_to_aim(map);
            std::pair<float, float> monster_new_pos = adopted_monsters[i].get_position();

            if (check_object_object_collision(monster_new_pos, monster_size, pl_lt_pos, pl_size)) {
                adopted_monsters[i].set_aim(monster_lt_pos);
                adopted_monsters[i].move_to(monster_lt_pos, map);
                continue;
            }
            
            for (int j = 0; j < adopted_monsters.size(); j++) {
                if (i != j) {
                    std::pair<float, float> another_monster_lt_pos = adopted_monsters[j].get_position();
                    int another_monster_size = adopted_monsters[j].get_size();

                    if (check_object_object_collision(monster_new_pos, monster_size, another_monster_lt_pos, another_monster_size)) {
                        adopted_monsters[i].set_aim(monster_lt_pos);
                        adopted_monsters[i].move_to(monster_lt_pos, map);
                        break;
                    }
                }
            }
        }

        // Проверяем, есть ли рядом игрок
        if (e1.check_object_near(pos, pl.get_size(), map, tile_size)) {
            e1.set_aim(pos);
            enemy_color = { 255, 0, 0 };
        }
        if (e1.get_aim() == e1.get_position()) {
            e1.set_aim(e1.get_base_position());
            enemy_color = { 0, 0, 255 };
        }
        e1.move_to_aim(map);

        
        draw_map(window, map, tile_size, pl.get_position());  // Отрисовываем карту
        for (int i = 0; i < adopted_monsters.size(); i++) {  // Отрисовываем приручённых монстров
            std::pair<float, float> monster_win_pos = adopted_monsters[i].count_pos_on_window(window, pos);
            int m_size = adopted_monsters[i].get_size();

            if (i == active_monster_num) {
                draw_object(window, monster_win_pos, m_size, active_color);
            }
            else {
                draw_object(window, monster_win_pos, m_size, passive_color);
            }
        }

        // Отрисовываем врагов
        std::pair<float, float> enemy_win_pos = e1.count_pos_on_window(window, pos);
        draw_object(window, enemy_win_pos, e1.get_size(), enemy_color);

        window.draw(pl_shape);
        window.display();
    }
    return 0;
}
