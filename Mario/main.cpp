#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>

//Декларация для получения класса.
#define DECLARE_GET_INSTANCE( ClassName ) \
    static ClassName* getInstance() {\
        static ClassName instance;\
        return &instance;\
    }

using namespace std;
using namespace sf;
unsigned int FontSize;
unsigned int pointsGame, timeGame; 
unsigned int lastPoints1, lastTime1, lastPoints2, lastTime2;
float offsetX, offsetY;
const int H = 17;
const int W = 150;
bool level = false;
bool restart = false;
void runGame();
std::ostringstream playerScore, playerTime;
bool menuT;
void startGame(String *TileMap);
RenderWindow win(VideoMode(600, 270), "Mario");

//Singleton
class Configurate
{
public: 
	static Configurate* getInstance()
	{
		static Configurate config;
		return &config;
	}
	void inFile()
	{
		int a;
		string s;
		init.open("config.ini", ios::in, ios::_Nocreate);
		if (init.good() == false)
		{
			cout << "Error at opening file config.ini!" << endl;
			return;
		}
		else 
		{
			while (!init.eof())
			{
				init >> s;
				if (s == "TimeGame1:")
				{
					init >> a;
					lastTime1 = a;
				}
				if (s == "PointsGame1:")
				{
					init >> a;
					lastPoints1 = a;
				}
				if (s == "TimeGame2:")
				{
					init >> a;
					lastTime2 = a;
				}
				if (s == "PointsGame2:")
				{
					init >> a;
					lastPoints2 = a;
				}
				if (s == "FontSize:")
				{
					init >> a;
					FontSize = a;
				}
			}
			init.close();
		}
	}
	void outFile()
	{
		init.open("config.ini", ios::out);
		init << "FontSize: " << FontSize << "\nTimeGame1: " << lastTime1 << "\nPointsGame1: " << lastPoints1<< "\nTimeGame2: " << lastTime2 << "\nPointsGame2: " << lastPoints2;
		init.close();
	}
private:
	fstream init;
	Configurate(){}
};

//State
class Menu
{
public:
	class ContinueState;
	class PauseState;
	class FinishState;
	
	explicit Menu() : m_state( PauseState::getInstance() )	{}

	class State
	{
	public:

		class NotSupported : public std::exception { };

		virtual ~State() {}

		virtual void sStart(Menu* menu) 
		{ menu->changeState(PauseState::getInstance());}

		virtual void sPause(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap) 
		{ menu->changeState(ContinueState::getInstance());}
		
		virtual void sMenu(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap) 
		{ menu->changeState(PauseState::getInstance());}
		
		virtual void sFinish(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text5, Text text6, Text text7, Text text8, bool mLife, bool mFinish, String *TileMap) 
		{ menu->changeState(FinishState::getInstance());}

	protected:
		State() {}
	};

	class ContinueState : public State
	{
	public:
		DECLARE_GET_INSTANCE( ContinueState )
		
		void sStart( Menu* menu)
		{throw NotSupported();}

		void sFinish(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text5, Text text6, Text text7, Text text8, bool mLife, bool mFinish, String *TileMap)
		{
			menu->changeState(FinishState::getInstance());
			menu->sFinish(text0, text1, text2, text3, text4, text5, text6, text7, text8, mLife, mFinish, TileMap);
		}

		void sMenu( Menu* menu) 
		{throw NotSupported();}

		void sPause(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap) 
		{
			menu->changeState(PauseState::getInstance());
			menu->sMenu(text0, text1, text2, text3, text4, text7, TileMap);
		}
	private:
		ContinueState() {}
	};

	class PauseState : public State
	{
	public:
		DECLARE_GET_INSTANCE( PauseState )

		void sStart( Menu* menu) 
		{runGame();}

		void sFinish(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text5, Text text6, Text text7, Text text8, bool mLife, bool mFinish, String *TileMap) 
		{  
			menu->changeState(FinishState::getInstance());
			menu->sFinish(text0, text1, text2, text3, text4, text5, text6, text7, text8, mLife, mFinish, TileMap);
		}

		void sPause(Menu* menu) 
		{throw NotSupported();}
		
