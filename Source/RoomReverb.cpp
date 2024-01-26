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

#include "RoomReverb.h"

RoomReverb::RoomReverb()
{
    // initialize unused freeverb parameters
    mEarlyReflections.setdryr(0.0f);
    mEarlyReflections.setwetr(1.0f);
    mEarlyReflections.loadPresetReflection(0l);
    mEarlyReflections.setDiffusionApFreq(150.0f, 4.0f);
    mEarlyReflections.setLRCrossApFreq(750.0f, 4.0f);
    mEarlyReflections.setLRDelay(0.3f);
    mEarlyReflections.setMuteOnChange(false);
    mEarlyReflections.setoutputhpf(4.0f);
    mEarlyReflections.setPreDelay(0.0f);
    mLateReverb.setdryr(0.0f);
    mLateReverb.setwetr(1.0f);
    mLateReverb.setbassap(150.0f, 4.0f);
    mLateReverb.setbassboost(0.1f);
    mLateReverb.setdamp2(500.0f);
    mLateReverb.setbassbw(2.0f);
    mLateReverb.setcrossfeed(0.4f);
    mLateReverb.setdccutfreq(5.0f);
    mLateReverb.setdecay0(0.237f);
    mLateReverb.setdecay1(0.938f);
    mLateReverb.setdecay2(0.844f);
    mLateReverb.setdecay3(0.906f);
    mLateReverb.setdecayf(1.000f);
    mLateReverb.setdiffusion1(0.375f);
    mLateReverb.setdiffusion2(0.312f);
    mLateReverb.setdiffusion3(0.406f);
    mLateReverb.setdiffusion4(0.25f);
    mLateReverb.setmodulationnoise1(0.09f);
    mLateReverb.setmodulationnoise2(0.06f);
    mLateReverb.setMuteOnChange(false);
    mLateReverb.setinputdamp(20000.0f);
    mLateReverb.setoutputdampbw(2.0f);
    mLateReverb.setReverbType(2u);
    mLateReverb.setspin2wander(22.0f);
    mLateReverb.setspinlimit(20.0f);
    mLateReverb.setspinlimit2(12.0f);

    // initialize everything else
    setSampleRate(44100.0f);
    setDryLevel(80.0f);
    setEarlyLevel(10.0f);
    setEarlySendLevel(20.0f);
    setLateLevel(20.0f);
    setStereoWidth(100.0f);
    setEarlyDamping(16000.0f);
    setEarlyRoomSize(25.0f);
    setLateDamping(14000.0f);
    setLateDiffusion(50.0f);
    setLatePredelay(8.0f);
    setLateRoomSize(25.0f);
    setLateDecay(0.4f);
    setLateSpin(0.4f);
    setLateWander(30.0f);
}

void RoomReverb::setSampleRate(float sampleRate)
{
    mEarlyReflections.setSampleRate(sampleRate);
    mLateReverb.setSampleRate(sampleRate);
}

void RoomReverb::process(const float* leftChannelIn,
                         const float* rightChannelIn, float* leftChannelOut,
                         float* rightChannelOut, int numSamples)
{
    // updating these parameters only once per buffer prevents segmention faults
    if (mEarlyRoomSizeNeedsUpdate)
    {
        mEarlyReflections.setRSFactor(mEarlyRoomSize);
        mEarlyRoomSizeNeedsUpdate = false;
    }
    if (mLateRoomSizeNeedsUpdate)
    {
        mLateReverb.setRSFactor(mLateRoomSize);
        mLateRoomSizeNeedsUpdate = false;
    }
    if (mLatePredelayNeedsUpdate)
    {
        mLateReverb.setPreDelay(mLatePredelay);
        mLatePredelayNeedsUpdate = false;
    }

    // work on fixed size buffer chunks to be independent of the provided buffer size
    for (int offset = 0; offset < numSamples; offset += INTERNAL_BUFFER_SIZE)
    {
        int numSamplesInBuffer = numSamples - offset < INTERNAL_BUFFER_SIZE ? numSamples - offset : INTERNAL_BUFFER_SIZE;

        for (int sample = 0; sample < numSamplesInBuffer; ++sample)
        {
            mLeftBufferIn[sample] = leftChannelIn[offset + sample];
            mRightBufferIn[sample] = rightChannelIn[offset + sample];
        }

        mEarlyReflections.processreplace(mLeftBufferIn,
                                         mRightBufferIn,
                                         mLeftEarlyOut,
                                         mRightEarlyOut,
                                         numSamplesInBuffer);

        for (int sample = 0; sample < numSamplesInBuffer; ++sample)
        {
            mLeftLateIn[sample] = mEarlySendLevel * mLeftEarlyOut[sample] + mLeftBufferIn[sample];
            mRightLateIn[sample] = mEarlySendLevel * mRightEarlyOut[sample] + mRightBufferIn[sample];
        }

        mLateReverb.processreplace(mLeftLateIn,
                                   mRightLateIn,
                                   mLeftLateOut,
                                   mRightLateOut,
                                   numSamplesInBuffer);

        for (int sample = 0; sample < numSamplesInBuffer; ++sample)
        {
            leftChannelOut[offset + sample] = mDryLevel * mLeftBufferIn[sample] +
                                              mEarlyLevel * mLeftEarlyOut[sample] +
                                              mLateLevel * mLeftLateOut[sample];
            rightChannelOut[offset + sample] = mDryLevel * mRightBufferIn[sample] +
                                               mEarlyLevel * mRightEarlyOut[sample] +
                                               mLateLevel * mRightLateOut[sample];
        }
    }
}

