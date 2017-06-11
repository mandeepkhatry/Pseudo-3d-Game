#ifndef MENU_H
#define MENU_H
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

#define No_of_Items 4

class Menu
{
private:
    sf::Text text[No_of_Items];

    static sf::Font font;
    static sf::SoundBuffer sbuffer;
    static sf::Sound click_sound;

    int selectedItemIndex;

public:
    static void LoadAssests ()
    {
        font.loadFromFile("cooper.ttf");
        sbuffer.loadFromFile("sound\\click.ogg");
        click_sound.setBuffer(sbuffer);
        click_sound.setLoop(false);
    }

    Menu(int width, int height)
    {
        for(int i=0; i<No_of_Items; i++)
        {
            text[i].setFont(font);
            text[i].setFillColor(sf::Color(255,255,255));
            text[i].setPosition(width/2-100, 200+100*i);
        }

        text[0].setString("Play");
        text[0].setFillColor(sf::Color(6,248,245));
        text[1].setString("Instructions");
        text[2].setString("High Score");
        text[3].setString("Exit");

        selectedItemIndex = 0;
    }

    ~Menu () {}

    void draw(sf::RenderWindow &window)
    {
        for(int i = 0; i< No_of_Items; i++)
        {
            window.draw(text[i]);
        }
    }

    void MoveUp()
    {
        if(selectedItemIndex > 0)
        {
            text[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex--;
            text[selectedItemIndex].setFillColor(sf::Color(6,245,248));
            click_sound.play();
        }
    }

    void MoveDown()
    {
        if(selectedItemIndex < No_of_Items)
        {
            text[selectedItemIndex].setFillColor(sf::Color::White);
            selectedItemIndex++;
            text[selectedItemIndex].setFillColor(sf::Color(6,245,248));
            click_sound.play();
        }
    }

    int GetPressedItem() {return selectedItemIndex;}
};

bool LoadMenu(sf::RenderWindow&, bool);

#endif // MENU_H