		void sMenu(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap)
		{	
			std::ostringstream pG1, tG1, pG2, tG2;
			text0.setColor(Color::White);
			text1.setColor(Color::White);
			text2.setColor(Color::White);
			text3.setColor(Color::White);
			text4.setColor(Color::White);
			text7.setColor(Color::White);
			text0.setPosition(80,100);
			text1.setPosition(80,140);
			text2.setPosition(350,100);
			text3.setPosition(350,140);
			text4.setPosition(40,60);
			text7.setPosition(100,0);
			text0.setString(L"Играть");
			text1.setString(L"Перезапустить игру");
			text2.setString(L"Об игре");
			text3.setString(L"Выход");
			text4.setString(L"Главным героем игры является водопроводчик Марио.\nЦель игры – пройти через Грибное королевство и\nзаполучить ключ, ускользая или уничтожая солдат \nКороля Купы, чтобы открыть дверь замка.");
			pG1 << lastPoints1;
			tG1 << lastTime1;
			pG2 << lastPoints2;
			tG2 << lastTime2;
			text7.setString(L"Последний счет\n\t\t   1 Уровень \t Время: " + tG1.str() + L"\t  Очки: " + pG1.str() + L"\n\t\t   2 Уровень \t Время: " + tG2.str() + L"\t  Очки: " + pG2.str());
			bool sMenu=1;
			int MenuNum=0;
			Clock Time;

			while(sMenu)
			{	   
			   MenuNum=0;
			   text0.setColor(Color::White);
			   text1.setColor(Color::White);
			   text2.setColor(Color::White);
			   text3.setColor(Color::White);
			   text7.setColor(Color::White);
			   win.clear(Color(0,0,0));
			   
			   if (IntRect(80,100,150,30).contains(Mouse::getPosition(win))) {text0.setColor(Color::Red); MenuNum=1;}
			   if (IntRect(80,140,200,30).contains(Mouse::getPosition(win))) {text1.setColor(Color::Red); MenuNum=2;}
			   if (IntRect(350,100,150,30).contains(Mouse::getPosition(win))) {text2.setColor(Color::Red); MenuNum=3;}
			   if (IntRect(350,140,150,30).contains(Mouse::getPosition(win))) {text3.setColor(Color::Red); MenuNum=4;}

			   if (Mouse::isButtonPressed(Mouse::Left))
			   {
				   if (MenuNum==1) {sMenu=false; menu->changeState( ContinueState::getInstance() );}
				   if (MenuNum==2) {sMenu=false; level = false; restart = true; runGame();}
				   if (MenuNum==3) {win.draw(text4); win.display(); while(!Keyboard::isKeyPressed(Keyboard::Escape)) ;}
				   if (MenuNum==4) {win.close(); sMenu=false; exit(0);}
			   }
			   win.draw(text0);
			   win.draw(text1);
			   win.draw(text2);
			   win.draw(text3);
			   win.draw(text7);
			   win.display();
			}
		}

	private:
		PauseState() {}
	};

	class FinishState : public State
	{
	public:
		DECLARE_GET_INSTANCE( FinishState )
		bool fFinish;

		void sStart( Menu* menu)
		{throw NotSupported();}
		
