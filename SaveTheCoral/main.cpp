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

// Window
sf::RenderWindow window;

// Background shapes
sf::RectangleShape menu;
sf::RectangleShape water;
sf::RectangleShape coral;

// Universal shape properties
float shapeOutlineThickness = 1;
sf::Color shapeOutlineColor(100, 100, 200);

// Water properties
sf::Color waterColor(32, 178, 170);

// Carbon dioxide properties
const int MIN_CarbonDioxideCount = 100;
const int MAX_CarbonDioxideCount = 200;
sf::Color carbonDioxideColor(sf::Color::Red);
sf::CircleShape carbonDioxideMolecules[MAX_CarbonDioxideCount];
int currentCarbonDioxideCount = 124;
int carbonDioxideSpeed = 6;
float carbonDioxideSize = 4;

// Carbonic acid properties
const int MIN_CarbonicAcidCount = MIN_CarbonDioxideCount;
const int MAX_CarbonicAcidCount = MAX_CarbonDioxideCount;
sf::Color carbonicAcidColor(255, 165, 0);
sf::CircleShape carbonicAcidMolecules[MAX_CarbonicAcidCount];
int currentCarbonicAcidCount = currentCarbonDioxideCount;
int carbonicAcidSpeed = 4;
float carbonicAcidSize = 6.5;

// Carbonate properties
const int MIN_CarbonateCount = MIN_CarbonDioxideCount;
const int MAX_CarbonateCount = MAX_CarbonDioxideCount;
sf::Color carbonateColor(255, 160, 122);
sf::CircleShape carbonateMolecules[MAX_CarbonateCount];
int currentCarbonateCount = 300 - currentCarbonicAcidCount;
int carbonateSpeed = 4;
float carbonateSize = 6;

// Bi-carbonate properties
const int MIN_biCarbonateCount = MIN_CarbonDioxideCount;
const int MAX_biCarbonateCount = MAX_CarbonDioxideCount;
sf::Color biCarbonateColor(25, 25, 112);
sf::CircleShape biCarbonateMolecules[MAX_biCarbonateCount];
int currentBiCarbonateCount = currentCarbonDioxideCount;
int biCarbonateSpeed = 4;
float biCarbonateSize = 6;

// Calcium carbonate properties
const int MIN_CalciumCarbonateCount = MIN_CarbonDioxideCount;
const int MAX_CalciumCarbonateCount = MAX_CarbonDioxideCount;
sf::Color calciumCarbonateColor(255, 248, 220);
sf::CircleShape calciumCarbonateMolecules[MAX_CalciumCarbonateCount];
int currentCalciumCarbonateCount = currentCarbonateCount;
int calciumCarbonateSpeed = 2;
float calciumCarbonateSize = 10;

// Menu text
sf::Font font;
sf::Color textColor(sf::Color::Black);
sf::Text textMenuTitle;
sf::Text textMenuCarbonDioxide;
sf::Text textMenuCarbonate;

void InitializeMenu()
{
    int xPos = 10;
    int yPos = 10;
    int fontSize;

    fontSize = 40;
    textMenuTitle.setFont(font);
    textMenuTitle.setString("Welcome to Save the Coral Simulation");
    textMenuTitle.setCharacterSize(fontSize);
    textMenuTitle.setFillColor(textColor);
    textMenuTitle.setPosition(xPos, yPos);
    yPos += fontSize + 10;

    fontSize = 20;
    textMenuCarbonDioxide.setFont(font);
    textMenuCarbonDioxide.setCharacterSize(fontSize);
    textMenuCarbonDioxide.setFillColor(textColor);
    textMenuCarbonDioxide.setPosition(xPos, yPos);
    yPos += fontSize + 10;

    fontSize = 20;
    textMenuCarbonate.setFont(font);
    textMenuCarbonate.setCharacterSize(fontSize);
    textMenuCarbonate.setFillColor(textColor);
    textMenuCarbonate.setPosition(xPos, yPos);
    yPos += fontSize + 10;
}

