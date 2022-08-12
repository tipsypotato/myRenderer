#include "common.h"

int GEngine::WINDOW_CONFIG::WINDOW_WIDTH = 1280;
int GEngine::WINDOW_CONFIG::WINDOW_HEIGHT = 720;
int GEngine::WINDOW_CONFIG::VIEWPORT_LOWERLEFT_X = 0;
int GEngine::WINDOW_CONFIG::VIEWPORT_LOWERLEFT_Y = 0;
int GEngine::WINDOW_CONFIG::VIEWPORT_WIDTH = GEngine::WINDOW_CONFIG::WINDOW_WIDTH;
int GEngine::WINDOW_CONFIG::VIEWPORT_HEIGHT = GEngine::WINDOW_CONFIG::WINDOW_HEIGHT;
std::string GEngine::WINDOW_CONFIG::WINDOW_TITLE = "GEngine";

// TRUE if on mac retina screen 
bool GEngine::WINDOW_CONFIG::IS_MACOS_WINDOW = true;