		void sFinish(Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text5, Text text6, Text text7, Text text8, bool mLife, bool mFinish, String *TileMap)
		{
			int FinishNum=0;
			
			while(!mLife)
			{	  
				text5.setColor(Color::White);
				text5.setPosition(245,10);
				text5.setString(L"Конец игры\n");
			
				text6.setColor(Color::White);
				text6.setPosition(236,80);
				text6.setString(L"Выйти в меню");
			    FinishNum=0;
			    text5.setColor(Color::White);
			    text6.setColor(Color::White);
					
				text8.setColor(Color::White);
				text8.setPosition(200,50);
				text8.setString(L"Перезапустить уровень");
				FinishNum=0;
				text5.setColor(Color::White);
				text6.setColor(Color::White);
				text8.setColor(Color::White);


			    if (IntRect(236,80,300,30).contains(Mouse::getPosition(win))) {text6.setColor(Color::Red); FinishNum=1;}
				if (IntRect(200,50,300,30).contains(Mouse::getPosition(win))) {text8.setColor(Color::Red); FinishNum=2;}
			    if (Mouse::isButtonPressed(Mouse::Left))
				{
				    if (FinishNum==1) 
				    {
						level = false;
				 	    runGame(); 
				 	    menu->changeState( PauseState::getInstance() );
					    menu->sMenu(text0, text1, text2, text3, text4, text7, TileMap);
				    }
					if (FinishNum==2) 
				    {	
						restart = true;
						runGame(); 
				    }
				}
			    win.draw(text5);
			    win.draw(text6);
				win.draw(text8);
			    win.display();
			}
			if (!level)
			{
				while(mFinish)
				{	 
					Configurate::getInstance()->outFile();
					text5.setColor(Color::White);
					text5.setPosition(210,10);
					text5.setString(L"Уровень пройден!\n");
			
					text6.setColor(Color::White);
					text6.setPosition(200,50);
					text6.setString(L"Следующий уровень");
					
					FinishNum=0;
					text5.setColor(Color::White);
					text6.setColor(Color::White);
					
					if (IntRect(200,50,300,30).contains(Mouse::getPosition(win))) {text6.setColor(Color::Red); FinishNum=1;}
					if (Mouse::isButtonPressed(Mouse::Left))
					{
						if (FinishNum==1) 
						{
							lastPoints1 = pointsGame;
							lastTime1 = timeGame;
							level = true;
				 			runGame(); 
						}
					}
					win.draw(text5);
					win.draw(text6);
					win.display();
				}
			}
			if (level)
			{
				while(mFinish)
				{
					level = false;
					lastPoints2 = pointsGame;
					lastTime2 = timeGame;
					Configurate::getInstance()->outFile();

					text5.setColor(Color::White);
					text5.setPosition(220,10);
					text5.setString(L"Игра завершена!\n");
			
					text6.setColor(Color::White);
					text6.setPosition(236,50);
					text6.setString(L"Выйти в меню");
					FinishNum=0;
					text5.setColor(Color::White);
					text6.setColor(Color::White);

					if (IntRect(236,50,300,30).contains(Mouse::getPosition(win))) {text6.setColor(Color::Red); FinishNum=1;}
					if (Mouse::isButtonPressed(Mouse::Left))
						if (FinishNum==1) 
						{
				 			runGame(); 
				 			menu->changeState( PauseState::getInstance() );
							menu->sMenu(text0, text1, text2, text3, text4, text7, TileMap);
						}
					win.draw(text5);
					win.draw(text6);
					win.display();
				}
			}
		}

		void sPause(Menu* menu) 
		{throw NotSupported();}

		void sMenu( Menu* menu, Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap) 
		{	
			menu->changeState(PauseState::getInstance());
			menu->sMenu(text0, text1, text2, text3, text4, text7, TileMap);
		}
		
	private:
		FinishState() { fFinish=true;}
	};

	void sMenu(Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap){
		m_state->sMenu( this, text0, text1, text2, text3, text4, text7, TileMap);
	}
	void sPause(Text text0, Text text1, Text text2, Text text3, Text text4, Text text7, String *TileMap)
	{
		m_state->sPause( this, text0, text1, text2, text3, text4, text7, TileMap);
	}
	void sFinish(Text text0, Text text1, Text text2, Text text3, Text text4, Text text5, Text text6, Text text7, Text text8, bool mLife, bool mFinish, String *TileMap)
	{
		m_state->sFinish( this, text0, text1, text2, text3, text4, text5, text6, text7, text8, mLife, mFinish, TileMap);			
	}
private:
	State* m_state;
	void changeState( State* newState ) {
		m_state = newState;
    }
};

//Adapter
class Player
{
public:
	bool onGround; //На земле ли мы
	float currentFrame; //Текущий кадр для анимации
	float dx,dy; //Скорость
	FloatRect rect; //Вместо ширины\высоты
	Sprite sprite; //Картинка
	bool life;
	bool exit;
	bool finish;

