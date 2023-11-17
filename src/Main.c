#include "SDL_keycode.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <wchar.h>

typedef struct {
  bool keep_window_open;
  SDL_Window *window;
  SDL_Renderer *renderer;
  Mix_Chunk *bounce;
  int rect_x;
  int rect_y;
} GameState;

GameState make_initial_gamestate(SDL_Window *window, SDL_Renderer *renderer,
                                 Mix_Chunk *bounce) {
  GameState state;
  state.keep_window_open = true;
  state.window = window;
  state.renderer = renderer;
  state.bounce = bounce;
  state.rect_x = 5;
  state.rect_y = 23;
  return state;
}

void process_input(GameState *gamestate) {
  SDL_Event event;
  SDL_PollEvent(&event);
  switch (event.type) {
  case SDL_QUIT:
    gamestate->keep_window_open = false;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_q:
      gamestate->keep_window_open = false;
      break;
    }
  }
}

bool flip_x = false;
bool flip_y = false;

void update(GameState *state) {
  int win_width;
  int win_height;
  SDL_GetWindowSize(state->window, &win_width, &win_height);
  if (flip_x) {
    state->rect_x += 1;
    if (state->rect_x >= (win_width - 100)) {
      flip_x = false;
      Mix_PlayChannel(-1, state->bounce, 0);
    }
  } else {
    state->rect_x -= 1;
    if (state->rect_x <= 0) {
      flip_x = true;
      Mix_PlayChannel(-1, state->bounce, 0);
    }
  }

  if (flip_y) {
    state->rect_y += 1;
    if (state->rect_y >= (win_height - 100)) {
      flip_y = false;
      Mix_PlayChannel(-1, state->bounce, 0);
    }
  } else {
    state->rect_y -= 1;
    if (state->rect_y <= 0) {
      flip_y = true;
      Mix_PlayChannel(-1, state->bounce, 0);
    }
  }
}

void render(GameState *state) {
  SDL_SetRenderDrawColor(state->renderer, 0x00, 0x00, 0x00, 0xFF);
  SDL_RenderClear(state->renderer);

  int win_width;
  SDL_GetWindowSize(state->window, &win_width, NULL);

  SDL_Rect ball = {state->rect_x, state->rect_y, 100, 100};
  SDL_SetRenderDrawColor(state->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderFillRect(state->renderer, &ball);

  SDL_RenderPresent(state->renderer);
}

int main() {
  SDL_Window *window;
  SDL_Renderer *renderer;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
                 SDL_GetError());
    return EXIT_FAILURE;
  }

  if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &window,
                                  &renderer)) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                 "Couldn't create window and renderer: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO, "Couldn't open audio: %s",
                 SDL_GetError());
    return EXIT_FAILURE;
  }

  if (Mix_AllocateChannels(256) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                 "Couldn't allocate mixing channels: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  Mix_Chunk *sample = Mix_LoadWAV("assets/bounce.wav");
  if (sample == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_AUDIO,
                 "Couldn't allocate mixing channels: %s", SDL_GetError());
    return EXIT_FAILURE;
  }

  GameState state = make_initial_gamestate(window, renderer, sample);
  while (state.keep_window_open) {
    process_input(&state);
    update(&state);
    render(&state);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  Mix_FreeChunk(sample);
  Mix_CloseAudio();
  SDL_Quit();

  return 0;
}
