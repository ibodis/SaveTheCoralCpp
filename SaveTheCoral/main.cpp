#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <Windows.h>

#define MAX_SHAPES 1000

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
sf::Rect<float> menuRect(0, 0, windowWidth, 150);
sf::Rect<float> reefRect(0, menuRect.height, windowWidth, windowHeight - menuRect.height);

// Window
sf::RenderWindow window;

// Sounds
sf::SoundBuffer backgroundSoundBuffer;
sf::SoundBuffer wavesSoundBuffer;
sf::Sound backgroundSound;
sf::Sound wavesSound;

// Images
sf::Texture reefTexture;
sf::Sprite reefSprite;
sf::Shader reefShader;

// Background shapes
sf::RectangleShape menu;
sf::RectangleShape coral;

// Universal shape properties
float shapeOutlineThickness = 1;
sf::Color shapeOutlineColor(100, 100, 200);

int carbonateSpeed = 4;
typedef struct MoleculeData
{
    int Min;
    int Max;
    int Count;
    sf::Color Color;
    sf::CircleShape Shapes[MAX_SHAPES];
    float Size;
    int Speed;

    MoleculeData()
    {
        Min = 0;
        Max = 0;
        Count = 0;
        Color = sf::Color::Black;
        Size = 0;
        Speed = 0;
    }

    void Initialize(int min, int max, sf::Color color, float size, int speed)
    {
        Min = min;
        Max = max;
        Color = color;
        Size = size;
        Speed = speed;

        for (int i = 0; i < MAX_SHAPES; i++)
        {
            float x = reefRect.left + std::rand() % (int)reefRect.width;
            float y = reefRect.top + std::rand() % (int)reefRect.height;
            Shapes[i].setPosition(x, y);
            Shapes[i].setFillColor(Color);
            Shapes[i].setRadius(size);
            Shapes[i].setOutlineThickness(shapeOutlineThickness);
            Shapes[i].setOutlineColor(shapeOutlineColor);
        }
    }

    void Animate()
    {
        for (int i = 0; i < Count; i++)
        {
            sf::Vector2f currentPosition = Shapes[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % Speed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % Speed);

            currentPosition.x = std::min(currentPosition.x, reefRect.left + reefRect.width);
            currentPosition.y = std::min(currentPosition.y, reefRect.top + reefRect.height);
            currentPosition.x = std::max(currentPosition.x, reefRect.left);
            currentPosition.y = std::max(currentPosition.y, reefRect.top);

            Shapes[i].setPosition(currentPosition);
        }
    }

    void Draw()
    {
        for (int i = 0; i < Count; i++)
        {
            window.draw(Shapes[i]);
        }
    }

    int GetRange()
    {
        return Max - Min;
    }
};

MoleculeData carbonDioxide;
MoleculeData carbonicAcid;
MoleculeData carbonate;
MoleculeData biCarbonate;
MoleculeData calciumCarbonate;

// Menu text
sf::Font font;
sf::Color textColor(sf::Color::Black);
sf::Text textMenuTitle;
sf::Text textMenuCarbonDioxide;

void AdjustCarbonDioxide(int amount)
{
    // This is what the user can change
    carbonDioxide.Count = std::clamp(carbonDioxide.Count + amount, carbonDioxide.Min, carbonDioxide.Max);

    float polutionFactor = (float)(carbonDioxide.Count - carbonDioxide.Min) / (float)(carbonDioxide.GetRange());

    // As carbon dioxide increases, so does carbonic acid, which is produced when carbon dioxide reacts with water
    carbonicAcid.Count = carbonicAcid.Min + (int)((float)carbonicAcid.GetRange() * polutionFactor);

    // As carbonic acid levels go up, they react with carbonate in the water, therefore carbonate levels go down
    carbonate.Count = carbonate.Max - (int)((float)carbonate.GetRange() * polutionFactor);

    // As carbonic acid levels go up, so do bi-carbonate levels which is produced when carbonic acid reacts with carbonate
    biCarbonate.Count = biCarbonate.Min + (int)((float)biCarbonate.GetRange() * polutionFactor);

    // As carbonate levels drop, there will be less carbonate to form calcium carbonate by the corals, therefore calcium carbonate levels also go down
    calciumCarbonate.Count = calciumCarbonate.Max - (int)((float)calciumCarbonate.GetRange() * polutionFactor);
}

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
}

