#include <SFML/Graphics.hpp>
#include <string.h>

#include "utils.h"
#include "vector.h"

Vector func(Vector z, int funcId_)
{
    switch(funcId_)
    {
        case 4:
        {
            return conjugate_c(z);
        }
        case 3:
        {
            Vector a = power_comp_c(sub_vec_v(z, vec_v(2.f, 0.f)), 2);
            Vector b = add_vec_v(z, vec_v(1.f,-2.f));
            Vector c = add_vec_v(z, vec_v(2.f,2.f));
            Vector d = power_comp_c(z, 3);
            return divide_comp_c(mult_comp_c(mult_comp_c(a, b), c), d);
        }
        case 2:
        {
            return add_vec_v(mult_scalar_v(z, 2.f), vec_v(0.f, 1.f));
        }
        case 1:
        {
            return power_comp_c(z, 3);
        }
        case 0:
        {
            return sub_vec_v( sub_vec_v( power_comp_c(z, 10) , z ), vec_v(1.f,0.f) );
        }
        default:
        {
            return vec_v(0.f, 0.f);
        }
    }
}

std::string get_function_string(int funcId_)
{
    switch(funcId_)
    {
        case 4:
        {
            return "f(z) = conjugate(z)";
        }
        case 3:
        {
            return "f(z) = (z - 2)**2 * (z + 1 - 2i) * ( z + 2 + 2i) / z**3";
        }
        case 2:
        {
            return "f(z) = z*2 + i";
        }
        case 1:
        {
            return "f(z) = z**3";
        }
        case 0:
        {
            return "f(z) = z**10 - z - 1";
        }
        default:
        {
            return "";
        }
    }
}

inline void shader_on_tex(sf::Shader* shader_,sf::RenderTexture* renderTex_)
{
    sf::Sprite tempS;
    sf::RenderTexture tempT;
    tempT.create(SCREEN_W, SCREEN_H);
    tempS.setTexture(tempT.getTexture());
    tempT.clear();
    sf::Sprite renderSprt;
    renderSprt.setTexture(renderTex_->getTexture());

    tempT.draw(renderSprt, shader_);
    renderTex_->draw(tempS);
}

void centerText(sf::Text* t)
{
    sf::FloatRect textRect = t->getLocalBounds();
    t->setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    sf::RenderWindow window(sf::VideoMode(SCREEN_W, SCREEN_H, 32), "Domain Color", sf::Style::Titlebar);
    window.setVerticalSyncEnabled(true);

    sf::Sprite renderSprt;
    sf::RenderTexture renderTex;
    renderTex.create(SCREEN_H, SCREEN_H);
    renderSprt.setTexture(renderTex.getTexture());
    renderTex.clear();

    sf::Font font;
    font.loadFromFile("res/GoogleSans-Bold.ttf");

    float worldSize = WORLD_SIZE;
    sf::Text text;
    text.setFont(font); // font is a sf::Font
    // text.setOrigin()
    sf::Vector2f mouseWorlPos;
    sf::Vector2f mouseWorlFuncPos;
    sf::Vector2f screenSize((float)SCREEN_W, (float)SCREEN_H);
    window.setMouseCursorVisible(false);
    std::string posSign;
    std::string functionString;

    int functionId = 0;
    functionString = get_function_string(functionId);

    int tick = 0;

    sf::Shader* updateShader = new sf::Shader;
    updateShader->loadFromFile("res/update.frag", sf::Shader::Fragment);
    updateShader->setUniform("texSize", sf::Vector2f(worldSize, worldSize));
    renderSprt.setOrigin(sf::Vector2f((float)SCREEN_H/2.f, (float)SCREEN_H/2.f));



    renderTex.clear();

    while (window.isOpen())
    {
        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        mouseWorlPos = sf::Vector2f(mousePos.x/screenSize.x*worldSize,mousePos.y/screenSize.y*worldSize) - sf::Vector2f(worldSize, worldSize)/2.f;
        mouseWorlPos.y = -mouseWorlPos.y;
        mouseWorlFuncPos = vec_to_sfvec2_v(func(sfvec2_to_vec_v(mouseWorlPos), functionId));
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                functionId -= 1;
                if (functionId < 0)
                {
                    functionId += 5;
                }
                functionId = functionId % 5;
                functionString = get_function_string(functionId);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                functionId += 1;
                functionId = functionId % 5;
                functionString = get_function_string(functionId);
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta < 0.f)
                {
                    worldSize += 0.1f;
                    updateShader->setUniform("texSize", sf::Vector2f(worldSize, worldSize));
                }
                else
                {
                    if (worldSize-1.f > 0.f)
                    {
                        worldSize -= 0.1f;
                    }
                    updateShader->setUniform("texSize", sf::Vector2f(worldSize, worldSize));
                }
            }
        }

        tick++;

        window.clear();

        text.setCharacterSize(screenSize.x/40.f); // in pixels, not points!
        text.setFillColor(sf::Color::White);
        
        renderSprt.setScale( sf::Vector2f(0.5f, 0.5f) );
        updateShader->setUniform(  "mousePos", sf::Vector2f( mousePos.x/screenSize.x, mousePos.y/screenSize.y )  );
        updateShader->setUniform("functionId", functionId);

        renderSprt.setPosition(  sf::Vector2f( screenSize.x/4.f, screenSize.y/2.f )  );
        updateShader->setUniform("isForOutput", false);
        shader_on_tex(updateShader, &renderTex);
        window.draw(renderSprt);

        text.setString("Input");
        centerText(&text);
        text.setPosition(  sf::Vector2f( screenSize.x/4.f, screenSize.y/1.8f +  screenSize.y/4.f ) );
        window.draw(text);

        renderSprt.setPosition(  sf::Vector2f( screenSize.x - screenSize.x/4.f, screenSize.y/2.f )  );
        updateShader->setUniform("isForOutput", true);
        shader_on_tex(updateShader, &renderTex);
        window.draw(renderSprt);

        text.setString("Output");
        centerText(&text);
        text.setPosition(  sf::Vector2f( screenSize.x - screenSize.x/4.f, screenSize.y/1.8f +  screenSize.y/4.f ) );
        window.draw(text);

        text.setString(functionString);
        centerText(&text);
        text.setPosition(  sf::Vector2f( screenSize.x/2.f, screenSize.y/6.f ) );
        window.draw(text);

        posSign = "+";
        if (mouseWorlPos.y < 0)
        {
            posSign = "";
        }
        text.setString(std::to_string(mouseWorlPos.x)+posSign+std::to_string(mouseWorlPos.y)+"i");
        centerText(&text);
        text.setPosition(  sf::Vector2f( screenSize.x/4.f, screenSize.y/1.65f +  screenSize.y/4.f ) );
        window.draw(text);

        posSign = "+";
        if (mouseWorlFuncPos.y < 0)
        {
            posSign = "";
        }
        text.setString(std::to_string(mouseWorlFuncPos.x)+posSign+std::to_string(mouseWorlFuncPos.y)+"i");
        centerText(&text);
        text.setPosition(  sf::Vector2f( screenSize.x - screenSize.x/4.f, screenSize.y/1.65f +  screenSize.y/4.f ) );
        window.draw(text);

        window.display();
    }
    return EXIT_SUCCESS;
}
