#include "menu.h"

sf::Font Menu::font;
sf::SoundBuffer Menu::sbuffer;
sf::Sound Menu::click_sound;

class Info;

bool LoadMenu(sf::RenderWindow &app, bool flowIntro)
{
    Menu::LoadAssests();

    sf::Texture intro_img;
    intro_img.loadFromFile("intro.png");
    sf::Sprite intro(intro_img);

//    Info Instructions(sf::String("Run as far as you can !\nCollect as much coin as you can !\nYou have control over the player using the arrow keys:\n\tRight Arrow: Move Right\n\tLeft Arrow: Move Left\n\tUp Arrow: Move Up\nYour main goal: Avoid obstacle and touch the coins !"));

    bool flowMenu;

    if(!flowIntro)
        flowMenu = true;

    Menu Menu_items(app.getSize().x, app.getSize().y);

    app.setKeyRepeatEnabled(false);
    while(app.isOpen())
    {
        app.clear();

        if(flowIntro)
        {
            sf::Event ev;
            while(app.pollEvent(ev))
            {
                if(ev.type==sf::Event::Closed)
                    exit(0);
            }
            app.draw(intro);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                flowMenu = true;
                flowIntro = false;
            }
        }

        sf::Event event;
        if(flowMenu)
        {
        while(app.pollEvent(event))
        {
            if(event.type==sf::Event::Closed)
                {
                    exit(0);
                };

            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Down)
                    Menu_items.MoveDown();
            }

            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Up)
                    Menu_items.MoveUp();
            }

            if(event.type==sf::Event::KeyPressed)
            {
                if(event.key.code==sf::Keyboard::Return)
                {
                    switch(Menu_items.GetPressedItem())
                    {
                        case 0 : return true;
                        case 1 : break;// Instructions.draw(app);
                        case 2 : break;
                        case 3 : exit(0);
                    }
                }
            }

        }
            Menu_items.draw(app);
        }


        app.display();
    }

}

//----CLASS TO CREATE INSTRUCTIONS, ABOUT AND HIGHSCORE----//
class Info
{
private:
    sf::RectangleShape box;
    sf::Text InfoText;
public:
    Info(sf::String& text)
    {
        box.setPosition(100,100);
        box.setFillColor(sf::Color(245,248,6));
        box.setSize(sf::Vector2f(300,300));
        box.setOutlineColor(sf::Color::Red);
        box.setOutlineThickness(3);

        InfoText.setString(text);
        InfoText.setPosition(110,110);
        InfoText.setFillColor(sf::Color::White);
    }

    void draw(sf::RenderWindow& app)
    {
        app.draw(box);
        app.draw(InfoText);
    }

    ~Info() {}
};