void UpdateText()
{
    wchar_t buffer[1000];

    wsprintfW(buffer, L"Carbon Dioxide control: [a] increase, [s] decrease  [Min: %d, Current: %d, Max: %d]", 
        MIN_CarbonDioxideCount, currentCarbonDioxideCount, MAX_CarbonDioxideCount);
    textMenuCarbonDioxide.setString(buffer);

    wsprintfW(buffer, L"Carbonate control: [z] increase, [x] decrease  [Min: %d, Current: %d, Max: %d]",
        MIN_CarbonateCount, currentCarbonateCount, MAX_CarbonateCount);
    textMenuCarbonate.setString(buffer);
}

void InitializeWater()
{
    water.setPosition(waterRect.left, waterRect.top);
    water.setSize(sf::Vector2f(waterRect.width, waterRect.height));
    water.setOutlineColor(waterColor);
    water.setFillColor(waterColor);
}

void InitializeMolecules(Molecule type)
{
    switch (type)
    {
    case CarbonDioxide:
        for (int i = 0; i < MAX_CarbonDioxideCount; i++)
        {
            float x = std::rand() % (int) waterRect.width;
            float y = std::rand() % (int) waterRect.height;

            carbonDioxideMolecules[i].setPosition(x, waterRect.top + y);
            carbonDioxideMolecules[i].setFillColor(carbonDioxideColor);
            carbonDioxideMolecules[i].setRadius(carbonDioxideSize);
            carbonDioxideMolecules[i].setOutlineThickness(shapeOutlineThickness);
            carbonDioxideMolecules[i].setOutlineColor(shapeOutlineColor);
        }
        break;
    case CarbonicAcid:
        for (int i = 0; i < MAX_CarbonicAcidCount; i++)
        {
            float x = std::rand() % (int)waterRect.width;
            float y = std::rand() % (int)waterRect.height;

            carbonicAcidMolecules[i].setPosition(x, waterRect.top + y);
            carbonicAcidMolecules[i].setFillColor(carbonicAcidColor);
            carbonicAcidMolecules[i].setRadius(carbonicAcidSize);
            carbonicAcidMolecules[i].setOutlineThickness(shapeOutlineThickness);
            carbonicAcidMolecules[i].setOutlineColor(shapeOutlineColor);   
        }
        break;
    case Carbonate:
        for (int i = 0; i < MAX_CarbonateCount; i++)
        {
            float x = std::rand() % (int)waterRect.width;
            float y = std::rand() % (int)waterRect.height;

            carbonateMolecules[i].setPosition(x, waterRect.top + y);
            carbonateMolecules[i].setFillColor(carbonateColor);
            carbonateMolecules[i].setRadius(carbonateSize);
            carbonateMolecules[i].setOutlineThickness(shapeOutlineThickness);
            carbonateMolecules[i].setOutlineColor(shapeOutlineColor);
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

            calciumCarbonateMolecules[i].setPosition(x, waterRect.top + y);
            calciumCarbonateMolecules[i].setFillColor(calciumCarbonateColor);
            calciumCarbonateMolecules[i].setRadius(calciumCarbonateSize);
            calciumCarbonateMolecules[i].setOutlineThickness(shapeOutlineThickness);
            calciumCarbonateMolecules[i].setOutlineColor(shapeOutlineColor);
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
            sf::Vector2f currentPosition = carbonDioxideMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % carbonDioxideSpeed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % carbonDioxideSpeed);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            carbonDioxideMolecules[i].setPosition(currentPosition);
        }
        break;
    case CarbonicAcid:
        for (int i = 0; i < currentCarbonicAcidCount; i++)
        {
            sf::Vector2f currentPosition = carbonicAcidMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % carbonicAcidSpeed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % carbonicAcidSpeed);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            carbonicAcidMolecules[i].setPosition(currentPosition);
        }
        break;
    case Carbonate:
        for (int i = 0; i < currentCarbonateCount; i++)
        {
            sf::Vector2f currentPosition = carbonateMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % carbonateSpeed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % carbonateSpeed);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

            carbonateMolecules[i].setPosition(currentPosition);
        }
        break;
    case BiCarbonate:
        for (int i = 0; i < currentBiCarbonateCount; i++)
        {
            sf::Vector2f currentPosition = biCarbonateMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % biCarbonateSpeed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % biCarbonateSpeed);

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
            sf::Vector2f currentPosition = calciumCarbonateMolecules[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % calciumCarbonateSpeed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % calciumCarbonateSpeed);

            currentPosition.x = std::min(currentPosition.x, waterRect.left + waterRect.width);
            currentPosition.y = std::min(currentPosition.y, waterRect.top + waterRect.height);
            currentPosition.x = std::max(currentPosition.x, waterRect.left);
            currentPosition.y = std::max(currentPosition.y, waterRect.top);

           calciumCarbonateMolecules[i].setPosition(currentPosition);
        }
        break;
    }
}

