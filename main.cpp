#include "worklibgame.h"

int main()
{
    srand(time(NULL));  // получение первого случайного числа
    
    // Свойства окна Windows
    RenderWindow win(VideoMode(1280, 720), L"Путь домой");
    Image icon;
    if (!icon.loadFromFile("Image/icon.png")) return 13;  // Отсутствует иконка приложения
    win.setIcon(32,32, icon.getPixelsPtr());
    // Фоновая музыка
    bool mus = false; // Переключатель музыки
    Music game_music;
    if (!game_music.openFromFile("Music/mgame.ogg")) return 14; // Нет музыки
    game_music.setLoop(true);
    // Звуковые эффекты
    Sound falsound, soundbonM, soundbonB, soundvic;
    SoundBuffer fal, bufM, bufB, vic;
    fal.loadFromFile("Music/faled1.wav"); // Гибель корабля
    falsound.setBuffer(fal);
    bufM.loadFromFile("Music/bonusm.wav"); // Немного топлива
    soundbonM.setBuffer(bufM);
    bufB.loadFromFile("Music/bonusB.wav"); // Много топлива
    soundbonB.setBuffer(bufB);
    vic.loadFromFile("Music/victoria.wav"); // Победа
    soundvic.setBuffer(vic);
    // Игровая панель
    Texture TexturInfoPanel;
    TexturInfoPanel.loadFromFile("Image/panel.png");
    RectangleShape GameInfoPanel(Vector2f(1280,113));
    GameInfoPanel.setTexture(&TexturInfoPanel);
    GameInfoPanel.setPosition(Vector2f(0,0));
    // Радар
    CircleShape Rcircle(5.f);
    Rcircle.setFillColor(Color(255, 0, 0));       // Закрашиваем круг
    Rcircle.setOutlineThickness(2.f);             // Устанавливаем толщину контура круга
    Rcircle.setOutlineColor(Color(255, 155, 0));  // Устанавливаем цвет контура
    Rcircle.setPosition(495,33);
    // Топливо
    int pusk = 0;
    double tmpfull = 0;
    int full = 200;
    Text text_full, plusfull, text_pause, end_game;
    Font GameFont;
    if (!GameFont.loadFromFile("Font/BankGothic Md BT Medium.otf")) return 11; // Нет шрифта
    //Топливо космического корабля
    text_full.setFont(GameFont);
    text_full.setFillColor(Color::Green);
    text_full.setCharacterSize(30);
    text_full.setPosition(1070,20);
    //Топливо в канистрах
    plusfull.setFont(GameFont);
    plusfull.setFillColor(Color::Green);
    plusfull.setCharacterSize(25);
    // Конец игры
    end_game.setFont(GameFont);
    end_game.setFillColor(Color::Red);
    end_game.setCharacterSize(100);
    end_game.setString(L"Конец игры");
    end_game.setPosition(300,333);
    //Паузе
    text_pause.setFont(GameFont);
    text_pause.setFillColor(Color::Magenta);
    text_pause.setCharacterSize(50);
    text_pause.setString(L"П А У З А");
    text_pause.setPosition(500, 333);
    // Космос
    Texture textureSpace;
    textureSpace.loadFromFile("Image/newkos1.jpg");
    RectangleShape gamingBackground(Vector2f(1280,720));
    gamingBackground.setTexture(&textureSpace);
    RectangleShape gamingBackground2(Vector2f(1280, 720));
    gamingBackground2.setTexture(&textureSpace);
    gamingBackground2.setPosition(Vector2f(1280, 0));
   
    
    // Переменные
    bool GameOver = false, GamePause = false;                   // Конец игры 
    Vector2f pos;                                               // Координаты объектов
    Clock clock,clockAnimPlay,clockAnimMeteor, clockAnimText;   // Часы
    float time, timePlayer,timeGamingBackground,timeMeteor;      // Вещественные переменные времени

    
    // Космический корабль игрока
    Vector2f moveRec;
    int traffic=0;          // направление перемещения корабля
    FrameAnim FramePlAnim;
    FramePlAnim.Frame = 700;
    Texture texplayer;
    texplayer.loadFromFile("Image/playeranim1.png");
    Sprite player;
    player.setTexture(texplayer);
    player.setTextureRect(IntRect(0, FramePlAnim.Frame,90, 90));
    player.scale(0.7,0.7);
    player.setPosition(Vector2f(80, 380));
    // Земля
    Texture textEarth;
    if (!textEarth.loadFromFile("Image/earth_PNG39.png")) return 33; // Загрузка текстуры планеты земля
    RectangleShape Earth(Vector2f(500,500));
    Earth.setTexture(&textEarth);
    Earth.setPosition(Vector2f(1100,150));
    // Взрыв космического корабля
    FrameAnim DestructAnim;
    DestructAnim.Frame = 5; 
    DestructAnim.Frame1 = 15;
    Texture texdest;
    texdest.loadFromFile("Image/bum.png");
    Sprite destruction;
    destruction.setTexture(texdest);
    destruction.setTextureRect(IntRect(5, 15, 95, 80));
    destruction.scale(0.7, 0.7);
    
    // Метеориты

    Meteor meteor[15];
    const int Nmeteor = 15;

    // Топливо
    bool minusfull = false;
    Full canister("Image/galon.png",1000,1000);


    while (win.isOpen())  // Игровой цикл
    {
        Event event;
        // Привязка игрового процесса ко времени -------
        time = clock.getElapsedTime().asMicroseconds();
        timeGamingBackground = time / 6000;
        timeMeteor= time / 3000;
        timePlayer = time/2000;
        clock.restart();
        //----------------------------------------------
        
        while (win.pollEvent(event))  // Цикл игровых событий: нажатие клавишь, перемещение мышки и другие.
        {
            if (event.type == Event::Closed) win.close(); // Закрыть окно
                
            // Управление кораблём ---------------
            switch (event.type)
            {
            case Event::KeyPressed:
                //Управление игрой
                if (event.key.code == Keyboard::Pause) GamePause = !GamePause;
                if (event.key.code == Keyboard::End) win.close();
                if (event.key.code == Keyboard::M)
                {
                    mus = !mus; if (mus) game_music.play(); else game_music.stop();
                }
                // Управление космическим кораблём
                if ((event.key.code == Keyboard::Numpad5) || (event.key.code == Keyboard::S)) 
                    if (full != 0) { moveRec.y = 0.3 * timePlayer; traffic = 2; minusfull = true;}
                if ((event.key.code == Keyboard::Numpad8) || (event.key.code == Keyboard::W)) 
                    if (full != 0) { moveRec.y = -0.3 * timePlayer; traffic = 1; minusfull = true;  }
                if ((event.key.code == Keyboard::Numpad4) || (event.key.code == Keyboard::A)) 
                    if (full != 0) { moveRec.x = -0.3 * timePlayer; minusfull = true;         }
                if ((event.key.code == Keyboard::Numpad6) || (event.key.code == Keyboard::D)) 
                    if (full != 0) { moveRec.x = 0.3 * timePlayer; minusfull = true;         }
                break;
            case Event::KeyReleased:
                if ((event.key.code == Keyboard::Numpad5) || (event.key.code == Keyboard::S)) 
                { moveRec.y = 0; traffic = 0; minusfull = false; }
                if ((event.key.code == Keyboard::Numpad8) || (event.key.code == Keyboard::W)) 
                { moveRec.y = 0; traffic = 0; minusfull = false; }
                if ((event.key.code == Keyboard::Numpad4) || (event.key.code == Keyboard::A))
                {
                    moveRec.x = 0; minusfull = false;
                }
                if ((event.key.code == Keyboard::Numpad6) || (event.key.code == Keyboard::D))
                {
                    moveRec.x = 0; minusfull = false;
                }
                break;

            default:
                break;
            }
           //--------------------------------------
        }
        if (!GamePause) {
            // Радар
            if (Rcircle.getPosition().x <= 850) {
                if (GameOver)
                {
                    // Анимация взрыва
                    if (clockAnimMeteor.getElapsedTime() > milliseconds(80))
                    {
                        clockAnimMeteor.restart();
                        DestructAnim.Frame += DestructAnim.Step;
                        if (DestructAnim.Frame > 405) { DestructAnim.Frame1 += DestructAnim.Step1; DestructAnim.Frame = 5; }
                        if (DestructAnim.Frame1 > 415) {
                            GameOver = false;
                            player.setPosition(Vector2f(80, 380));
                            for (int i = 0; i < Nmeteor; i++) meteor[i].restart();
                            canister.restart();
                            Rcircle.setPosition(495,33);
                            pusk = 0;
                            DestructAnim.Frame = 5; DestructAnim.Frame1 = 15;
                            full = 200;
                        }
                        else { destruction.setTextureRect(IntRect(DestructAnim.Frame, DestructAnim.Frame1, 95, 80)); }
                    }
                }
                else {
                    // Анимация 
                    if (clockAnimPlay.getElapsedTime() > milliseconds(100))
                    {
                        clockAnimPlay.restart();
                        playeranim(player, FramePlAnim, traffic);

                    }

                    if (clockAnimMeteor.getElapsedTime() > milliseconds(80))
                    {
                        clockAnimMeteor.restart();
                        for (int i = 0; i < Nmeteor; i++) meteor[i].animation();
                        if (minusfull && full > 0) full--;
                        Rcircle.move(0.1, 0); // Движение радара
                    }

                    if (clockAnimText.getElapsedTime() > milliseconds(50))
                    {
                        clockAnimText.restart();
                        if (pusk > 0)
                        {
                            pusk--;
                            plusfull.move(0, -1);
                        }
                    }


                    // Космос
                    gamingBackground.move(-0.2 * timeGamingBackground, 0);
                    pos = gamingBackground.getPosition();
                    if (pos.x < -1280) gamingBackground.setPosition(1280, pos.y);
                    gamingBackground2.move(-0.2 * timeGamingBackground, 0);
                    pos = gamingBackground2.getPosition();
                    if (pos.x < -1280) gamingBackground2.setPosition(1280, pos.y);
                    //-----------------------------------------------------------------------

                    playermove(player, moveRec); // движение космического корабля


                    for (int i = 0; i < Nmeteor; i++) {
                        if (meteor[i].newborn) Correct(canister, i, meteor, Nmeteor);
                        meteor[i].move(timeMeteor);          // движение митеора
                        if (meteor[i].Collision(player.getGlobalBounds()))
                        {
                            falsound.play(); GameOver = true; destruction.setPosition(player.getPosition().x, player.getPosition().y); break;
                        }
                    }
                    // Топливо
                    if (canister.newborn) CorrectFull(canister, meteor, Nmeteor);
                    canister.move(timeMeteor); //Движение топливных элементов
                    // Заправка корабля
                    if (canister.Collision(player.getGlobalBounds()))
                    {
                        tmpfull = 10 + rand() % 90;
                        full += tmpfull;
                        if (tmpfull > 50) soundbonB.play(); else soundbonM.play();
                        plusfull.setString(IntToStr(tmpfull));
                        plusfull.setPosition(canister.getPosBonus().x, canister.getPosBonus().y);
                        pusk = 40;
                        canister.restart();
                    }
                    //Топливо на игровой панели
                    if (full >= 100) text_full.setFillColor(Color::Green);
                    if (full < 100 && full >= 50) text_full.setFillColor(Color::Yellow);
                    if (full < 50) text_full.setFillColor(Color::Red);
                    text_full.setString(IntToStr(full) + L" тонн");
                }
                // Область отрисовки объектовwsw
                win.clear();               // очистка области отрисовки
                // Отрисовка космоса------
                win.draw(gamingBackground2);
                win.draw(gamingBackground);
                //------------------------
                win.draw(GameInfoPanel);                                         // отрисовка игровой панели
                win.draw(Rcircle);
                if (GameOver) win.draw(destruction); else win.draw(player);      // отрисовка космического корабля
                for (int i = 0; i < Nmeteor; i++)  meteor[i].draw(win);          // отрисовка метеоров
                canister.draw(win);
                win.draw(text_full);
                if (pusk > 0) win.draw(plusfull);
                win.display();  // вывести всё на экран
                if (Rcircle.getPosition().x > 850) { game_music.stop(); soundvic.play(); }
                //------------------------------------------------------------------------
            } //Радар
            else // Радар
            {
                // Область отрисовки объектоввки завершения игры
                win.clear();               // очистка области отрисовки
                // Отрисовка космоса------
                win.draw(gamingBackground2);
                win.draw(gamingBackground);
                //------------------------
                win.draw(GameInfoPanel);  // отрисовка игровой панели
                win.draw(Rcircle);
                win.draw(text_full);
                win.draw(Earth);
                win.draw(end_game);
                win.display();            // вывести всё на экран
                if (clockAnimText.getElapsedTime() > seconds(20)) win.close();
            }
        }//Pause
        else  //Pause
{
win.draw(text_pause);
win.display();
}

}

    return 0;
}

