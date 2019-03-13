#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <Windows.h>

#define MAX_SHAPES 1000

// Define some constants
const float windowWidth = 2000;
const float windowHeight = 1400;
const float menuHeight = 220;

// Window
sf::RenderWindow window;

// Screen areas
sf::Rect<float> reefRect(0.0f, menuHeight, windowWidth, windowHeight - menuHeight);

// Sounds
sf::SoundBuffer backgroundSoundBuffer;
sf::SoundBuffer wavesSoundBuffer;
sf::Sound backgroundSound;
sf::Sound wavesSound;

// Images
sf::Texture reefTexture;
sf::Sprite reefSprite;
sf::Shader reefShader;

// Universal shape properties
float shapeOutlineThickness = 3;
sf::Color shapeOutlineColor(100, 100, 100);

// Font and text color
sf::Font font;
sf::Color textColor(sf::Color::Black);

// Text elements
sf::Text textMenuTitle;
sf::Text textMenuCarbonDioxide;

// Define struct to keep track of various simulation data
struct VariableData
{
    sf::String Name;
    float Min = 0;
    float Max = 0;
    float Level = 0;
    float Size = 0;
    int Speed = 0;
    sf::Color Color = sf::Color::Black;
    sf::CircleShape Shapes[MAX_SHAPES];

    void Initialize(const sf::String name, float min, float max, sf::Color color, float size, int speed)
    {
        Name = name;
        Min = min;
        Max = max;
        Color = color;
        Size = size;
        Speed = speed;

        // Initialize shapes
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

    float GetRange()
    {
        return Max - Min;
    }

    void DrawShapes()
    {
        // Only draw up to current Level shapes
        for (int i = 0; i < Level; i++)
        {
            sf::Vector2f currentPosition = Shapes[i].getPosition();

            currentPosition.x += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % Speed);
            currentPosition.y += (std::rand() % 2 == 0 ? -1 : 1) * (std::rand() % Speed);

            currentPosition.x = std::min(currentPosition.x, reefRect.left + reefRect.width);
            currentPosition.y = std::min(currentPosition.y, reefRect.top + reefRect.height);
            currentPosition.x = std::max(currentPosition.x, reefRect.left);
            currentPosition.y = std::max(currentPosition.y, reefRect.top);

            Shapes[i].setPosition(currentPosition);

            window.draw(Shapes[i]);
        }
    }

    float DrawLegend(float x, float y, bool drawSampleShape = true)
    {
        float fontSize = 20;
        sf::Text text;
        text.setFont(font);
        text.setFillColor(textColor);
        
        if (drawSampleShape)
        {
            sf::CircleShape indicator;
            indicator.setPosition(x + (fontSize - Size) / 2.0f, y + (fontSize - Size) / 2.0f);
            indicator.setFillColor(Color);
            indicator.setRadius(Size);
            indicator.setOutlineThickness(shapeOutlineThickness);
            indicator.setOutlineColor(shapeOutlineColor);
            window.draw(indicator);
        }

        int nameOffset = 30;
        wchar_t buffer[1000];
        wsprintfW(buffer, L"%s", Name.toWideString().c_str());
        text.setString(buffer);
        text.setCharacterSize((int)fontSize);
        text.setPosition(x + nameOffset, y);

        window.draw(text);

        float sliderOffset = 250;
        float sliderRange = 200;
        float margin = 3;

        sf::RectangleShape bar;
        bar.setFillColor(Color);
        bar.setOutlineColor(sf::Color::Black);
        bar.setOutlineThickness(1.0f);
        bar.setSize(sf::Vector2f(sliderRange + (margin * 2), fontSize));
        bar.setPosition(x + sliderOffset, y);
        window.draw(bar);

        bar.setSize(sf::Vector2f(3.0f, fontSize));

        bar.setFillColor(sf::Color::Green);
        bar.setOutlineColor(sf::Color(50, 50, 50));
        bar.setOutlineThickness(3);
        int indicatorOffset = (int) ( sliderRange * ( (Level - Min) /  (Max - Min)));
        bar.setPosition(x + sliderOffset + margin + indicatorOffset, y);
        window.draw(bar);

        return y + fontSize + 10;
    }
} carbonDioxide, carbonicAcid, carbonate, biCarbonate, calciumCarbonate, phLevel, waterTemperature;

void AdjustCarbonDioxide(int amount)
{
    // This is what the user can change
    carbonDioxide.Level = std::clamp(carbonDioxide.Level + amount, carbonDioxide.Min, carbonDioxide.Max);

    float polutionFactor = (carbonDioxide.Level - carbonDioxide.Min) / carbonDioxide.GetRange();

    // As carbon dioxide increases, so does carbonic acid, which is produced when carbon dioxide reacts with water
    carbonicAcid.Level = carbonicAcid.Min + carbonicAcid.GetRange() * polutionFactor;

    // As carbonic acid levels go up, they react with carbonate in the water, therefore carbonate levels go down
    carbonate.Level = carbonate.Max - (carbonate.GetRange() * polutionFactor);

    // As carbonic acid levels go up, so do bi-carbonate levels which is produced when carbonic acid reacts with carbonate
    biCarbonate.Level = biCarbonate.Min + (biCarbonate.GetRange() * polutionFactor);

    // As carbonate levels drop, there will be less carbonate to form calcium carbonate by the corals, therefore calcium carbonate levels also go down
    calciumCarbonate.Level = calciumCarbonate.Max - (calciumCarbonate.GetRange() * polutionFactor);

    // Water gets more acidic (pH goes down) as the level of carbon dioxide goes up
    phLevel.Level = phLevel.Max - (phLevel.GetRange() * polutionFactor);

    // Due to global warming caused by CO2, the water temperature increases
    waterTemperature.Level = waterTemperature.Min + (phLevel.GetRange() * polutionFactor);
}

