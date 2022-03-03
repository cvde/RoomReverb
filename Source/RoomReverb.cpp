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
    early.setdryr(0.0f);
    early.setwetr(1.0f);
    early.loadPresetReflection(0);
    early.setDiffusionApFreq(150.0f, 4.0f);
    early.setLRCrossApFreq(750.0f, 4.0f);
    early.setLRDelay(0.3f);
    early.setMuteOnChange(false);
    early.setoutputhpf(4.0f);
    early.setPreDelay(0.0f);
    late.setdryr(0.0f);
    late.setwetr(1.0f);
    late.setbassap(150.0f, 4.0f);
    late.setbassboost(0.1f);
    late.setdamp2(500.0f);
    late.setbassbw(2.0f);
    late.setcrossfeed(0.4f);
    late.setdccutfreq(5.0f);
    late.setdecay0(0.237f);
    late.setdecay1(0.938f);
    late.setdecay2(0.844f);
    late.setdecay3(0.906f);
    late.setdecayf(1.000f);
    late.setdiffusion1(0.375f);
    late.setdiffusion2(0.312f);
    late.setdiffusion3(0.406f);
    late.setdiffusion4(0.25f);
    late.setmodulationnoise1(0.09f);
    late.setmodulationnoise2(0.06f);
    late.setMuteOnChange(false);
    late.setinputdamp(20000.0f);
    late.setoutputdampbw(2.0f);
    late.setReverbType(2);
    late.setspin2wander(22.0f);
    late.setspinlimit(20.0f);
    late.setspinlimit2(12.0f);

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

void RoomReverb::setSampleRate(float newSampleRate)
{
    early.setSampleRate(newSampleRate);
    late.setSampleRate(newSampleRate);
}

void RoomReverb::process(const float* leftChannelIn,
                         const float* rightChannelIn, float* leftChannelOut,
                         float* rightChannelOut, int numSamples)
{
    // updating these parameters only once per buffer prevents segmention faults
    if (earlyRoomSizeNeedsUpdate)
    {
        early.setRSFactor(earlyRoomSize);
        earlyRoomSizeNeedsUpdate = false;
    }
    if (lateRoomSizeNeedsUpdate)
    {
        late.setRSFactor(lateRoomSize);
        lateRoomSizeNeedsUpdate = false;
    }
    if (latePredelayNeedsUpdate)
    {
        late.setPreDelay(latePredelay);
        latePredelayNeedsUpdate = false;
    }

    // split the buffer into fixed size chunks
    for (int offset = 0; offset < numSamples; offset += bufferSize)
    {
        int numSamplesInBuffer = numSamples - offset < bufferSize ? numSamples - offset : bufferSize;

        for (int i = 0; i < numSamplesInBuffer; ++i)
        {
            leftBufferIn[i] = leftChannelIn[offset + i];
            rightBufferIn[i] = rightChannelIn[offset + i];
        }

        early.processreplace(leftBufferIn,
                             rightBufferIn,
                             leftEarlyOut,
                             rightEarlyOut,
                             numSamplesInBuffer);

        for (int i = 0; i < numSamplesInBuffer; ++i)
        {
            leftLateIn[i] = earlySendLevel * leftEarlyOut[i] + leftBufferIn[i];
            rightLateIn[i] = earlySendLevel * rightEarlyOut[i] + rightBufferIn[i];
        }

        late.processreplace(leftLateIn,
                            rightLateIn,
                            leftLateOut,
                            rightLateOut,
                            numSamplesInBuffer);

        for (int i = 0; i < numSamplesInBuffer; ++i)
        {
            leftChannelOut[offset + i] = dryLevel * leftBufferIn[i] +
                                         earlyLevel * leftEarlyOut[i] +
                                         lateLevel * leftLateOut[i];
            rightChannelOut[offset + i] = dryLevel * rightBufferIn[i] +
                                          earlyLevel * rightEarlyOut[i] +
                                          lateLevel * rightLateOut[i];
        }
    }
}

