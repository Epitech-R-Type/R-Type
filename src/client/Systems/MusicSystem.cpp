/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#include "MusicSystem.hpp"
#include <iostream>

std::queue<SFXID> MusicSystem::SFXQueue;

MusicSystem::MusicSystem() {
    Ray::InitAudioDevice();
    Ray::SetMasterVolume(this->_volume);
}

MusicSystem::~MusicSystem() {
    Ray::StopSoundMulti();
    Ray::UnloadMusicStream(this->_music);
    Ray::CloseAudioDevice();
}

void MusicSystem::apply() {
    Ray::UpdateMusicStream(this->_music);
    for (int i = MusicSystem::SFXQueue.size(); i != 0; i--) {
        this->playSFX(MusicSystem::SFXQueue.front());
        MusicSystem::SFXQueue.pop();
    }
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

void MusicSystem::playSFX(int ID) {
    if (SoundEffects.size() - 1 < ID || ID < 0)
        return;
    if (!HAS_KEY(this->SFXobjects, (SFXID)ID)) {
        const cmrc::file soundFile = this->_fs.open(SoundEffects[ID].path);
        unsigned char* soundBuffer = (unsigned char*)(soundFile.begin());
        Ray::Wave soundWave = Ray::LoadWaveFromMemory(".wav", soundBuffer, soundFile.size());
        Ray::Sound soundObject = LoadSoundFromWave(soundWave);
        Ray::SetSoundVolume(soundObject, 0.7);
        Ray::PlaySoundMulti(soundObject);
        this->SFXobjects[(SFXID)ID] = soundObject;
    } else
        Ray::PlaySoundMulti(this->SFXobjects[(SFXID)ID]);
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