int SetText(sf::Text& text, int fontSize, int x, int y, const sf::String textString)
{
    text.setFont(font);
    text.setFillColor(textColor);

    text.setString(textString);
    text.setCharacterSize(fontSize);
    text.setPosition((float)x, (float)y);

    return y + fontSize + 10;
}

void Initialize()
{
    std::srand(GetTickCount());

    carbonDioxide.Initialize("Carbon Dioxide", 100, 200, sf::Color::Red, 5, 3);
    carbonicAcid.Initialize("Carbonic Acid", 100, 400, sf::Color(255, 165, 0), 8, 2);
    carbonate.Initialize("Carbonate", 20, 300, sf::Color::Green, 5, 3);
    biCarbonate.Initialize("Bi-Carbonate", 10, 200, sf::Color(25, 25, 112), 7, 2);
    calciumCarbonate.Initialize("Calcium Carbonate", 10, 200, sf::Color(255, 248, 220), 10, 2);
    phLevel.Initialize("pH Level", 0, 10, sf::Color::White, 10, 2);
    waterTemperature.Initialize("Water temperature", 0, 10, sf::Color::White, 10, 2);

    AdjustCarbonDioxide(0);

    // Create the window of the application
    window.create(sf::VideoMode((int)windowWidth, (int)windowHeight, 32), "Sample graphics", sf::Style::Titlebar | sf::Style::Close);
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
    reefSprite.setScale(sf::Vector2f(reefRect.width / reefSize.x, reefRect.height / reefSize.y));
    reefSprite.setColor(sf::Color::Black);

    // Create gray scale shader
    const std::string fragmentShader = \
        "uniform sampler2D texture;" \
        "uniform float grayScale;" \
        "void main()" \
        "{" \
        // Read the pixel color
        "    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
        // Determine the grayness of this pixel
        "    float grayValue = (pixel.x + pixel.y + pixel.z)/3;" \
        // Move each color value towards the gray pixel value by the grayScale amount and shift it towards sea blue (36, 187, 242) = (0.14, 0.73, 0.94) so that the water remains blue while the corals get bleached
        "    float r = pixel.x + ((grayValue - pixel.x) * (grayScale * 0.50));" \
        "    float g = pixel.y + ((grayValue - pixel.y) * (grayScale * 0.85));" \
        "    float b = pixel.z + ((grayValue - pixel.z) * (grayScale * 0.94));" \
        // Set the output pixel color (leave alpha channel as it was)
        "    gl_FragColor = vec4(r, g + (0.2 * grayScale), b + (0.3 * grayScale), pixel.w);" \
        "}";
    reefShader.loadFromMemory(fragmentShader, sf::Shader::Fragment);
    reefShader.setUniform("texture", sf::Shader::CurrentTexture);

    // Create text objects
    int xPos = 20;
    int yPos = 20;
    yPos = SetText(textMenuTitle, 40, xPos, yPos, "Welcome to \"Save the Coral\" Simulation");
    yPos = SetText(textMenuCarbonDioxide, 20, xPos, yPos, "To change the level of Carbon Dioxide: press 'Right' or 'Up' to increase; press 'Left' or 'Down' to decrease");
}

int main()
{
    Initialize();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            int changeAmount = 2;
            if ((event.type == sf::Event::KeyPressed))
            {
                switch (event.key.code)
                {
                case sf::Keyboard::A:
                case sf::Keyboard::Up:
                case sf::Keyboard::Right:
                    AdjustCarbonDioxide(changeAmount);
                    break;
                case sf::Keyboard::S:
                case sf::Keyboard::Down:
                case sf::Keyboard::Left:
                    AdjustCarbonDioxide(-changeAmount);
                    break;
                }
            }
        }

        //
        // Draw the window
        //

        window.clear(sf::Color::White); // Clear the window to white

        // Draw the reef
        float grayScale = (carbonDioxide.Level - carbonDioxide.Min) / (carbonDioxide.Max - carbonDioxide.Min);
        reefShader.setUniform("grayScale", grayScale);
        window.draw(reefSprite, &reefShader);

        // Draw the molecules
        carbonDioxide.DrawShapes();
        carbonicAcid.DrawShapes();
        carbonate.DrawShapes();
        biCarbonate.DrawShapes();
        calciumCarbonate.DrawShapes();

        // Draw the text
        window.draw(textMenuTitle);
        window.draw(textMenuCarbonDioxide);

        float x = 1000;
        float y = 30;
        y = carbonDioxide.DrawLegend(x, y);
        y = carbonicAcid.DrawLegend(x, y);
        y = biCarbonate.DrawLegend(x, y);
        y = carbonate.DrawLegend(x, y);
        y = calciumCarbonate.DrawLegend(x, y);

        x = 1500;
        y = 30;
        y = phLevel.DrawLegend(x, y, false);
        y = waterTemperature.DrawLegend(x, y, false);

        // Display things on screen
        window.display();
    }

    return EXIT_SUCCESS;
}
