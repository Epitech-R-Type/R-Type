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
    InitAudioDevice();
    SetMasterVolume(this->_volume);
}

MusicSystem::~MusicSystem() {
    StopSoundMulti();
    UnloadMusicStream(this->_music);
    CloseAudioDevice();
}

void MusicSystem::apply() {
    UpdateMusicStream(this->_music);
    for (int i = MusicSystem::SFXQueue.size(); i != 0; i--) {
        this->playSFX(MusicSystem::SFXQueue.front());
        MusicSystem::SFXQueue.pop();
    }
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

void MusicSystem::playSFX(int ID) {
    if (SoundEffects.size() - 1 < ID || ID < 0)
        return;
    if (!HAS_KEY(this->SFXobjects, (SFXID)ID)) {
        const cmrc::file soundFile = this->_fs.open(SoundEffects[ID].path);
        unsigned char* soundBuffer = (unsigned char*)(soundFile.begin());
        Wave soundWave = LoadWaveFromMemory(".wav", soundBuffer, soundFile.size());
        Sound soundObject = LoadSoundFromWave(soundWave);
        SetSoundVolume(soundObject, 0.7);
        PlaySoundMulti(soundObject);
        this->SFXobjects[(SFXID)ID] = soundObject;
    } else
        PlaySoundMulti(this->SFXobjects[(SFXID)ID]);
}

void MusicSystem::changeSong(SongID id) {
    const cmrc::file changedMusic = this->_fs.open(Songs[id].path);

    unsigned char* musicBuffer = (unsigned char*)(changedMusic.begin());
    if (IsMusicStreamPlaying(this->_music))
        UnloadMusicStream(this->_music);
    this->_music = LoadMusicStreamFromMemory(".mp3", musicBuffer, changedMusic.size());

    PlayMusicStream(this->_music);
}