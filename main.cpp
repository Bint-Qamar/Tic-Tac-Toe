#include<SDL2/SDL.h>
#include<SDL_ttf.h>
#include<iostream>
#include<vector>

using namespace std;

SDL_Renderer* renderer;
  
struct structure
{
   int x, y;
};

void check_position(structure&);
bool check_victory(structure&);
void get_position(int &x, int &y)
  {
        if (x >= 248 && x <= 348)
        {
            x = 248;
        }
        else
        {
            if (x >= 350 && x <= 450)
                x = 350;

            else 
            {
                if (x >= 452 && x <= 550)
                    x = 452;
                else
                    x = 0;
            }
        }

        if (y >= 125 && y <= 225)
        {
             y = 125;
        }
        else
        {
            if (y >= 227 && y <= 327)
                y = 227;

            else
            {
                if (y >= 329 && y <= 425)
                    y = 329;
                else
                    y = 0;
            }
        }
  }


class PlayerOne 
{
	SDL_Texture* cross;
    vector<structure> pos;

    friend void check_position(structure&);
    friend bool check_victory(structure&);

public:
    int DrawCross()
	{
        structure position ;
        cross = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 100, 100);
        SDL_GetMouseState(&position.x, &position.y);
  
        get_position(position.x, position.y);
        check_position(position);
        
        if (position.x && position.y)
        {
            SDL_Rect src{ 0,0,100,100 };
            SDL_Rect dst{ position.x,position.y,100,100 };


            SDL_SetRenderTarget(renderer, cross); 
            SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, 10, 10, 90, 90);
            SDL_RenderDrawLine(renderer, 90, 10, 10, 90);

            SDL_SetRenderTarget(renderer, nullptr);

            SDL_RenderCopy(renderer, cross, &src, &dst);
            SDL_RenderPresent(renderer);

            pos.push_back(position);

            if (check_victory(position))
                return 2;

            return 1;
        }
        else
            return 0;
	}
};

class PlayerTwo
{
    SDL_Texture* circle;
    vector<structure> pos;

    friend void check_position(structure&);
    friend bool check_victory(structure&);

public:
    int DrawCircle()
    {
        int32_t centreX = 50, centreY = 50, radius = 40;
        structure position;

        circle = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 100, 100);
        SDL_GetMouseState(&position.x, &position.y);

        get_position(position.x, position.y);
        check_position(position);

        if (position.x && position.y)
        {
            SDL_Rect src{ 0,0,100,100 };
            SDL_Rect dst{ position.x,position.y,100,100 };

            const int32_t diameter = (radius * 2);

            int32_t X = (radius - 1);
            int32_t Y = 0;
            int32_t tx = 1;
            int32_t ty = 1;
            int32_t error = (tx - diameter);


            SDL_SetRenderTarget(renderer, circle);
            SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            while (X >= Y)
            {
                // Each of the following renders an octant of the circle
                SDL_RenderDrawPoint(renderer, centreX + X, centreY - Y);
                SDL_RenderDrawPoint(renderer, centreX + X, centreY + Y);
                SDL_RenderDrawPoint(renderer, centreX - X, centreY - Y);
                SDL_RenderDrawPoint(renderer, centreX - X, centreY + Y);
                SDL_RenderDrawPoint(renderer, centreX + Y, centreY - X);
                SDL_RenderDrawPoint(renderer, centreX + Y, centreY + X);
                SDL_RenderDrawPoint(renderer, centreX - Y, centreY - X);
                SDL_RenderDrawPoint(renderer, centreX - Y, centreY + X);

                if (error <= 0)
                {
                    ++Y;
                    error += ty;
                    ty += 2;
                }

                if (error > 0)
                {
                    --X;
                    tx += 2;
                    error += (tx - diameter);
                }
            }

            SDL_SetRenderTarget(renderer, nullptr);

            SDL_RenderCopy(renderer, circle, &src, &dst);
            SDL_RenderPresent(renderer);

            pos.push_back(position);

            if (check_victory(position))
               return 2;

            return 1;
        }
        else
            return 0;
    }
};

PlayerOne player1{};
PlayerTwo player2{};


void check_position(structure &strc)
{
    structure st;
    for (int i = 0; i < player1.pos.size(); i++)
    {
        st = player1.pos[i];
        if (strc.x == st.x && strc.y == st.y)
        {
            strc.x = strc.y = 0;
            break;
        }
    }
    for (int i = 0; i < player2.pos.size(); i++)
    {
        st = player2.pos[i];
        if (strc.x == st.x && strc.y == st.y)
        {
            strc.x = strc.y = 0;
            break;
        }
    }
}

