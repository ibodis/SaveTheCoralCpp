#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>

using namespace sf;

// Define some constants
const int windowWidth = 2000;
const int windowHeight = 1200;

// Global objects
sf::Rect<float> waterRect;

sf::Color waterColor(32, 178, 170);
sf::Color carbonDioxideColor(107, 142, 35);
sf::Color carbonicAcidColor(255, 165, 0);
sf::Color carbonateColor(255, 160, 122);
sf::Color biCorbonateColor(25, 25, 112);
sf::Color calciumCarbonateColor(255, 248, 220);

float shapeOutlineThickness = 3;
sf::Color shapeOutlineColor(100, 100, 200);

#define MIN_CarbonDioxideLevel 10
#define MAX_CarbonDioxideLevel 70

sf::RectangleShape CarbonDioxideMolecules[MAX_CarbonDioxideLevel+1];
int currentCarbonDioxideLevel = MIN_CarbonDioxideLevel;

void MakeCarbonDioxide()
{
    for (int i = 0; i < currentCarbonDioxideLevel; i++)
    {
        CarbonDioxideMolecules[i].setPosition(std::rand() % windowWidth, std::rand() % windowHeight);
        CarbonDioxideMolecules[i].setSize(sf::Vector2f(10, 10));
        CarbonDioxideMolecules[i].setOutlineThickness(1);
        CarbonDioxideMolecules[i].setOutlineColor(sf::Color::White);
        CarbonDioxideMolecules[i].setFillColor(carbonDioxideColor);
    }
}



enum Molecule 
{
    CarbonDioxide,
    CarbonicAcid,
    Carbonate,
    BiCarbonate,
    CalciumCarbonate
};

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

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    sf::Text text("Save the Coral!!", font, 50);
     
    MakeCarbonDioxide();

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
           
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A))
            {
                currentCarbonDioxideLevel+=10;

                if (currentCarbonDioxideLevel > MAX_CarbonDioxideLevel)
                {
                    currentCarbonDioxideLevel = MAX_CarbonDioxideLevel;
                }
                MakeCarbonDioxide();
            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::B))
            {
                currentCarbonDioxideLevel-=10;
                if (currentCarbonDioxideLevel < MIN_CarbonDioxideLevel)
                {
                    currentCarbonDioxideLevel = MIN_CarbonDioxideLevel;
                }
                MakeCarbonDioxide();
            }
            // Space key pressed: change position of rectangle
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space))
            {
                MakeCarbonDioxide();
                text.setPosition(std::rand() % windowWidth, std::rand() % windowHeight);
            }
        }

        // Clear the window
        window.clear(waterColor);

        for (int i = 0; i < currentCarbonDioxideLevel; i++)
        {
            window.draw(CarbonDioxideMolecules[i]);

         }

        // Draw the text
        window.draw(text);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
