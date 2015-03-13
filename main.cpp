
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include<iostream>
#include<vector>
#include<list>
#include "Sho.h"
#include "EnemigoAzul.h"
#include "EnemigoVerde.h"
#include "EnemigoRojo.h"
#include <iostream>
#include <fstream>


using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background;
SDL_Rect rect_background;
TTF_Font* Sans;
SDL_Color White;

void initTTF(){
    if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    exit(2);
}
//
//TTF_Font *myFont;
//myFont=TTF_OpenFont("font.ttf", 16);
//if(!myFont) {
//    printf("TTF_OpenFont: %s\n", TTF_GetError());
//    // handle error
//}

Sans = TTF_OpenFont("font.ttf", 24); //this opens a font style and sets a size
if(!Sans) {
    printf("TTF_OpenFont: %s\n", TTF_GetError());
    // handle error
}
White = {0, 0, 0};  // this is the color in rgb format, maxing out all would give you the color white, and it will be your text's color
cout<<"Finalice"<<endl;
}

void showTTF(int frame){
//    int minx = 0;
//int maxx = 0;
//int miny = 0;
//int maxy = 0;
//int advance = 0;
string esto = ""+frame;
cout<<esto<<endl;
SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, (char*)&frame, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage); //now you can convert it into a texture

SDL_Rect Message_rect; //create a rect
Message_rect.x = 0;  //controls the rect's x coordinate
Message_rect.y = 0; // controls the rect's y coordinte
Message_rect.w = 100; // controls the width of the rect
Message_rect.h = 100; // controls the height of the rect

//Mind you that (0,0) is on the top left of the window/screen, think a rect as the text's box, that way it would be very simple to understance

//Now since it's a texture, you have to put RenderCopy in your game loop area, the area where the whole code executes

SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

}

int readFrame(){
    cout<<"Leyendo"<<endl;
    ifstream in("points");
    in.seekg(0);
    int puntaje;
    in.read((char*)&puntaje,4);
    in.close();
    return puntaje;
}

void writeFrame(int frame){
    cout<<"Escribiendo nueva cantidad de frames: ";
    ofstream out("points");
    out.write((char*)&frame,4);
    cout<<frame<<endl;
    out.close();
}

void loopInstruc(){
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }

        int w=0,h=0;
        background = IMG_LoadTexture(renderer,"Instrucciones.png");
        SDL_QueryTexture(background, NULL, NULL, &w, &h);
        rect_background.x = 0;
        rect_background.y = 0;
        rect_background.w = w;
        rect_background.h = h;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer,background,NULL,&rect_background);
        SDL_RenderPresent(renderer);
    }
}

void loopJuego()
{
    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"fondo.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;


    list<Personaje*> personajes;
    personajes.push_back(new Sho(renderer,&personajes));
    personajes.push_back(new EnemigoVerde(renderer,&personajes));
    personajes.push_back(new EnemigoRojo(renderer,&personajes));

    //Main Loop
    int frame=0;
    int animacion_sho = 0;
    bool over = false;
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
            }
        }

        if(frame%1000==0)
        {
            personajes.push_back(new EnemigoAzul(renderer,&personajes));
        }

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->act();

        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 255);

        // Clear the entire screen to our selected color.
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        //showTTF(frame);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            (*p)->draw(renderer);

        for(list<Personaje*>::iterator p=personajes.begin();
                p!=personajes.end();
                p++)
            if((*p)->muerto)
            {
                if ((*p)->clase=="Sho")
                    over = true;
                personajes.erase(p);
                break;
            }

        SDL_RenderPresent(renderer);

        frame++;
        if (over){
            if (readFrame()<frame)
                writeFrame(frame);
            break;
        }
    }
}

class MenuButton
{
public:
    SDL_Texture *selected;
    SDL_Texture *unselected;
    SDL_Rect rect;
    bool is_selected;
    MenuButton(string selected, string unselected, int x, int y)
    {
        this->selected = IMG_LoadTexture(renderer,selected.c_str());
        this->unselected = IMG_LoadTexture(renderer,unselected.c_str());
        SDL_QueryTexture(this->selected, NULL, NULL, &rect.w, &rect.h);
        rect.x = x;
        rect.y = y;
        is_selected = false;
    }

    void render()
    {
        if(is_selected)
            SDL_RenderCopy(renderer,selected,NULL,&rect);
        else
            SDL_RenderCopy(renderer,unselected,NULL,&rect);
    }

    void select()
    {
        is_selected = true;
    }

    void unselect()
    {
        is_selected = false;
    }
};

void mainMenu()
{
    int opcion = 1;
    SDL_Texture *menu_fondo = IMG_LoadTexture(renderer,"menu_fondo.png");
    SDL_Rect menu_rect;
    vector<MenuButton*>buttons;
    buttons.push_back(new MenuButton("button1_selected.png","button1_unselected.png",300,300));
    buttons.push_back(new MenuButton("button2_selected.png","button2_unselected.png",300,400));
    buttons.push_back(new MenuButton("button3_selected.png","button3_unselected.png",300,500));
    buttons.push_back(new MenuButton("reset_selected.png","reset_unselected.png",300,600));
    SDL_QueryTexture(menu_fondo, NULL, NULL, &menu_rect.w, &menu_rect.h);
    menu_rect.x = 0;
    menu_rect.y = 0;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {
                if(Event.key.keysym.sym == SDLK_2)
                {
                    exit(0);
                }
                if(Event.key.keysym.sym == SDLK_1)
                {
                    loopJuego();
                }
                if(Event.key.keysym.sym == SDLK_ESCAPE)
                {
                    return;
                }
                if(Event.key.keysym.sym == SDLK_DOWN)
                {
                    opcion++;
                    if(opcion > 4)
                        opcion = 4;
                }
                if(Event.key.keysym.sym == SDLK_UP)
                {
                    opcion--;
                    if(opcion < 1)
                        opcion = 1;
                }
                if(Event.key.keysym.sym == SDLK_RETURN)
                {
                    switch(opcion)
                    {
                        case 1:
                            loopJuego();
                        break;
                        case 2:
                            loopInstruc();
                        break;
                        case 3:
                            exit(0);
                        break;
                        case 4:
                            writeFrame(0);
                        break;
                    }
                }
            }
        }

        SDL_RenderCopy(renderer,menu_fondo,NULL,&menu_rect);



        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->unselect();
        }
        buttons[opcion-1]->select();
        for(int i=0;i<buttons.size();i++)
        {
            buttons[i]->render();
        }

        SDL_RenderPresent(renderer);
    }
}

int main( int argc, char* args[] )
{
    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 1024/*WIDTH*/, 768/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }
    initTTF();
    mainMenu();

	return 0;
}
