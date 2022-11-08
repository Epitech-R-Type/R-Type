/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#pragma once

#include "../../shared/Utilities/Utilities.hpp"
#include "../../shared/Systems/ISystem.hpp"
#include "../../shared/Utilities/ray.hpp"
#include <cmrc/cmrc.hpp>
#include <queue>
#include <map>

CMRC_DECLARE(client);

struct Song {
    const std::string path;
};

struct SFX {
    const std::string path;
};

enum SongID {
    NORMAL,
    BOSS,
};

static std::map<SongID, Song> Songs {
    {SongID::NORMAL, {"resources/song0.mp3"}},
    {SongID::BOSS, {"resources/song1.mp3"}},
};

static std::vector<SFX> SoundEffects {
    {"resources/gunShot1.wav"},
    {"resources/gunShot2.wav"},
    {"resources/gunShot3.wav"}
};

class MusicSystem : public System {
public:
    static std::queue<SFXID> SFXQueue;

    MusicSystem();
    ~MusicSystem();
    void apply();
    void changeSong(SongID id);
    void playSFX(int ID);

private:
    Ray::Music _music;
    float _volume = 0.5;
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
    std::map<SFXID, Ray::Sound> SFXobjects;
};
