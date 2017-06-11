#ifndef ROAD_H
#define ROAD_H
#include <SFML/Graphics.hpp>
#include<sstream>
#include<iostream>
#include<fstream>

//CLASS TO DRAW QUADRILATERALS FOR ROAD AND ENVIRONMENT//

class DrawQuad
{
private:
    sf::ConvexShape shape;

public:
    DrawQuad(sf::RenderWindow &app, sf::Color c, int x1, int y1, int w1, int x2, int y2, int w2)
    {
        shape.setPointCount(4);
        shape.setFillColor(c);
        shape.setPoint(0,sf::Vector2f(x1-w1,y1));
        shape.setPoint(1,sf::Vector2f(x2-w2,y2));
        shape.setPoint(2,sf::Vector2f(x2+w2,y2));
        shape.setPoint(3,sf::Vector2f(x1+w1,y1));
        app.draw(shape);
    }

    ~DrawQuad() {}
};

//--THIS IS JUST COZ MY COMPLIER IS NOT SUPPORTING std::to_string--//
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

//-----DRAW SCOREBOARD------//
class ScoreBoard
{
private:
    sf::RectangleShape box;
    sf::Text text[2];
    sf::Font font;

public:
    ScoreBoard(sf::RenderWindow &app, sf::Font font, int score, int coin)
    {
        font.loadFromFile("arial black.ttf");
        box.setSize(sf::Vector2f(310,160));
        box.setPosition(width/2-155, height/2-100);
        box.setFillColor(sf::Color(245,248,6,250));
        box.setOutlineColor(sf::Color::Red);
        box.setOutlineThickness(3);

        text[0].setPosition(box.getPosition().x+15,box.getPosition().y+10);
        text[0].setFillColor(sf::Color(136,1,131));
        text[0].setFont(font);
        text[0].setCharacterSize(28);
        text[0].setString("Uff! You are dead !");

        text[1].setPosition(box.getPosition().x+25,box.getPosition().y+50);
        text[1].setFillColor(sf::Color(4,181,162));
        text[1].setFont(font);
        text[1].setCharacterSize(20);


        if(checkHighScore(score,coin)==true)
            text[1].setString("Score: "+patch::to_string(score)+"\nCoin: "+patch::to_string(coin)+"\n\n  \tNEW HIGHSCORE");
        else
            text[1].setString("Score: "+patch::to_string(score)+"\nCoin: "+patch::to_string(coin));

        app.draw(box);
        app.draw(text[0]);
        app.draw(text[1]);
    }

    //----CHECK HIGHSCORE FROM FILE AND WRITE NEW HIGHSCORE----//
    bool checkHighScore(int score, int coin)
    {
        int hscore, hcoin;
        std::ifstream ifile("score.dat",std::ios::in|std::ios::app|std::ios::binary);

        if(ifile)
        {
            ifile.read((char*)&hscore,sizeof(hscore));
            ifile.read((char*)&hcoin,sizeof(hcoin));
            ifile.close();
        }

        if(hscore<=score)
        {
            std::ofstream ofile("score.dat",std::ios::out|std::ios::binary);
            ofile.write((char*)&score,sizeof(score));
            ofile.write((char*)&coin,sizeof(coin));
            ofile.close();

            return true;
        }
        else
            return false;
    }

    ~ScoreBoard() {}
};


#endif // ROAD_H