void IncreaseCarbonDioxide(int amount)
{
    currentCarbonDioxideCount = std::min(currentCarbonDioxideCount + amount, MAX_CarbonDioxideCount);
    currentCarbonicAcidCount = currentCarbonDioxideCount;
    currentBiCarbonateCount = currentCarbonDioxideCount;
    currentCarbonateCount = 300 - currentCarbonicAcidCount;
    currentCalciumCarbonateCount = currentCarbonateCount;
}

void DecreaseCarbonDioxide(int amount)
{
    currentCarbonDioxideCount = std::max(currentCarbonDioxideCount - amount, MIN_CarbonDioxideCount);
    currentCarbonicAcidCount = currentCarbonDioxideCount;
    currentBiCarbonateCount = currentCarbonDioxideCount;
    currentCarbonateCount = 300 - currentCarbonicAcidCount;
    currentCalciumCarbonateCount = currentCarbonateCount;
}

void Initialize()
{
    std::srand(GetTickCount());

    // Create the window of the application
    window.create(sf::VideoMode(windowWidth, windowHeight, 32), "Sample graphics", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    font.loadFromFile("resources/sansation.ttf");

    // Create water rectangle shape
    InitializeWater();

    // Create text objects
    InitializeMenu();

    // Initialize molecules
    InitializeMolecules(CarbonDioxide);
    InitializeMolecules(CarbonicAcid);
    InitializeMolecules(Carbonate);
    InitializeMolecules(BiCarbonate);
    InitializeMolecules(CalciumCarbonate);
}

void RenderWindow()
{
    // Clear the window
    window.clear(sf::Color::White);

    // Draw the water
    window.draw(water);

    // Draw the molecules
    for (int i = 0; i < currentCarbonDioxideCount; i++)
    {
        window.draw(carbonDioxideMolecules[i]);
    }
    for (int i = 0; i < currentCarbonicAcidCount; i++)
    {
        window.draw(carbonicAcidMolecules[i]);
    }
    for (int i = 0; i < currentCarbonateCount; i++)
    {
        window.draw(carbonateMolecules[i]);
    }
    for (int i = 0; i < currentBiCarbonateCount; i++)
    {
        window.draw(biCarbonateMolecules[i]);
    }
    for (int i = 0; i < currentCalciumCarbonateCount; i++)
    {
        window.draw(calciumCarbonateMolecules[i]);
    }

    // Draw the text
    window.draw(textMenuTitle);
    window.draw(textMenuCarbonDioxide);
    window.draw(textMenuCarbonate);

    // Display things on screen
    window.display();
}

void HandleEvents()
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
                IncreaseCarbonDioxide(1);
                break;
            case sf::Keyboard::S:
                DecreaseCarbonDioxide(1);
                break;
            }
        }
    }
}

void UpdateSimulation()
{
    // Animate molecules
    AnimateMolecules(CarbonDioxide);
    AnimateMolecules(CarbonicAcid);
    AnimateMolecules(Carbonate);
    AnimateMolecules(BiCarbonate);
    AnimateMolecules(CalciumCarbonate);

    // Update text strings
    UpdateText();
}

int main()
{
    Initialize();

    while (window.isOpen())
    {
        HandleEvents();

        UpdateSimulation();

        RenderWindow();
    }

    return EXIT_SUCCESS;
}
