
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    Window window(VideoMode(800, 600), "MGLearnOpenGL", Style::Default, ContextSettings(0, 0, 0, 3, 3, ContextSettings::Core));
    window.setVerticalSyncEnabled(true);

    bool running = true;

    do
    {
        Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    running = false;
                    break;
                }
            }
        }

        window.display();

    } while (running);

    return 0;
}