void RoomReverb::mute()
{
    early.mute();
    late.mute();
}

void RoomReverb::setDryLevel(float newDryLevel)
{
    dryLevel = newDryLevel / 100.0f;
}

void RoomReverb::setEarlyLevel(float newEarlyLevel)
{
    earlyLevel = newEarlyLevel / 100.0f;
}

void RoomReverb::setEarlySendLevel(float newEarlySend)
{
    earlySendLevel = newEarlySend / 100.0f;
}

void RoomReverb::setLateLevel(float newLateLevel)
{
    lateLevel = newLateLevel / 100.0f;
}

void RoomReverb::setStereoWidth(float newStereoWidth)
{
    // plugin range: 0 - 100
    // freeverb range: -1.0 - 1.0
    // freeverb range = plugin range / 50.0 - 1.0
    early.setwidth(newStereoWidth / 50.0f - 1.0f);
    late.setwidth(newStereoWidth / 50.0f - 1.0f);
}

void RoomReverb::setEarlyDamping(float newEarlyDamping)
{
    early.setoutputlpf(newEarlyDamping);
}

void RoomReverb::setEarlyRoomSize(float newEarlyRoomSize)
{
    // plugin range: 0 - 100
    // freeverb range: 0.4 - 3.6
    // freeverb range = plugin range / 31.25 + 0.4
    earlyRoomSize = newEarlyRoomSize / 31.25f + 0.4f;
    earlyRoomSizeNeedsUpdate = true;
}

void RoomReverb::setLateDamping(float newLateDamping)
{
    late.setdamp(newLateDamping);
    late.setoutputdamp(newLateDamping);
}

void RoomReverb::setLateDiffusion(float newLateDiffusion)
{
    // limit values to avoid signal clips
    late.setidiffusion1(newLateDiffusion / 105.0f);
    late.setodiffusion1(newLateDiffusion / 105.0f);
}

void RoomReverb::setLatePredelay(float newLatePredelay)
{
    latePredelay = newLatePredelay;
    latePredelayNeedsUpdate = true;
}

void RoomReverb::setLateRoomSize(float newLateRoomSize)
{
    // plugin range: 0 - 100
    // freeverb range: 0.4 - 3.6
    // freeverb range = plugin range / 31.25 + 0.4
    lateRoomSize = newLateRoomSize / 31.25f + 0.4f;
    lateRoomSizeNeedsUpdate = true;
}

void RoomReverb::setLateDecay(float newLateDecay)
{
    late.setrt60(newLateDecay);
}

void RoomReverb::setLateSpin(float newLateSpin)
{
    late.setspin(newLateSpin);
    late.setspin2(std::sqrt(100.0f - (10.0f - newLateSpin) * (10.0f - newLateSpin)) / 2.0f);
}

void RoomReverb::setLateWander(float newLateWander)
{
    // limit values to avoid signal clips
    late.setwander(newLateWander / 200.0f + 0.1f);
    late.setwander2(newLateWander / 200.0f + 0.1f);
}

