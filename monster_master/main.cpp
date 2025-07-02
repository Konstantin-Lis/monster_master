#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

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
        int pos_x = this->position[0];
        int pos_y = this->position[1];
        int speed = this->speed;

        if (move_dir[0] == 1) {
            pos_y -= speed;
        }
        if (move_dir[1] == 1) {
            pos_x -= speed;
        }
        if (move_dir[2] == 1) {
            pos_y += speed;
        }
        if (move_dir[3] == 1) {
            pos_x += speed;
        }

        int map_size_x = map[0].size() * tile_size - 2 * (this->size);
        int map_size_y = map.size() * tile_size - 2 * (this->size);
        if (pos_x < 0) {
            pos_x = 0;
        }
        if (pos_y < 0) {
            pos_y = 0;
        }
        if (pos_x > map_size_x) {
            pos_x = map_size_x;
        }
        if (pos_y > map_size_y) {
            pos_y = map_size_y;
        }

        std::vector<int> new_pos = { pos_x, pos_y };
        this->set_position(new_pos);
    }

    void draw(std::vector<int> pos) {

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
                rect.setFillColor(sf::Color::White);
            }
            else if (map[chor_y][chor_x] == 0) {
                rect.setFillColor(sf::Color::Green);
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

    int speed = 10;

    std::vector<int> pos = { int(window.getSize().x) / 2, int(window.getSize().y) / 2 };
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

        }

        pl.move(window, move_dir, map, tile_size);

        std::vector<int> pos = pl.get_position();
        
        //shape.setPosition(pos[0], pos[1]);

        //window.clear(sf::Color::Blue);
        draw_map(window, map, tile_size, pl.get_position());
        window.draw(shape);
        window.display();
    }
    return 0;
}

/*



*/