    void Update(float time, String *TileMap)
    {	
		finish = false;
		rect.left += dx * time;	
		Collision(0, TileMap);
   
		if (!onGround) dy=dy+0.0005*time; //Если не на земле, то падаем с ускорение 0.0005
		rect.top += dy*time; //Движение персонажа
		onGround=false;
		Collision(1, TileMap);
	
		currentFrame += time * 0.005;
		if (currentFrame > 3) currentFrame -= 3;
	 
		if (dx>0) sprite.setTextureRect(IntRect(112+31*int(currentFrame),144,16,16));
		if (dx<0) sprite.setTextureRect(IntRect(112+31*int(currentFrame)+16,144,-16,16));
	 
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
		dx=0;
	}
	
	//Столкновение (num = 1 по горизонтали \ num = 0 по вертикали)
	void Collision(int num, String *TileMap)
	{
	   //Прходимся по плиткам
	   for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++) //Топ на толщину клетки. Топ плюс толщина героя на клетку
		   for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++)
		   {
			   //Если блок
			   if ((TileMap[i][j]=='P')||(TileMap[i][j]=='a')||(TileMap[i][j]=='b') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='t'))
			   { 
				   //Если плитка, то перемещаемся
				   if (dy>0 && num==1)
				   { rect.top =   i*16 -  rect.height;  dy=0;   onGround=true; }
				   if (dy<0 && num==1)
				   { rect.top = i*16 + 16;   dy=0;}
				   if (dx>0 && num==0)
				   { rect.left =  j*16 -  rect.width; }
				   if (dx<0 && num==0)
				   { rect.left =  j*16 +16;}
			   }
			   
			   if (TileMap[i][j]=='c') 
			   {
				   TileMap[i][j]=' ';
				   pointsGame++;
			   }
			   if (TileMap[i][j]=='Y') 
			   {
				   TileMap[i][j]=' ';
				   exit = true;
			   }
			   if (TileMap[i][j]=='a') 
			   {
				   life = false;
			   }
			   if(exit && TileMap[i][j]=='x')
			   {
				   finish = true;
			   }
		   }
   }
};
class AdapterPlayer
{
public:
	virtual void UpdatePlayer(float time, String *TileMap)=0;
	virtual void CollisionPlayer(int num, String *TileMap)=0;

	virtual bool getOnGround()=0;
	virtual void setOnGround(bool OnGround)=0;
	
	virtual bool getFinish()=0;

	virtual bool getLife()=0;
	virtual void setLife(bool Life)=0;

	virtual bool getExit()=0;
	virtual void setExit(bool Exit)=0;

	virtual Sprite getSprite()=0;
	virtual	void setSprite(Color color)=0;
	
	virtual FloatRect getRect()=0;

	virtual float getDx()=0;
	virtual void setDx(float Dx)=0;

	virtual float getDy()=0;
	virtual void setDy(float Dy)=0;
};
class Adapter : AdapterPlayer
{
private:
	Player player;
public:
	Adapter(Texture &image) //Конструктор
	{
		player.life = true;
		player.sprite.setTexture(image);
		player.rect = FloatRect(100,180,16,16);
		player.dx=player.dy=0.1;
		player.currentFrame = 0;			   
    }
	void UpdatePlayer(float time, String *TileMap)
	{
		player.Update(time, TileMap);
	}
	void CollisionPlayer(int num, String *TileMap)
	{
		player.Collision(num, TileMap);
	}
	
	bool getFinish() {return player.finish;}

	bool getExit() {return player.exit;}
	void setExit(bool Exit) {player.exit = Exit;};

	bool getOnGround() {return player.onGround;}
	void setOnGround(bool OnGround) {player.onGround = OnGround;}
	
	bool getLife() {return player.life;}
	void setLife(bool Life) {player.life = Life;}

	Sprite getSprite() {return player.sprite;}
	void setSprite(Color color) {player.sprite.setColor(color);}

	FloatRect getRect() {return player.rect;}

	float getDx() {return player.dx;}
	void setDx(float Dx) {player.dx = Dx;}

	float getDy() {return player.dy;}
	void setDy(float Dy) {player.dy = Dy;}
};

class Enemy
{
public:
	float dx,dy; //Скорость
	FloatRect rect;
	bool onGround;
	Sprite sprite;
	float currentFrame; //Текущий кадр для анимации

