/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#include "MusicSystem.hpp"
#include <iostream>

MusicSystem::MusicSystem(int ID)
{
    const cmrc::file menuMusic = this->_fs.open(Songs[ID].path);
    unsigned char* musicBuffer = (unsigned char*)(menuMusic.begin());

    InitAudioDevice();
    this->_music = LoadMusicStreamFromMemory(".mp3", musicBuffer, menuMusic.size());
    SetMasterVolume(this->_volume);
}

MusicSystem::~MusicSystem()
{
    StopSoundMulti();
    UnloadMusicStream(this->_music);
    CloseAudioDevice();
}

void MusicSystem::apply()
{
    UpdateMusicStream(this->_music);
    if (IsKeyPressed(KEY_P)) {
        if (IsMusicStreamPlaying(this->_music))
            PauseMusicStream(this->_music);
        else
            ResumeMusicStream(this->_music);
    }
    if (IsKeyPressed(KEY_Z) && this->_volume < 1.0)
        this->_volume += 0.1;
    if (IsKeyPressed(KEY_U) && this->_volume > 0)
        this->_volume -= 0.1;
    SetMasterVolume(this->_volume);
}

void MusicSystem::playSFX(int ID)
{
    if (SoundEffects.size() - 1 < ID || ID < 0)
        return;
    const cmrc::file soundFile = this->_fs.open(SoundEffects[ID].path);
    unsigned char* soundBuffer = (unsigned char*)(soundFile.begin());
    Wave soundWave = LoadWaveFromMemory(".wav", soundBuffer, soundFile.size());
    Sound soundObject = LoadSoundFromWave(soundWave);
    // SetSoundVolume(soundObject, 0.5);
    PlaySoundMulti(soundObject);
    UnloadSound(soundObject);
}

void MusicSystem::changeSong(int ID)
{
    if (Songs.size() - 1 < ID || ID < 0)
        return;
    const cmrc::file changedMusic = this->_fs.open(Songs[ID].path);
    unsigned char* musicBuffer = (unsigned char*)(changedMusic.begin());
    UnloadMusicStream(this->_music);
    this->_music = LoadMusicStreamFromMemory(".mp3", musicBuffer, changedMusic.size());
    PlayMusicStream(this->_music);
}