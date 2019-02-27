#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

// Define some constants
const int windowWidth = 800;
const int windowHeight = 600;

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Sample graphics", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Create a rectangle
    sf::RectangleShape myRect;
    myRect.setSize(sf::Vector2f(50,50));
    myRect.setOutlineThickness(3);
    myRect.setOutlineColor(sf::Color::Black);
    myRect.setFillColor(sf::Color(100, 100, 200));
    myRect.setOrigin(0, 0);

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    sf::Text text("Hello world", font, 50);

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Space key pressed: change position of rectangle
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                myRect.setPosition(std::rand() % windowWidth, std::rand() % windowHeight);
                text.setPosition(std::rand() % windowWidth, std::rand() % windowHeight);
            }
        }

        // Clear the window
        window.clear(sf::Color(50, 200, 50));

        // Draw the rectangle
        window.draw(myRect);

        // Draw the text
        window.draw(text);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
