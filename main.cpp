#include <SDL2/SDL.h>

int main(int argc,char** argv) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_CreateWindowAndRenderer(500,500,SDL_WINDOW_SHOWN,&win,&ren);
    bool shouldClose = false;
    while(!shouldClose) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) shouldClose = true;
        }
        SDL_RenderClear(ren);
        SDL_RenderPresent(ren);
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}