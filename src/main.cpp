#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <string>

#include "ShadowFrame/shadowFrame.h"

int main(int argc, char *argv[])
{
  ShadowFrame shadowFrame;

  shadowFrame.setWindowTitle("Shadow Frame");
  shadowFrame.setVSync(1);
  shadowFrame.setWindowDimensions(800, 600);
  shadowFrame.setBackgroundColor(255, 0, 0, SDL_ALPHA_OPAQUE);
  shadowFrame.open();

  // SDL_Init(SDL_INIT_VIDEO);
  // TTF_Init();

  // SDL_Window *window = SDL_CreateWindow("Window", 640, 480, SDL_WINDOW_RESIZABLE);
  // SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  // SDL_SetRenderVSync(renderer, 1);

  // bool quit = false;
  // SDL_Event e;

  // const char *content = "Hello world";
  // TTF_TextEngine *engine = TTF_CreateRendererTextEngine(renderer);

  // TTF_Font *font = TTF_OpenFont("fonts/Roboto-Regular.ttf", 32);
  // TTF_Text *text1 = TTF_CreateText(engine, font, NULL, 0);

  // TTF_Font *font2 = TTF_OpenFont("fonts/Roboto-Regular.ttf", 32);
  // TTF_Text *text2 = TTF_CreateText(engine, font2, NULL, 0);

  // TTF_SetTextColor(text2, 255, 0, 0, 255);
  // while (!quit)
  // {

  //   while (SDL_PollEvent(&e) != 0)
  //   {
  //     if (e.type == SDL_EVENT_QUIT)
  //     {
  //       quit = true;
  //       break;
  //     }
  //   }

  //   SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  //   SDL_RenderClear(renderer);

  //   std::string content1 = "1 FPS: " + std::to_string((int)(1 / 1));
  //   // TTF_SetTextString(text1, content1.c_str(), 0);
  //   TTF_DrawRendererText(text1, 0, 0);

  //   std::string content2 = "2 FPS: " + std::to_string((int)(1 / 1));
  //   TTF_SetTextString(text2, content2.c_str(), 0);
  //   TTF_DrawRendererText(text2, 0, 100);

  //   std::string content3 = "3 FPS: " + std::to_string((int)(1 / 1));
  //   TTF_SetTextString(text2, content3.c_str(), 0);
  //   TTF_DrawRendererText(text2, 0, 200);

  //   SDL_RenderPresent(renderer);
  // }

  // TTF_DestroyText(text1);
  // TTF_DestroyText(text2);
  // TTF_DestroyRendererTextEngine(engine);
  // TTF_CloseFont(font);
  // TTF_Quit();

  // SDL_DestroyRenderer(renderer);
  // SDL_DestroyWindow(window);
  return 0;
}