	virtual	void Set(Texture &image, int x, int y) = 0;

	virtual void Update(float time, bool pLife, String *TileMap) = 0;
   
	virtual void Collision(String *TileMap) = 0;
};
class EnemyGo : public Enemy
{
public:
	bool life;

	void Set(Texture &image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x,y,16,16);

		dx=0.05;
		currentFrame = 0;
		life=true;
   }

	void Update(float time, bool pLife, String *TileMap)
	{ 
		if(pLife) rect.left += dx * time;
		
		Collision(TileMap);
		
		//Движение по горизонтали влево\вправо
		currentFrame += time * 0.005;
		if (currentFrame > 2) currentFrame -= 2;
		
		sprite.setTextureRect(IntRect(18*int(currentFrame), 0, 16,16));

		if (!life) sprite.setTextureRect(IntRect(58, 0, 16,16));
			sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}
   
	void Collision(String *TileMap)
   {
	   for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++)
		   for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++)
			   if ((TileMap[i][j]=='P')|| (TileMap[i][j]=='b') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='t'))
			   { 
				   if (dx>0)
				   { rect.left =  j*16 - rect.width; dx*=-1; }
				   else if (dx<0)
				   { rect.left =  j*16 + 16;  dx*=-1; }
			   }
   }
};
class EnemyJump : public Enemy
{
public:
	FloatRect rectJ;
	bool active;
	bool intersect;

	void Set(Texture &image, int x, int y)
	{
		sprite.setTexture(image);
		rect = FloatRect(x,y,16,16);
		rectJ = FloatRect(x-32,y-64,80,96);
		onGround = true;
		dy=0.05;
		currentFrame = 0;
		active=false;
	}

	void Update(float time, bool pLife, String *TileMap)
	{ 
		if (intersect)
			if(active)
			{
				if(rect.top >= 128 && onGround) 
				{
					rect.top += -0.70; 
				}
				if(rect.top < 128)
					onGround = false;
				if(!onGround) 
				{
					rect.top += +0.70; 
				}
				if(rect.top == 208)
					onGround = true;
				Collision(TileMap);
				sprite.setTextureRect( IntRect(65,128,16,16) );
			}
		if (!active) 
		{
			if(rect.top != 208)
					onGround = false;
			if(!onGround) 
			{
				rect.top += +0.70; 
			}
			if(rect.top == 208)
					onGround = true;
			sprite.setTextureRect( IntRect(80,128,16,16) );

			Collision(TileMap);
				
		}
		sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	}
   
	void Collision(String *TileMap)
	{
	   for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++)
		   for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++)
			   if ((TileMap[i][j]=='P')|| (TileMap[i][j]=='b') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='t'))
			   { 
				   if (dy>0) rect.top = i*16 - rect.height;
				   else if (dy<0) rect.top = i*16 + 16;
			   }
   }
};

