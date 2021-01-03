#ifndef VIDEO_H
#define VIDEO_H

class Video
{
public:
  SDL_Surface * init();
  void free();
};

SDL_Surface * Video::init()
{
  SDL_Surface * screen;

  if (SDL_Init (SDL_INIT_VIDEO)) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  screen = SDL_SetVideoMode (800, 600, 16, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);//SDL_NOFRAM

  if (screen == NULL) {
    printf("SDL could not set video mode! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  return screen;
}

#endif