bool check_victory(structure& turn)
{
    structure play;


    for (int i = 0, countX = 0, countY = 0; i < player1.pos.size(); i++)
    {
        play = player1.pos[i];
        if (turn.x == play.x)
        {
            countX++;
        }
        if (turn.y == play.y)
        {
            countY++;
        }
        if (countX == 3 || countY == 3)
        {
            return true;
        }
    }

    for (int i = 0, countX = 0, countY = 0; i < player2.pos.size(); i++)
    {
        play = player2.pos[i];
        if (turn.x == play.x)
        {
            countX++;
        }
        if (turn.y == play.y)
        {
            countY++;
        }
        if (countX == 3 || countY == 3)
        {
            return true;
        }
    }

    for (int i = 0; i < player1.pos.size(); i++)
    {
        play = player1.pos[i];
        if (play.x == 248 && play.y == 125)
        {
            for (int i = 0,count=1; i < player1.pos.size(); i++)
            {
                play = player1.pos[i];
                if (play.x == 350 && play.y == 227 || play.x == 452 && play.y == 329)
                {
                    count++;
                }
                if (count == 3)
                    return true;
            }
        }
        if (play.x == 452 && play.y == 125)
        {
            for (int i = 0, count = 1; i < player1.pos.size(); i++)
            {
                play = player1.pos[i];
                if (play.x == 350 && play.y == 227|| play.x == 248 && play.y ==329 )
                {
                    count++;
                }

                if (count == 3)
                    return true;
            }
        }
    }
    for (int i = 0; i < player2.pos.size(); i++)
    {
        play = player2.pos[i];
        if (play.x == 248 && play.y == 125)
        {
            for (int i = 0, count = 0; i < player2.pos.size(); i++)
            {
                play = player2.pos[i];
                if (play.x == 350 && play.y == 227 || play.x == 452 && play.y == 329)
                {
                    count++;
                }
                if (count == 2)
                    return true;
            }
        }
        if (play.x == 452 && play.y == 125)
        {
            for (int i = 0, count = 0; i < player2.pos.size(); i++)
            {
                play = player2.pos[i];
                if (play.x == 350 && play.y == 227 || play.x == 248 && play.y == 329)
                {
                    count++;
                }

                if (count == 2)
                    return true;
            }
        }
    }

        return false;
    
}

class CreateWindow
{
    SDL_Window* window;
    SDL_Surface* surface;
    SDL_Texture* texture;

  
public:

    CreateWindow()
    {

        this->window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
        if (this->window == NULL)
        {
            cout << "Can't open the window";
            exit(1);
        }


        renderer = SDL_CreateRenderer(this->window, -1, 0);


        SDL_SetRenderDrawColor(renderer, 0, 0, 40, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
         
        //For vertical lines          x1    y1   x2   y2
        this->SDL_RenderDrawThickLine(348, 125, 348, 425, 2, "vertical");
        this->SDL_RenderDrawThickLine(450, 125, 450, 425, 2, "vertical");
        //For horizontal lines        x1   y1   x2    y2
        this->SDL_RenderDrawThickLine(248, 225, 550, 225, 2, "horizontal");
        this->SDL_RenderDrawThickLine(248, 327, 550, 327, 2, "horizontal");
          

        SDL_RenderPresent(renderer);  


        SDL_Event player1event,player2event;
 
        bool gameloop = true;
        int i=0, game = 0;

        while (gameloop)
        {
            i++;
            while (!SDL_PollEvent(&player1event) || game == 0)
            {
                if (player1event.type == SDL_QUIT || player1event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    gameloop = false;
                    break;
                }
                else if (player1event.type == SDL_MOUSEBUTTONDOWN)
                {
                    game = player1.DrawCross();
                }
            }

            if (game == 2 )
            {
                cout << "X won" << endl;
                break;
            }
            if (i == 5)
            {
                cout << "Draw" << endl;
                break;
            }
            game=0;

            while(!SDL_PollEvent(&player2event) || game==0)
            {

                if(player2event.type == SDL_QUIT || player2event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    gameloop=false;
                    break;
                }
                else if(player2event.type == SDL_MOUSEBUTTONDOWN)
                {
                    game = player2.DrawCircle();
                }
            }
            if (game == 2) 
            {
                cout << "O won"<<endl;
                break;
            }
            game = 0;
        }
        SDL_Delay(5000);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();

    } 

    void SDL_RenderDrawThickLine(int x1, int y1, int x2, int y2, int width, string str)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        if (str == "vertical")
        {
            for (int i = 1; i <= width; i++, x1++, x2++)
            {
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
        else
        {
            for (int i = 1; i <= width; i++, y1++, y2++)
            {
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }
    }

};

/*,pos[9][2] = {
            {248,125},//0
            {350,125},//1
            {452,125},//2
            {248,227},//3
            {350,227},//4
            {452,227},//5
            {248,329},//6
            {350,329},//7
            {452,329},//8
        }*/

int main(int argc,char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Failed";
        return 1;
    }
    CreateWindow Window;
}