void startGame(String *TileMap)
{
	if(!level)
		Configurate::getInstance()->inFile();
	Font font;
	font.loadFromFile("arial.ttf");
	Text text("", font, 20);
	text.setColor(Color::Red);
	text.setStyle(sf::Text::Bold);
	Texture tileSet;
	tileSet.loadFromFile("Tileset.png");
	Adapter Mario(tileSet);
	EnemyGo enemy[10]; 
	EnemyJump enemyJump[10]; 
	pointsGame = 0;
	int timeG1 = 0, timeG2 = 0, timeG3 = 0;
	offsetX=0, offsetY=0;
	Mario.setExit(false);
	Sprite tile(tileSet);

	Menu menu;
	Text text0("", font, FontSize);
	Text text1("", font, FontSize);
	Text text2("", font, FontSize);
	Text text3("", font, FontSize);
	Text text4("", font, FontSize);
	Text text5("", font, 22);
	Text text6("", font, FontSize);
	Text text7("", font, 18);
	Text text8("", font, 18);

	if(!restart)
		if(!level)
			menu.sMenu(text0, text1, text2, text3, text4, text7, TileMap);
	if(restart) restart = false;

	Clock clock;
	Clock a;
	Clock b;
	Sprite exitS;
	FloatRect exitFR;
	bool mT = true;

	if(!level)
	{
		enemy[0].Set(tileSet,21*16,13*16); 
		enemy[1].Set(tileSet,36*16,13*16); 
		enemy[2].Set(tileSet,48*16,13*16); 
		enemy[3].Set(tileSet,56*16,13*16); 
		enemy[4].Set(tileSet,21*16,6*16); 
		enemy[5].Set(tileSet,28*16,6*16); 
		enemy[6].Set(tileSet,60*16,6*16); 
		enemy[7].Set(tileSet,64*16,6*16); 
		enemy[8].Set(tileSet,110*16,6*16); 
		enemy[9].Set(tileSet,120*16,6*16); 
		enemyJump[0].Set(tileSet,18*16,13*16);
		enemyJump[1].Set(tileSet,25*16,13*16);
		enemyJump[2].Set(tileSet,32*16,13*16);
		enemyJump[3].Set(tileSet,50*16,13*16);
		enemyJump[4].Set(tileSet,59*16,13*16);
		enemyJump[5].Set(tileSet,66*16,13*16);
		enemyJump[6].Set(tileSet,88*16,13*16);
		enemyJump[7].Set(tileSet,95*16,13*16);
		enemyJump[8].Set(tileSet,108*16,13*16);
		enemyJump[9].Set(tileSet,122*16,13*16);
	}
	else if (level)
	{
		enemy[0].Set(tileSet,20*16,6*16); 
		enemy[1].Set(tileSet,30*16,6*16); 
		enemy[2].Set(tileSet,40*16,6*16); 
		enemy[3].Set(tileSet,48*16,13*16); 
		enemy[4].Set(tileSet,90*16,6*16); 
		enemy[5].Set(tileSet,74*16,8*16); 
		enemy[6].Set(tileSet,60*16,6*16); 
		enemy[7].Set(tileSet,108*16,6*16); 
		enemy[8].Set(tileSet,100*16,13*16); 
		enemy[9].Set(tileSet,120*16,13*16);
		enemyJump[0].Set(tileSet,20*16,13*16);
		enemyJump[1].Set(tileSet,27*16,13*16);
		enemyJump[2].Set(tileSet,34*16,13*16);
		enemyJump[3].Set(tileSet,41*16,13*16);
		enemyJump[4].Set(tileSet,48*16,13*16);
		enemyJump[5].Set(tileSet,97*16,13*16);
		enemyJump[6].Set(tileSet,106*16,13*16);
		enemyJump[7].Set(tileSet,113*16,13*16);
		enemyJump[8].Set(tileSet,120*16,13*16);
		enemyJump[9].Set(tileSet,128*16,13*16);
	}

	while (win.isOpen())
    { 	
		if(menuT) timeG3 += timeG2;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		if(Mario.getLife()) 
		{
			timeG1 = a.getElapsedTime().asSeconds();
			timeGame = timeG1 - timeG3;
			menuT = false;
		}

		time = time/1000; 

		if (time > 20) time = 20; 

        Event event;
        while (win.pollEvent(event))
        {
            if (event.type == Event::Closed)      
                win.close();
		}
		if(Mario.getLife())
		{
			if (Keyboard::isKeyPressed(Keyboard::Left))
				Mario.setDx(-0.1);
       
			if (Keyboard::isKeyPressed(Keyboard::Right))	
				Mario.setDx(0.1);
    
			if (Keyboard::isKeyPressed(Keyboard::Up)) 

			if (Mario.getOnGround()) 
			{
				Mario.setDy(-0.27); Mario.setOnGround(false); 
			}
		}

		Mario.UpdatePlayer(time, TileMap);
			
		for (int i = 0; i < 10; i++)
			enemy[i].Update(time, Mario.getLife(), TileMap);
		for (int i = 0; i < 10; i++)
			enemyJump[i].Update(time, Mario.getLife(), TileMap);
																							
		 for (int i = 0; i < 10; i++)
		 {
			 if  (Mario.getRect().intersects( enemy[i].rect ))
			 {
				 if (enemy[i].life) 
				 {
					 if (Mario.getDy()>0) { enemy[i].dx=0; Mario.setDy(-0.2); enemy[i].life=false;}
					 else 
					 { 
						Mario.setLife(false);  
					 }
				 }
			 }
		 }
		 for (int i = 0; i < 10; i++)
		 {
			 if  (Mario.getRect().intersects( enemyJump[i].rectJ ))
			 {
				 if  (Mario.getRect().intersects( enemyJump[i].rect ))
				 {
					 if (enemyJump[i].active) 
					 {
						 Mario.setLife(false);  
					 }
				 }
				enemyJump[i].intersect = true; 	enemyJump[i].active = true; 
			 }
			 else {enemyJump[i].intersect = false; enemyJump[i].active = false;}
		 }

		 if (Mario.getRect().left>200) offsetX = Mario.getRect().left-200;

		 if(!level) win.clear(Color(60,80,255));
		 if(level) win.clear(Color(20,0,50));

		 for (int i=0; i<H; i++)
			 for (int j=0; j<W; j++)
			  { 
				//Ключ
				if (TileMap[i][j]=='Y')  tile.setTextureRect( IntRect(80,112,16,16) );
				//Ломанный блок
				if (TileMap[i][j]=='P')  tile.setTextureRect( IntRect(143-16*3,112,16,16) );
				//Кирпичный блок
				if (TileMap[i][j]=='k')  tile.setTextureRect( IntRect(143,112,16,16) );
				//Алмаз                                 
   			    if (TileMap[i][j]=='c')  tile.setTextureRect( IntRect(128,112,16,16) );
				//Труба
				if (TileMap[i][j]=='t')  tile.setTextureRect( IntRect(0,47,32,95-47) );
				//Холм
				if (TileMap[i][j]=='g')  tile.setTextureRect( IntRect(0,16*9-5,3*16,16*2+5) );
				//Большой холм
				if (TileMap[i][j]=='G')  tile.setTextureRect( IntRect(145,222,222-145,255-222) );
				//Трава
			    if (TileMap[i][j]=='d')  tile.setTextureRect( IntRect(0,106,74,127-106) );
				//Облако
				if (TileMap[i][j]=='w')  tile.setTextureRect( IntRect(99,224,140-99,255-224) );
				//Коричневый блок
				if (TileMap[i][j]=='r')  tile.setTextureRect( IntRect(143-32,112,16,16) );
				//Голубой блок
				if (TileMap[i][j]=='b')  tile.setTextureRect( IntRect(111,128,16,16) );
				//Замок
				if (TileMap[i][j]=='Z')  tile.setTextureRect( IntRect(95,0,110,100) );
				//Лава
				if (TileMap[i][j]=='a')  tile.setTextureRect( IntRect(239,192,16,16) );
				//Земля
				if (TileMap[i][j]=='s')  tile.setTextureRect( IntRect(223,192,16,16) );

				if ((TileMap[i][j]==' ') || (TileMap[i][j]=='0') || (TileMap[i][j]=='x')) continue;

  			    tile.setPosition(j*16-offsetX,i*16 - offsetY) ; 
				win.draw(tile);
		     }

		std::ostringstream playerScore, playerTime;
		playerScore << pointsGame;
		playerTime << timeGame;
		text.setString(L"Время: " + playerTime.str() + L"\nОчки: " + playerScore.str());
		text.setPosition(16, 0);

		for (int i = 0; i < 10; i++)
			win.draw(enemy[i].sprite);
		for (int i = 0; i < 10; i++)
			win.draw(enemyJump[i].sprite);

		win.draw(text);
		win.draw(Mario.getSprite());

		win.display();
		if(!Mario.getLife()) 
		{
			menu.sFinish(text0, text1, text2, text3, text4, text5, text6, text7, text8, Mario.getLife(), Mario.getFinish(), TileMap); 
			startGame(TileMap);
		}
		if(Mario.getFinish()) 
		{
			menu.sFinish(text0, text1, text2, text3, text4, text5, text6, text7, text8, Mario.getLife(), Mario.getFinish(), TileMap); 
			startGame(TileMap);
		}
		if(Keyboard::isKeyPressed(Keyboard::Escape)) 
		{
			menuT = true;
			b.restart();
			b.getElapsedTime().asSeconds();
			menu.sPause(text0, text1, text2, text3, text4, text7, TileMap);
			timeG2 = b.getElapsedTime().asSeconds();
		}
    }
}

