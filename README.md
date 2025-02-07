# Room Reverb

Room Reverb is a mono/stereo to stereo algorithmic reverb audio plugin with many presets. It uses the implementation of Moorer's early reflection model and Jon Dattorro's Progenitor Reverb from the Freeverb3 signal processing library.

For more information about Room Reverb, visit my [website](https://www.elephantdsp.com/products/room-reverb/).

![Room Reverb screenshot](Assets/room-reverb-screenshot.png)

## Download
The latest version of Room Reverb can be downloaded from my [website](https://www.elephantdsp.com/products/room-reverb/) or the iOS version from the [App Store](https://apps.apple.com/app/room-reverb-elephantdsp-com/id1665662082).

## Building Room Reverb

Room Reverb uses the JUCE C++ framework. If you are on Linux, you need to install [these dependencies](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md) first.
```bash
# Get the repository including submodules
git clone --recurse-submodules https://github.com/cvde/RoomReverb.git
cd RoomReverb

# Build plugin with CMake
cmake -B build
cmake --build build
```
After a successful build, the plugin binaries can be found in `build/RoomReverb_artefacts`.

## References
- [Freeverb3 signal processing library](https://www.nongnu.org/freeverb3/)
- [Freeverb3VST](https://freeverb3-vst.sourceforge.io/)
- [Dragonfly Reverb (also based on Freeverb3)](https://github.com/michaelwillis/dragonfly-reverb)

## Licenses
Room Reverb is free software and distributed under the [GNU General Public License Version 3](LICENSE).

Freeverb3 is free software and distributed under the [GNU General Public License Version 2](Libs/Freeverb3/COPYING) or (at your option) any later version.

JUCE is dual licenced under both the commercial [JUCE licence](https://juce.com/legal/juce-8-licence/) and the [GNU Affero General Public License Version 3](https://www.gnu.org/licenses/agpl-3.0.en.html).

<img src="Assets/VST_Compatible_Logo_Steinberg_with_TM_negative.svg" alt="Steinberg VST compatible logo" width="250" height="232"/>

All other trademarks, registered trademarks and logos or other manufacturers written or shown herein are the property of their respective owners. Use of them does not imply any affiliation with or endorsement by them.
