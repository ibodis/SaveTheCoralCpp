#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <Windows.h>

enum Molecule
{
    CarbonDioxide,
    CarbonicAcid,
    Carbonate,
    BiCarbonate,
    CalciumCarbonate
};

// Define some constants
const int windowWidth = 2000;
const int windowHeight = 1200;

// Screen areas
float menuHeight = 300;
float coralHeight = 300;
sf::Rect<float> menuRect(0, 0, windowWidth, menuHeight);
sf::Rect<float> waterRect(0, menuRect.height, windowWidth, windowHeight - menuRect.height);
sf::Rect<float> coralRect(0, windowHeight - coralHeight, windowWidth, coralHeight);

// Background shapes
sf::RectangleShape menu;
sf::RectangleShape water;
sf::RectangleShape coral;

// Universal shape properties
float shapeOutlineThickness = 1;
sf::Color shapeOutlineColor(100, 100, 200);
int movementAmount = 4;
sf::Color textColor(sf::Color::Black);

// Water properties
sf::Color waterColor(32, 178, 170);

// Carbon dioxide properties
const int MIN_CarbonDioxideCount = 10;
const int MAX_CarbonDioxideCount = 300;
sf::CircleShape CarbonDioxideMolecules[MAX_CarbonDioxideCount];
int currentCarbonDioxideCount = MIN_CarbonDioxideCount;
sf::Color carbonDioxideColor(107, 142, 35);
float carbonDioxideSize = 5;

// Carbonic acid properties
sf::Color carbonicAcidColor(255, 165, 0);
float carbonicAcidSize = 7;

// Carbonate properties
sf::Color carbonateColor(255, 160, 122);
float carbonateSize = 2;

// Bi-carbonate properties
sf::Color biCorbonateColor(25, 25, 112);
float biCarbonateSize = 4;

// Calcium carbonate properties
sf::Color calciumCarbonateColor(255, 248, 220);
float calciumCarbonateSize = 6;

void InitializeMolecules(Molecule type)
{
    switch (type)
    {
    case CarbonDioxide:
        for (int i = 0; i < MAX_CarbonDioxideCount; i++)
        {
            float x = std::rand() % (int) waterRect.width;
            float y = std::rand() % (int) waterRect.height;

            CarbonDioxideMolecules[i].setPosition(x, waterRect.top + y);
            CarbonDioxideMolecules[i].setFillColor(carbonDioxideColor);
            CarbonDioxideMolecules[i].setRadius(carbonDioxideSize);
            CarbonDioxideMolecules[i].setOutlineThickness(shapeOutlineThickness);
            CarbonDioxideMolecules[i].setOutlineColor(shapeOutlineColor);
        }
        break;
    }
}

void AnimateMolecules(Molecule type)
{
    switch (type)
    {
    case CarbonDioxide:
        for (int i = 0; i < currentCarbonDioxideCount; i++)
        {
            sf::Vector2f currentPosition = CarbonDioxideMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            CarbonDioxideMolecules[i].setPosition(currentPosition);
        }
        break;
    }
}

void IncreaseMoleculeCount(Molecule type, int amount)
{
    switch (type)
    {
    case CarbonDioxide:
        currentCarbonDioxideCount = std::min(currentCarbonDioxideCount + amount, MAX_CarbonDioxideCount);
        break;
    }
}

void DecreaseMoleculeCount(Molecule type, int amount)
{
    switch (type)
    {
    case CarbonDioxide:
        currentCarbonDioxideCount = std::max(currentCarbonDioxideCount - amount, MIN_CarbonDioxideCount);
        break;
    }
}

int main()
{
    std::srand(GetTickCount());

    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Create the window of the application
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight, 32), "Sample graphics", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Create water rectangle shape
    water.setPosition(waterRect.left, waterRect.top);
    water.setSize(sf::Vector2f(waterRect.width, waterRect.height));
    water.setOutlineColor(waterColor);
    water.setFillColor(waterColor);

    sf::Text text("Save the Coral!!", font, 50);
    text.setFillColor(textColor);
     
    InitializeMolecules(CarbonDioxide);

    while (window.isOpen())
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
           
            if ((event.type == sf::Event::KeyPressed))
            {
                switch (event.key.code)
                {
                case sf::Keyboard::A:
                    IncreaseMoleculeCount(CarbonDioxide, 10);
                    break;
                case sf::Keyboard::B:
                    DecreaseMoleculeCount(CarbonDioxide, 10);
                    break;
                case sf::Keyboard::Space:
                    text.setPosition(std::rand() % windowWidth, std::rand() % windowHeight);
                    break;
                }
            }
        }

        // Animate molecules
        AnimateMolecules(CarbonDioxide);

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the water
        window.draw(water);

        // Draw the molecules
        for (int i = 0; i < currentCarbonDioxideCount; i++)
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
