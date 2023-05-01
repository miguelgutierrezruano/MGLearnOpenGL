
// Distributed under MIT License
// @miguelgutierrezruano
// 2023

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <glad/glad.h>

#include <cassert>
#include <iostream>

using namespace sf;

int main()
{
    // Window with OpenGL context
    Window window(VideoMode(800, 600), "MGLearnOpenGL", Style::Default, ContextSettings(0, 0, 0, 4, 4, ContextSettings::Core));

    // Glad initialization
    GLenum glad_init = gladLoadGL();

    // Stop program if glad could not load properly
    assert(glad_init != 0);

    std::cout << glGetString(GL_VERSION) << std::endl;

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