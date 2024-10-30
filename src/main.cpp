#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <string>

#include "FPSManager/FPSManager.h"

int main(int argc, char *argv[])
{
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  SDL_Window *window = SDL_CreateWindow("Window", 640, 480, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  SDL_SetRenderVSync(renderer, 1);

  bool quit = false;
  SDL_Event e;
  FPSManager fpsManager;

  const char *content = "Hello world";
  TTF_Font *font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 32);
  TTF_TextEngine *engine = TTF_CreateRendererTextEngine(renderer);
  TTF_Text *text = TTF_CreateText(engine, font, content, strlen(content));

  while (!quit)
  {
    float deltaTime = fpsManager.update();

    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_EVENT_QUIT)
      {
        quit = true;
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    std::string content = "FPS: " + std::to_string((int)(1 / deltaTime));
    TTF_SetTextString(text, content.c_str(), 0);
    TTF_DrawRendererText(text, 0, 0);

    SDL_RenderPresent(renderer);
  }

  TTF_DestroyText(text);
  TTF_DestroyRendererTextEngine(engine);
  TTF_CloseFont(font);
  TTF_Quit();

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  return 0;
}