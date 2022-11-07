/*
** EPITECH PROJECT, 2022
** R-Type
** File description:
** MusicSystem
*/

#pragma once

#include "../../shared/Systems/ISystem.hpp"
#include "../../shared/Utilities/ray.hpp"
#include <cmrc/cmrc.hpp>
#include <map>
CMRC_DECLARE(client);

struct Song {
    const std::string path;
};

enum SongID {
    NORMAL,
    BOSS,
};

static std::map<SongID, Song> Songs{
    {SongID::NORMAL, {"resources/song0.mp3"}},
    {SongID::BOSS, {"resources/song1.mp3"}},
};

class MusicSystem : public System {
public:
    MusicSystem();
    ~MusicSystem();
    void apply();
    void changeSong(SongID id);

private:
    Ray::Music _music;
    float _volume = 0.5;
    cmrc::embedded_filesystem _fs = cmrc::client::get_filesystem();
};
