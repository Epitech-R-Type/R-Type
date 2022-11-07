/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#include "MusicSystem.hpp"
#include <iostream>

MusicSystem::MusicSystem() {
    Ray::InitAudioDevice();
    Ray::SetMasterVolume(this->_volume);
}

MusicSystem::~MusicSystem() {
    Ray::UnloadMusicStream(this->_music);
    Ray::CloseAudioDevice();
}

void MusicSystem::apply() {
    Ray::UpdateMusicStream(this->_music);
    if (Ray::IsKeyPressed(Ray::KEY_P)) {
        if (IsMusicStreamPlaying(this->_music))
            Ray::PauseMusicStream(this->_music);
        else
            Ray::ResumeMusicStream(this->_music);
    }
    if (Ray::IsKeyPressed(Ray::KEY_Z) && this->_volume < 1.0)
        this->_volume += 0.1;
    if (Ray::IsKeyPressed(Ray::KEY_U) && this->_volume > 0)
        this->_volume -= 0.1;
    Ray::SetMasterVolume(this->_volume);
}

void MusicSystem::changeSong(SongID id) {
    const cmrc::file changedMusic = this->_fs.open(Songs[id].path);

    unsigned char* musicBuffer = (unsigned char*)(changedMusic.begin());
    if (Ray::IsMusicStreamPlaying(this->_music))
        Ray::UnloadMusicStream(this->_music);
    this->_music = Ray::LoadMusicStreamFromMemory(".mp3", musicBuffer, changedMusic.size());

    Ray::PlayMusicStream(this->_music);
    Ray::SetMasterVolume(this->_volume);
}