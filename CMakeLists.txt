cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_OSX_ARCHITECTURES arm64 x86_64)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(PROJECT_NAME "RoomReverb")
set(PROJECT_VERSION "0.6.1")

project(${PROJECT_NAME} VERSION ${PROJECT_VERSION})

# include JUCE
add_subdirectory(Libs/JUCE)

# include clap extension to build plugin in clap format
add_subdirectory(Libs/clap-juce-extensions EXCLUDE_FROM_ALL)

juce_add_plugin(${PROJECT_NAME}
    PLUGIN_NAME "Room Reverb"
    PLUGIN_CODE "Errp"
    PLUGIN_MANUFACTURER_CODE "Edsp"
    COMPANY_NAME "ElephantDSP.com"
    COMPANY_WEBSITE "https://www.ElephantDSP.com"
    COMPANY_EMAIL "mail@ElephantDSP.com"
    BUNDLE_ID "com.elephantdsp.roomreverb"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    FORMATS VST3 AU LV2 # Standalone Unity VST3 AU AUv3 AAX VST LV2
    VST3_CATEGORIES "Fx" "Reverb"
    AAX_CATEGORY "AAX_ePlugInCategory_Reverb"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    COPY_PLUGIN_AFTER_BUILD TRUE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
)

# configure clap plugin
clap_juce_extensions_plugin(
    TARGET ${PROJECT_NAME}
    CLAP_ID "com.elephantdsp.roomreverb"
    CLAP_FEATURES audio-effect reverb
)

# create Config.h file
configure_file(Config.h.in Config.h)
target_include_directories(${PROJECT_NAME}
    PUBLIC
        "${PROJECT_BINARY_DIR}"
)

# include Assets folder
add_subdirectory(Assets)

# include Source folder
add_subdirectory(Source)

# include Freeverb3 folder and add as include folder
add_subdirectory(Libs/Freeverb3)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "Libs/Freeverb3/"
)

# adds some preprocessor definitions for JUCE and freeverb
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_REPORT_APP_USAGE=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        LIBFV3_FLOAT # needed for freeverb
)

# link libraries
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        BinaryData
        juce_audio_processors
        juce_gui_basics
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
