#include "game.h"
#include "../lib/image.cpp"
#include "../lib/audio.cpp"
#include <iostream>
#include <thread>

void playBackGroundMusic()
{
    playingMusic("assets/background_sound.mp3");
}

void playGameOverMusic()
{
    playingMusic("assets/game_over.mp3");
}

void playMergeSquareMusic()
{
    playingMusic("assets/drop.wav");
}

void setBackgroundImage(int width, int heigth)
{
    loadTexture("assets/background2.jpeg", width, heigth);
}