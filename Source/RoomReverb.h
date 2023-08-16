/**
 *  ElephantDSP.com Room Reverb
 *
 *  Copyright (C) 2021 Christian Voigt
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "freeverb/earlyref.hpp"
#include "freeverb/progenitor2.hpp"
#include <atomic>

class RoomReverb
{
public:
    RoomReverb();

    void setSampleRate(float newSampleRate);
    void process(const float* leftChannelIn, const float* rightChannelIn, float* leftChannelOut, float* rightChannelOut, int numSamples);
    void mute();

    // output
    void setDryLevel(float newDryLevel);
    void setEarlyLevel(float newEarlyLevel);
    void setEarlySendLevel(float newEarlySend);
    void setLateLevel(float newLateLevel);
    void setStereoWidth(float newStereoWidth);

    // early reflections
    void setEarlyDamping(float newEarlyDamping);
    void setEarlyRoomSize(float newEarlyRoomSize);

    // late reverb
    void setLateDamping(float newLateDamping);
    void setLateDiffusion(float newLateDiffusion);
    void setLatePredelay(float newLatePredelay);
    void setLateRoomSize(float newLateRoomSize);
    void setLateDecay(float newLateDecay);
    void setLateSpin(float newLateSpin);
    void setLateWander(float newLateWander);

    // void dumpSettings();

private:
    float dryLevel;
    float earlyLevel;
    float earlySendLevel;
    float lateLevel;

    std::atomic<bool> earlyRoomSizeNeedsUpdate = false;
    float earlyRoomSize;
    std::atomic<bool> lateRoomSizeNeedsUpdate = false;
    float lateRoomSize;
    std::atomic<bool> latePredelayNeedsUpdate = false;
    float latePredelay;

    static constexpr int bufferSize = 512;
    float leftBufferIn[bufferSize];
    float rightBufferIn[bufferSize];
    float leftEarlyOut[bufferSize];
    float rightEarlyOut[bufferSize];
    float leftLateIn[bufferSize];
    float rightLateIn[bufferSize];
    float leftLateOut[bufferSize];
    float rightLateOut[bufferSize];

    fv3::earlyref_f early;
    fv3::progenitor2_f late;
};