// void RoomReverb::dumpSettings()
// {
//     std::cout << "LEVEL:\n";
//     std::cout << "dryLevel:" << dryLevel << "\n";
//     std::cout << "earlyLevel:" << earlyLevel << "\n";
//     std::cout << "earlySendLevel:" << earlySendLevel << "\n";
//     std::cout << "lateLevel:" << lateLevel << "\n";
//     std::cout << "EARLY:\n";
//     std::cout << "early.getdryr:" << early.getdryr() << "\n";
//     std::cout << "early.getwetr:" << early.getwetr() << "\n";
//     std::cout << "early.getCurrentPreset:" << early.getCurrentPreset() << "\n";
//     std::cout << "early.getDiffusionApFreq:" << early.getDiffusionApFreq() << "\n";
//     std::cout << "early.getLRCrossApFreq:" << early.getLRCrossApFreq() << "\n";
//     std::cout << "early.getLRDelay:" << early.getLRDelay() << "\n";
//     std::cout << "early.getMuteOnChange:" << early.getMuteOnChange() << "\n";
//     std::cout << "early.getoutputhpf:" << early.getoutputhpf() << "\n";
//     std::cout << "early.getoutputlpf:" << early.getoutputlpf() << "\n";
//     std::cout << "early.getPreDelay:" << early.getPreDelay() << "\n";
//     std::cout << "early.getRSFactor:" << early.getRSFactor() << "\n";
//     std::cout << "early.getSampleRate:" << early.getSampleRate() << "\n";
//     std::cout << "early.getwidth:" << early.getwidth() << "\n";
//     std::cout << "LATE:\n";
//     std::cout << "late.getdryr:" << late.getdryr() << "\n";
//     std::cout << "late.getwetr:" << late.getwetr() << "\n";
//     std::cout << "late.getBassAp:" << late.getbassap() << "\n";
//     std::cout << "late.getbassboost:" << late.getbassboost() << "\n";
//     std::cout << "late.getdamp2:" << late.getdamp2() << "\n";
//     std::cout << "late.getbassbw:" << late.getbassbw() << "\n";
//     std::cout << "late.getcrossfeed:" << late.getcrossfeed() << "\n";
//     std::cout << "late.getdamp:" << late.getdamp() << "\n";
//     std::cout << "late.getdccutfreq:" << late.getdccutfreq() << "\n";
//     std::cout << "late.getdecay0:" << late.getdecay0() << "\n";
//     std::cout << "late.getdecay1:" << late.getdecay1() << "\n";
//     std::cout << "late.getdecay2:" << late.getdecay2() << "\n";
//     std::cout << "late.getdecay3:" << late.getdecay3() << "\n";
//     std::cout << "late.getdecayf:" << late.getdecayf() << "\n";
//     std::cout << "late.getdiffusion1:" << late.getdiffusion1() << "\n";
//     std::cout << "late.getdiffusion2:" << late.getdiffusion2() << "\n";
//     std::cout << "late.getdiffusion3:" << late.getdiffusion3() << "\n";
//     std::cout << "late.getdiffusion4:" << late.getdiffusion4() << "\n";
//     std::cout << "late.getidiffusion1:" << late.getidiffusion1() << "\n";
//     std::cout << "late.getodiffusion1:" << late.getodiffusion1() << "\n";
//     std::cout << "late.getmodulationnoise1:" << late.getmodulationnoise1() << "\n";
//     std::cout << "late.getmodulationnoise2:" << late.getmodulationnoise2() << "\n";
//     std::cout << "late.getMuteOnChange:" << late.getMuteOnChange() << "\n";
//     std::cout << "late.getinputdamp:" << late.getinputdamp() << "\n";
//     std::cout << "late.getoutputdamp:" << late.getoutputdamp() << "\n";
//     std::cout << "late.getoutputdampbw:" << late.getoutputdampbw() << "\n";
//     std::cout << "late.getPreDelay:" << late.getPreDelay() << "\n";
//     std::cout << "late.getReverbType:" << late.getReverbType() << "\n";
//     std::cout << "late.getRSFactor:" << late.getRSFactor() << "\n";
//     std::cout << "late.getrt60:" << late.getrt60() << "\n";
//     std::cout << "late.getSampleRate:" << late.getSampleRate() << "\n";
//     std::cout << "late.getspin:" << late.getspin() << "\n";
//     std::cout << "late.getspin2:" << late.getspin2() << "\n";
//     std::cout << "late.getspin2wander:" << late.getspin2wander() << "\n";
//     std::cout << "late.getspinlimit:" << late.getspinlimit() << "\n";
//     std::cout << "late.getspinlimit2:" << late.getspinlimit2() << "\n";
//     std::cout << "late.getwander:" << late.getwander() << "\n";
//     std::cout << "late.getwander2:" << late.getwander2() << "\n";
//     std::cout << "late.getwidth:" << late.getwidth() << "\n";
// }
