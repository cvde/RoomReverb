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

    void setSampleRate(float sampleRate);
    void process(const float* leftChannelIn, const float* rightChannelIn, float* leftChannelOut, float* rightChannelOut, int numSamples);
    void mute();

    // output
    void setDryLevel(float dryLevel);
    void setEarlyLevel(float earlyLevel);
    void setEarlySendLevel(float earlySend);
    void setLateLevel(float lateLevel);
    void setStereoWidth(float stereoWidth);

    // early reflections
    void setEarlyDamping(float earlyDamping);
    void setEarlyRoomSize(float earlyRoomSize);

    // late reverb
    void setLateDamping(float lateDamping);
    void setLateDiffusion(float lateDiffusion);
    void setLatePredelay(float latePredelay);
    void setLateRoomSize(float lateRoomSize);
    void setLateDecay(float lateDecay);
    void setLateSpin(float lateSpin);
    void setLateWander(float lateWander);

private:
    float mDryLevel = 0.0f;
    float mEarlyLevel = 0.0f;
    float mEarlySendLevel = 0.0f;
    float mLateLevel = 0.0f;

    std::atomic<bool> mEarlyRoomSizeNeedsUpdate{false};
    float mEarlyRoomSize = 0.0f;
    std::atomic<bool> mLateRoomSizeNeedsUpdate{false};
    float mLateRoomSize = 0.0f;
    std::atomic<bool> mLatePredelayNeedsUpdate{false};
    float mLatePredelay = 0.0f;

    static constexpr int INTERNAL_BUFFER_SIZE = 512;
    float mLeftBufferIn[INTERNAL_BUFFER_SIZE] = {};
    float mRightBufferIn[INTERNAL_BUFFER_SIZE] = {};
    float mLeftEarlyOut[INTERNAL_BUFFER_SIZE] = {};
    float mRightEarlyOut[INTERNAL_BUFFER_SIZE] = {};
    float mLeftLateIn[INTERNAL_BUFFER_SIZE] = {};
    float mRightLateIn[INTERNAL_BUFFER_SIZE] = {};
    float mLeftLateOut[INTERNAL_BUFFER_SIZE] = {};
    float mRightLateOut[INTERNAL_BUFFER_SIZE] = {};

    fv3::earlyref_f mEarlyReflections;
    fv3::progenitor2_f mLateReverb;
};
