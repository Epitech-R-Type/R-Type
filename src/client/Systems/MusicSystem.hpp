/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#pragma once

#include "../../shared/Systems/ISystem.hpp"
#include "../../shared/Utilities/Utilities.hpp"
#include "../../shared/Utilities/ray.hpp"
#include <cmrc/cmrc.hpp>
#include <map>
#include <queue>

CMRC_DECLARE(client);

struct Song {
    const std::string path;
};

struct SFX {
    const std::string path;
};

static std::map<SongID, Song> Songs{
    {SongID::BOSS, {"resources/song0.mp3"}},
    {SongID::NORMAL, {"resources/song1.mp3"}},
    {SongID::PIANO, {"resources/song2.mp3"}},
};

static std::map<SFXID, SFX> SoundEffects{
    {SFXID::KNOCK, {"resources/gunShot1.wav"}},
    {SFXID::LIGHT_GUNSHOT, {"resources/gunShot2.wav"}},
    {SFXID::HEAVY_GUNSHOT, {"resources/gunShot3.wav"}},
};

class MusicSystem : public System {
public:
    static std::queue<SFXID> SFXQueue;

    MusicSystem();
    ~MusicSystem();
    void apply();
    void changeSong(SongID id);
    void playSFX(SFXID ID);

private:
    Music _music;
    float _volume = 0.5;
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
    std::map<SFXID, Sound> SFXobjects;
};
