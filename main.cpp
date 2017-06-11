#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include "line.h"
#include "road.h"
#include "generator.h"
#include "menu.h"

using namespace sf;

int main()
{
    RenderWindow Load_Menu(VideoMode(width, height), "Dr. Strange - An Infinite Runner",Style::Close);

    if(LoadMenu(Load_Menu,true)== true)
        Load_Menu.close();
    else if(LoadMenu(Load_Menu,true)==false)
        return 0;

    RenderWindow app(VideoMode(width, height),"Dr. Strange - An Infinite Runner", Style::Close);
    app.setFramerateLimit(60);

//-----------------------GETTING THINGS READY--------------------------//
   //----LOAD BACKGROUND----//
    Texture background_img;
    background_img.loadFromFile("bg.jpg");
    background_img.setRepeated(true);

    Sprite background(background_img);
    background.setTextureRect(IntRect(0,0,5000,792));
    background.setPosition(-1500,-300);
//-------------------------------------------------------------//
   //---CREATE SCORE TEXT------//
    Font font;
    font.loadFromFile("cooper.ttf");
    Text score_text;
    score_text.setPosition(width-250, 25);
    score_text.setFont(font);
//------------------------------------------------------------//
   //----CREATE COLLIDER-------//
    CircleShape collider(30);
    float colliderX = width/2-40;
    float colliderY = height/2 + 200;
    collider.setPosition(colliderX,colliderY);

   //------PLAYER SETUP--------//
    Vector2i playerSize(210,350); //size of each frame

    Texture playerFoto;
    playerFoto.loadFromFile("man.png");

    Sprite player(playerFoto);
    player.setPosition(width/2-75,height/2-50);

    int framesNum = 27;
    float animDuration = 0.45; //seconds

    Clock clockForPlayer;
    float deltatime=0; //for elapsed time
//-------------------------------------------------------------------------//
   //----LOAD OBSTACLES----//
    Texture t[20];
    Sprite obstacle[20];
    for(int i=1; i<16; i++)
    {
        t[i].loadFromFile("images/"+patch::to_string(i)+".png");
        t[i].setSmooth(true);
        obstacle[i].setTexture(t[i]);
    }
   //----LOAD COIN----//
    Texture tcoin;
    tcoin.loadFromFile("coin.png");
    tcoin.setSmooth(true);
    Sprite coin;
    coin.setTexture(tcoin);


   //----LOAD SOUNDS-------//
    SoundBuffer coinbuff;
    coinbuff.loadFromFile("sound\\coin sound.ogg");
    Sound playCoin(coinbuff);

    SoundBuffer collisonbuff;
    collisonbuff.loadFromFile("sound\\impact sound.ogg");
    Sound collideSound(collisonbuff);

    Music gameMusic;
    gameMusic.openFromFile("sound\\game sound.ogg");
    gameMusic.setLoop(true);

//<----------NOW THE TIME TO GENERATE LINES----------------------->//
    generateLines();
    generateObstacles(obstacle);
    generateCoins(coin);

//-----------------SOME INITIALIZERS-----------------------------//
    int N = lines.size(); //total number of lines
    int posZ=0;
    int posX=0;
    int score = 0;
    int coin_collected = 0;

    bool isAlive = true;
    int speed=200;

    Clock clockForScore;
    float collideTime = 0;

    //----------------------gravity----------------------------------//
    float groundHeight = height/1.4;
    float gravity = 20.0f;
    bool isJumping =false;
    float moveSpeed = 300.0f;


   //----CLOCK FOR FRAME RATE INDEPENDENT GAME----//
  //  Clock mainClock;
  //  float mainClockEllapsed = 0;
  //  float runDuration = 120; //complete track once in 60 seconds

//--------------------WINDOW STARTS-------------------------------//

    app.setKeyRepeatEnabled(false);
    gameMusic.play();
   while(app.isOpen())
    {
        Event event;
        while(app.pollEvent(event))
        {
            if(event.type==Event::Closed)
                app.close();

            if(event.type==Event::KeyPressed){
                if(event.key.code==Keyboard::Up)
        {
            if(collider.getPosition().y>=(height/2+100)){
                isJumping=true;
                collider.move(0,-moveSpeed);
                player.move(0,-moveSpeed);

            }
        }
        }}

        if(Keyboard::isKeyPressed(Keyboard::Right))
        {
            if(collider.getPosition().x<colliderX+300)
                {collider.move(20,0);player.move(20,0);}
        }

        if(Keyboard::isKeyPressed(Keyboard::Left))
        {
            if(collider.getPosition().x>colliderX-300)
                {collider.move(-20,0);player.move(-20,0);}
        }



        if(player.getPosition().y <groundHeight&&collider.getPosition().y<groundHeight&&isJumping==true){
            player.move(0,gravity);
            collider.move(0,gravity);
        }


//----------X, Y, Z POSITIONs INITIALIZE---------------------//
        float x=0, dx=0;
        int maxy = height;
      //----INCREASE SPEED AFTER 3000 POINTS----//
        if(score>3000)
        {
            speed=300;
            animDuration = 0.35;
        }

        if(isAlive)
            posZ+=speed;

        if(posZ >= N*segL)
            posZ-=N*segL;
        if(posZ < 0)
            posZ += N*segL;

        int startPos=posZ/segL;

        if(isAlive)
            if(speed>0)
                background.move(-lines[startPos].getCurve()*2,0);

        if(isAlive)
            score = clockForScore.getElapsedTime().asMilliseconds()/20;
            score_text.setString("Score: "+patch::to_string(score)+"\nCoin: "+patch::to_string(coin_collected));

        //------COIN TOUCH TEST----//
        if(lines[startPos+5].getCoinBounds().intersects(collider.getGlobalBounds()))
            {
                coin_collected++;
                playCoin.play();
            }

        //------COLLISION TEST-------//
        if(isAlive)
        if(lines[startPos+10].getObstacleBounds().intersects(collider.getGlobalBounds()))
        {
            collideSound.play();
            isAlive = false;
        }


        app.clear();
        app.draw(background);

    //--------THIS IS THE MAIN LOOP FOR RUNNING--------//
        for(int n=startPos; n<startPos+300; n++)
        {
            Line &l = lines[n%N]; //Line &l is reference to each line being drawn

            l.projectToScreen(posX-x, 1500, posZ - (n>=N?N*segL:0));

            x+=dx;
            dx+=l.getCurve();

            if(l.getY()>=maxy)
                continue;

            Line p = lines[(n-1)%N]; //previous line

        //----------DEFINE COLORS---------------//
            Color environment = (n/5)%2?Color(2,41,2):Color(3,43,3);
            Color roadside = (n/3)%2?Color(253,227,8):Color(0,0,0);
            Color road = (n/12)%2?Color(107,107,107):Color(109,109,109);

        //----------DRAWING ROADS AND ENVIROMENT-------------------//
            DrawQuad env(app, environment,0,p.getY(),width,0,l.getY(),width);

            DrawQuad side(app, roadside, p.getX(),p.getY(), p.getW()*1.2, l.getX(),l.getY(), l.getW()*1.2);

            DrawQuad road_lane(app,road,p.getX(),p.getY(),p.getW(),l.getX(),l.getY(),l.getW());
        }


    //-------DRAWING OBSTACLES AND COINS----------//
        for(int n=startPos+300; n>startPos; n--)
        {
            lines[n%N].drawSprite(app);
            lines[n%N].drawCoin(app);
        }


    //------PLAYER ANIMATION-----------//
        if(isAlive)
            deltatime += clockForPlayer.restart().asSeconds();

        //---Get Current Frame----//
        int animFrame = static_cast<int>((deltatime/animDuration)*framesNum)%framesNum;

        //---Set Sprite Rectangle Based on Frame---//
        player.setTextureRect(IntRect(animFrame*playerSize.x,0,playerSize.x,playerSize.y));
//----------------------------------------------------------------------------------------//

        //app.draw(collider);
        if(isAlive)
        {
            app.draw(player);
            app.draw(score_text);
        }


     //------SCORE BOARD-------//
        if(isAlive==false)
        {
            collideTime+=clockForScore.restart().asSeconds();

            if(collideTime>2)
                ScoreBoard scoreboard(app,font,score,coin_collected);

            Event ev;
            while(app.pollEvent(ev))
            {
                if(ev.type==Event::Closed)
                    app.close();

                if(ev.type==Event::KeyPressed)
                {
                    if(ev.key.code==Keyboard::Return)
                        if(LoadMenu(app,false))
                        {
                            posZ = 0;
                            posX = 0;
                            score = 0;
                            coin_collected = 0;
                            isAlive = true;
                        }

            }
        }}

        app.display();
    }
return 0;
}