void RoomReverb::mute()
{
    mEarlyReflections.mute();
    mLateReverb.mute();
}

void RoomReverb::setDryLevel(float dryLevel)
{
    mDryLevel = dryLevel / 100.0f;
}

void RoomReverb::setEarlyLevel(float earlyLevel)
{
    mEarlyLevel = earlyLevel / 100.0f;
}

void RoomReverb::setEarlySendLevel(float earlySend)
{
    mEarlySendLevel = earlySend / 100.0f;
}

void RoomReverb::setLateLevel(float lateLevel)
{
    mLateLevel = lateLevel / 100.0f;
}

void RoomReverb::setStereoWidth(float stereoWidth)
{
    // plugin range: 0 - 100
    // freeverb range: -1.0 - 1.0
    // freeverb range = plugin range / 50.0 - 1.0
    stereoWidth = stereoWidth / 50.0f - 1.0f;
    mEarlyReflections.setwidth(stereoWidth);
    mLateReverb.setwidth(stereoWidth);
}

void RoomReverb::setEarlyDamping(float earlyDamping)
{
    mEarlyReflections.setoutputlpf(earlyDamping);
}

void RoomReverb::setEarlyRoomSize(float earlyRoomSize)
{
    // plugin range: 0 - 100
    // freeverb range: 0.4 - 3.6
    // freeverb range = plugin range / 31.25 + 0.4
    mEarlyRoomSize = earlyRoomSize / 31.25f + 0.4f;
    mEarlyRoomSizeNeedsUpdate = true;
}

void RoomReverb::setLateDamping(float lateDamping)
{
    mLateReverb.setdamp(lateDamping);
    mLateReverb.setoutputdamp(lateDamping);
}

void RoomReverb::setLateDiffusion(float lateDiffusion)
{
    // limit values to avoid signal clips
    lateDiffusion = lateDiffusion / 105.0f;
    mLateReverb.setidiffusion1(lateDiffusion);
    mLateReverb.setodiffusion1(lateDiffusion);
}

void RoomReverb::setLatePredelay(float latePredelay)
{
    mLatePredelay = latePredelay;
    mLatePredelayNeedsUpdate = true;
}

void RoomReverb::setLateRoomSize(float lateRoomSize)
{
    // plugin range: 0 - 100
    // freeverb range: 0.4 - 3.6
    // freeverb range = plugin range / 31.25 + 0.4
    mLateRoomSize = lateRoomSize / 31.25f + 0.4f;
    mLateRoomSizeNeedsUpdate = true;
}

void RoomReverb::setLateDecay(float lateDecay)
{
    mLateReverb.setrt60(lateDecay);
}

void RoomReverb::setLateSpin(float lateSpin)
{
    mLateReverb.setspin(lateSpin);
    mLateReverb.setspin2(std::sqrt(100.0f - (10.0f - lateSpin) * (10.0f - lateSpin)) / 2.0f);
}

void RoomReverb::setLateWander(float lateWander)
{
    // limit values to avoid signal clips
    lateWander = lateWander / 200.0f + 0.1f;
    mLateReverb.setwander(lateWander);
    mLateReverb.setwander2(lateWander);
}