void runGame()
{
	String 	TileMap1[H] = {
"b000000000000000000000000b000000000000000000000000000000000000000000000000000000000000000000000000000000b000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"b                        b                                                                              b               bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"b                       cb                         w                  w                           w     b          c   Ybbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"b     w              bbbbb                  w                                                           b        bbbbbbbbbbbbbbbbbaabbbbbbbbbbbbbbbbbb",
"b                        b                                                                                              bbbbbbbbbaaabbbbbbbbbbbbbbbbbb",
"b                        b                                                               bbbbbb                         bbbbbbbbbbaabbbbbbbbbbbbbbbbbb",
"b               r       cbc        r                    bc        cb                                    b    c         cbbbbbbbbbbaabbbbbbbbbbbbbbbbbb",
"b              bbbbbbbbbbbbbbbbbbbbbb                   bbbbbbbbbbbb            bbbb              bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbaabbbbbbbbbbbbbbbbbb",
"br                                                                         r                                     Z            bbbbaabbbbbbbbbbbbbbbbbb",
"brr                  c      c                 bbbb            c           rr                              c                   bbbbaabbbbbbbbbbbbbbbbbb",
"brrr                                          b       r                  rrr                                                  bbbbaabbbbbbbbbbbbbbbbbb",
"brrrr       t0                         t0     b       rr                rrrr       t0                                         bbbaaaabbbbbbbbbbbbbbbbb",
"brrrrr      00                         00     b      rrr               rrrrr       00              r                          bbbbbbbbbbbbbbbbbbbbbbbb",
"brrrrrr     00                         00     bc     rrr              rrrrrr       00       c     rr                x        cbbbbbbbbbbbbbbbbbbbbbbbb",
"PPPPPPP    PPPPPPPPPPPPPPPPPPPPPPPPPPPPPP     PPPPPPPPPPPPPPPPPPPPPPPPPPPPPP      PPPPPPPPPPPPPPPPPP     PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPaaaaaaPPPPPPPPPPPPPPPPPPaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPaaaaaaPPPPPPPPPPPPPPPPPPaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
}; 
	String 	TileMap2[H] = {
"b00000000000000b00000000000000000000000000000000000000000000000000000000000000000000000000000000000000bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
"b              b                                                                                                     Z        bbbbbbbbbbbbbbbbbbbbbbbb",
"b              b                                                                                                              bbbbbbbbbbbbbbbbbbbbbbbb",
"b              b                                                                                              c               bbbbaaaaaabbbbbbbbbbbbbb",
"b              b                                                                                                              bbbbbbbbbabbbbbbbbbbbbbb",
"b              b                                                                                                              bbbbbbbbbabbbbbbbbbbbbbb",
"b Z            bc         c         c            b        b   c  b                   b  c   b         b                 x    cbbbbbbbbbabbbbbbbbbbbbbb",
"b              bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb        bbbbbbbb                   bbbbbbbb         bbbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaaabbbbbbbbbbbbbb",
"b                                                                       b  c   b            b         b                       bbbbabbbbbbbbbbbbbbbbbbb",
"b                                                                       bbbbbbbb            b       bbb      c      c         bbbbabbbbbbbbbbbbbbbbbbb",
"b                      c       c     c       c        r                                     b                                 bbbbabbbbbbbbbbbbbbbbbbb",
"b                                                    rr                                     bbb                               bbbbaaaaaabbbbbbbbbbbbbb",
"bbbbbbbbb      b                                    rrr                                     b                                 bbbbbbbbbbbbbbbbbbbbbbbb",
"bbbbbbbbb      bb                                  rrrr                                     bc                               Ybbbbbbbbbbbbbbbbbbbbbbbb",
"PPPPPPPPP      PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP                                     PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPaaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPaaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
}; 
	if(!level)
		startGame(TileMap1);
	else if(level)
		startGame(TileMap2);
}

int main()
{   
	runGame();
    return 0;
}