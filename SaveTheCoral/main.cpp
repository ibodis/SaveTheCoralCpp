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
float carbonDioxideSize = 4;

// Carbonic acid properties
const int MIN_CarbonicAcidCount = 10;
const int MAX_CarbonicAcidCount = 300;
sf::Color carbonicAcidColor(255, 165, 0);
sf::CircleShape CarbonicAcidMolecules[MAX_CarbonicAcidCount];
int currentCarbonicAcidCount = MIN_CarbonicAcidCount;
float carbonicAcidSize = 6.5;

// Carbonate properties
sf::Color carbonateColor(255, 160, 122);
float carbonateSize = 6;
const int MIN_CarbonateCount = 10;
const int MAX_CarbonateCount = 250;
sf::CircleShape CarbonateMolecules[MAX_CarbonateCount];
int currentCarbonateCount = MIN_CarbonateCount;

// Bi-carbonate properties
sf::Color biCarbonateColor(25, 25, 112);
float biCarbonateSize = 6;
const int MIN_biCarbonateCount = 10;
const int MAX_biCarbonateCount = 250;
sf::CircleShape biCarbonateMolecules[MAX_biCarbonateCount];
int currentBiCarbonateCount = MIN_biCarbonateCount;

// Calcium carbonate properties
sf::Color calciumCarbonateColor(255, 248, 220);
float calciumCarbonateSize = 10;
const int MIN_CalciumCarbonateCount = 10;
const int MAX_CalciumCarbonateCount = 250;
sf::CircleShape CalciumCarbonateMolecules[MAX_CalciumCarbonateCount];
int currentCalciumCarbonateCount = MIN_CalciumCarbonateCount;

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
    case CarbonicAcid:
        for (int i = 0; i < MAX_CarbonicAcidCount; i++)
        {
            float x = std::rand() % (int)waterRect.width;
            float y = std::rand() % (int)waterRect.height;

            CarbonicAcidMolecules[i].setPosition(x, waterRect.top + y);
            CarbonicAcidMolecules[i].setFillColor(carbonicAcidColor);
            CarbonicAcidMolecules[i].setRadius(carbonicAcidSize);
            CarbonicAcidMolecules[i].setOutlineThickness(shapeOutlineThickness);
            CarbonicAcidMolecules[i].setOutlineColor(shapeOutlineColor);   
        }
        break;
    case Carbonate:
        for (int i = 0; i < MAX_CarbonateCount; i++)
        {
            float x = std::rand() % (int)waterRect.width;
            float y = std::rand() % (int)waterRect.height;

            CarbonateMolecules[i].setPosition(x, waterRect.top + y);
            CarbonateMolecules[i].setFillColor(carbonateColor);
            CarbonateMolecules[i].setRadius(carbonateSize);
            CarbonateMolecules[i].setOutlineThickness(shapeOutlineThickness);
            CarbonateMolecules[i].setOutlineColor(shapeOutlineColor);
        }
        break;
    case BiCarbonate:
        for (int i = 0; i < MAX_biCarbonateCount; i++)
        {
            float x = std::rand() % (int)waterRect.width;
            float y = std::rand() % (int)waterRect.height;

            biCarbonateMolecules[i].setPosition(x, waterRect.top + y);
            biCarbonateMolecules[i].setFillColor(biCarbonateColor);
            biCarbonateMolecules[i].setRadius(biCarbonateSize);
            biCarbonateMolecules[i].setOutlineThickness(shapeOutlineThickness);
            biCarbonateMolecules[i].setOutlineColor(shapeOutlineColor);
        }   
        break;
    case CalciumCarbonate:
        for (int i = 0; i < MAX_CalciumCarbonateCount; i++)
        {
            float x = std::rand() % (int)waterRect.width;
            float y = std::rand() % (int)waterRect.height;

            CalciumCarbonateMolecules[i].setPosition(x, waterRect.top + y);
            CalciumCarbonateMolecules[i].setFillColor(calciumCarbonateColor);
            CalciumCarbonateMolecules[i].setRadius(calciumCarbonateSize);
            CalciumCarbonateMolecules[i].setOutlineThickness(shapeOutlineThickness);
            CalciumCarbonateMolecules[i].setOutlineColor(shapeOutlineColor);
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
    case CarbonicAcid:
        for (int i = 0; i < currentCarbonicAcidCount; i++)
        {
            sf::Vector2f currentPosition = CarbonicAcidMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            CarbonicAcidMolecules[i].setPosition(currentPosition);
        }
        break;
    case Carbonate:
        for (int i = 0; i < currentCarbonateCount; i++)
        {
            sf::Vector2f currentPosition = CarbonateMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            CarbonateMolecules[i].setPosition(currentPosition);
        }
        break;
    case BiCarbonate:
        for (int i = 0; i < currentBiCarbonateCount; i++)
        {
            sf::Vector2f currentPosition = biCarbonateMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            biCarbonateMolecules[i].setPosition(currentPosition);
        }
        break;
    case CalciumCarbonate:
        for (int i = 0; i < currentCalciumCarbonateCount; i++)
        {
            sf::Vector2f currentPosition = CalciumCarbonateMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % movementAmount);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

           CalciumCarbonateMolecules[i].setPosition(currentPosition);
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
    case CarbonicAcid:
        currentCarbonicAcidCount = std::min(currentCarbonicAcidCount + amount, MAX_CarbonicAcidCount);
        break;
    case Carbonate:
        currentCarbonateCount = std::min(currentCarbonateCount + amount, MAX_CarbonateCount);
        break;
    case BiCarbonate:
        currentBiCarbonateCount = std::min(currentBiCarbonateCount + amount, MAX_biCarbonateCount);
        break;
    case CalciumCarbonate:
        currentCalciumCarbonateCount = std::min(currentCalciumCarbonateCount + amount, MAX_CalciumCarbonateCount);
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
    case CarbonicAcid:
        currentCarbonicAcidCount = std::max(currentCarbonicAcidCount - amount, MIN_CarbonicAcidCount);
        break;
    case Carbonate:
        currentCarbonateCount = std::max(currentCarbonateCount - amount, MIN_CarbonateCount);
        break;
    case BiCarbonate:
        currentBiCarbonateCount = std::max(currentBiCarbonateCount - amount, MIN_biCarbonateCount);
        break;
    case CalciumCarbonate:
        currentCalciumCarbonateCount = std::max(currentCalciumCarbonateCount - amount, MIN_CalciumCarbonateCount);
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
    InitializeMolecules(CarbonicAcid);
    InitializeMolecules(Carbonate);
    InitializeMolecules(BiCarbonate);
    InitializeMolecules(CalciumCarbonate);

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
                case sf::Keyboard::S:
                    DecreaseMoleculeCount(CarbonDioxide, 10);
                    break;
                
                case sf::Keyboard::Q:
                    IncreaseMoleculeCount(CarbonicAcid, 10);
                    break;
                case sf::Keyboard::W:
                    DecreaseMoleculeCount(CarbonicAcid, 10);
                    break;
                case sf::Keyboard::Z:
                    IncreaseMoleculeCount(Carbonate, 10);
                    break;                      
                case sf::Keyboard::X:           
                    DecreaseMoleculeCount(Carbonate, 10);
                    break;
                case sf::Keyboard::D:
                    IncreaseMoleculeCount(BiCarbonate, 10);
                    break;
                case sf::Keyboard::F:
                    DecreaseMoleculeCount(BiCarbonate, 10);
                    break;
                case sf::Keyboard::E:
                    IncreaseMoleculeCount(CalciumCarbonate, 10);
                    break;
                case sf::Keyboard::R:
                    DecreaseMoleculeCount(CalciumCarbonate, 10);
                    break;
                }
            }
        }

        // Animate molecules
        AnimateMolecules(CarbonDioxide);
        AnimateMolecules(CarbonicAcid);
        AnimateMolecules(Carbonate);
        AnimateMolecules(BiCarbonate);
        AnimateMolecules(CalciumCarbonate);

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the water
        window.draw(water);

        // Draw the molecules
        for (int i = 0; i < currentCarbonDioxideCount; i++)
        {
            window.draw(CarbonDioxideMolecules[i]);
        }
        for (int i = 0; i < currentCarbonicAcidCount; i++)
        {
            window.draw(CarbonicAcidMolecules[i]);
        }
        for (int i = 0; i < currentCarbonateCount; i++)
        {
            window.draw(CarbonateMolecules[i]);
        }
        for (int i = 0; i < currentBiCarbonateCount; i++)
        {
            window.draw(biCarbonateMolecules[i]);
        }
        for (int i = 0; i < currentCalciumCarbonateCount; i++)
        {
            window.draw(CalciumCarbonateMolecules[i]);
        }

        // Draw the text
        window.draw(text);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