void Initialize()
{
    std::srand(GetTickCount());

    carbonDioxide.Initialize(280, 600, sf::Color::Red, 4, 6);
    carbonicAcid.Initialize(100, 500, sf::Color(255, 165, 0), 6.5, 4);
    carbonate.Initialize(20, 300, sf::Color(255, 160, 122), 4, 6);
    biCarbonate.Initialize(10, 200, sf::Color(25, 25, 112), 6, 4);
    calciumCarbonate.Initialize(10, 200, sf::Color(255, 248, 220), 10, 2);

    AdjustCarbonDioxide(0);

    // Create the window of the application
    window.create(sf::VideoMode(windowWidth, windowHeight, 32), "Sample graphics", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    // Load the font
    font.loadFromFile("resources/sansation.ttf");

    // Load the sounds
    backgroundSoundBuffer.loadFromFile("resources/underwaterpool.wav");
    backgroundSound.setBuffer(backgroundSoundBuffer);
    backgroundSound.setLoop(true);
    backgroundSound.play();

    wavesSoundBuffer.loadFromFile("resources/seawaves.wav");
    wavesSound.setBuffer(wavesSoundBuffer);
    wavesSound.setVolume(50);
    wavesSound.setLoop(true);
    wavesSound.play();

    // Load images
    reefTexture.loadFromFile("resources/reef.jpg");
    reefTexture.setSmooth(true);

    // Initialize reef sprite
    reefSprite.setTexture(reefTexture);
    reefSprite.setPosition(reefRect.left, reefRect.top);
    sf::Vector2u reefSize = reefTexture.getSize();
    reefSprite.setScale(sf::Vector2f((float)reefRect.width / (float)reefSize.x, (float)reefRect.height / (float)reefSize.y));
    reefSprite.setColor(sf::Color::Black);

    // Create greyscale shader
    const std::string fragmentShader = \
        "uniform sampler2D texture;" \
        "uniform float greyScale;" \
        "void main()" \
        "{" \
        // Read the pixel color
        "    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
        // Determine the greyness of this pixel
        "    float greyValue = (pixel.x + pixel.y + pixel.z)/3;" \
        // Move each color value towards the grey pixel value by the greyScale amount
        "    float r = pixel.x + (greyValue - pixel.x) * greyScale;" \
        "    float g = pixel.y + (greyValue - pixel.y) * greyScale;" \
        // Leave a slight blue bias
        "    float b = pixel.z + (greyValue - pixel.z) * greyScale;" \
        // Set the output pixel color (leave alpha channel as it was)
        "    gl_FragColor = vec4(r, g, b, pixel.w);" \
        "}";
    reefShader.loadFromMemory(fragmentShader, sf::Shader::Fragment);
    reefShader.setUniform("texture", sf::Shader::CurrentTexture);

    // Create text objects
    InitializeMenu();
}

void UpdateText()
{
    wchar_t buffer[1000];

    wsprintfW(buffer, L"Carbon Dioxide control: [a] increase, [s] decrease  [Min: %d ppm, Current: %d ppm, Max: %d ppm]", 
        carbonDioxide.Min, carbonDioxide.Count, carbonDioxide.Max);
    textMenuCarbonDioxide.setString(buffer);
}

void DrawWindow()
{
    // Clear the window
    window.clear(sf::Color::White);

    // Draw the reef
    float greyScale = ((float)carbonDioxide.Count - (float)carbonDioxide.Min) / ((float)carbonDioxide.Max - (float)carbonDioxide.Min);
    reefShader.setUniform("greyScale", greyScale);
    window.draw(reefSprite, &reefShader);

    // Draw the molecules
    carbonDioxide.Draw();
    carbonicAcid.Draw();
    carbonate.Draw();
    biCarbonate.Draw();
    calciumCarbonate.Draw();

    // Draw the text
    window.draw(textMenuTitle);
    window.draw(textMenuCarbonDioxide);

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
                AdjustCarbonDioxide(5);
                break;
            case sf::Keyboard::S:
                AdjustCarbonDioxide(-5);
                break;
            }
        }
    }
}

void UpdateSimulation()
{
    // Animate molecules
    carbonDioxide.Animate();
    carbonicAcid.Animate();
    carbonate.Animate();
    biCarbonate.Animate();
    calciumCarbonate.Animate();

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

        DrawWindow();
    }

    return EXIT_SUCCESS;
}
