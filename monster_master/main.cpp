#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

std::vector<int> move(std::vector<int> move_dir, std::vector<int> pos, int speed) {
    int pos_x = pos[0];
    int pos_y = pos[1];

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
    std::vector<int> new_pos = { pos_x, pos_y };

    return new_pos;
}



int main()
{
    int speed = 3;

    std::vector<int> pos = { 100, 100 };
    std::vector<int> move_dir = { 0, 0, 0, 0 };
    std::vector<int> move_vec = { 0, 0 };

    sf::RenderWindow window(sf::VideoMode(), L"Новый проект", sf::Style::Fullscreen);

    window.setVerticalSyncEnabled(true);

    sf::CircleShape shape(20.f);
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

        pos = move(move_dir, pos, speed);
        shape.setPosition(pos[0], pos[1]);

        window.clear(sf::Color::Blue);
        window.draw(shape);
        window.display();
    }
    return 0;
}

/*
На среду:
1. Класс игрока с функцией движения
2. Карта/стена и обработка столкновений со стенами


*/