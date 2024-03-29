/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    This header file contains configuration options for the plug-in. If you need to change any of
    these, it'd be wise to do so using the Jucer, rather than editing this file directly...

*/

#ifndef __PLUGINCHARACTERISTICS_0NRD9LLGO__
#define __PLUGINCHARACTERISTICS_0NRD9LLGO__

#define JucePlugin_Build_VST    1  // (If you change this value, you'll also need to re-export the projects using the Jucer)
#define JucePlugin_Build_AU     1  // (If you change this value, you'll also need to re-export the projects using the Jucer)
#define JucePlugin_Build_RTAS   0  // (If you change this value, you'll also need to re-export the projects using the Jucer)

#define JucePlugin_Name                 "TAL NoiseMaker Plugin"
#define JucePlugin_Desc                 "TAL-NoiseMaker"
#define JucePlugin_Manufacturer         "TAL-Togu Audio Line"
#define JucePlugin_ManufacturerCode     'TOGU'
#define JucePlugin_PluginCode           'ncut'
#define JucePlugin_MaxNumInputChannels  2
#define JucePlugin_MaxNumOutputChannels 2
#define JucePlugin_PreferredChannelConfigurations   {2, 2}
#define JucePlugin_IsSynth              1
#define JucePlugin_WantsMidiInput       1
#define JucePlugin_ProducesMidiOutput   0
#define JucePlugin_SilenceInProducesSilenceOut  0
#define JucePlugin_TailLengthSeconds    0
#define JucePlugin_EditorRequiresKeyboardFocus  1
#define JucePlugin_VersionCode          0x10001
#define JucePlugin_VersionString        "1.0.1"
#define JucePlugin_VSTUniqueID          JucePlugin_PluginCode
#define JucePlugin_VSTCategory          kPlugCategEffect
#define JucePlugin_AUMainType           kAudioUnitType_MusicDevice
#define JucePlugin_AUSubType            JucePlugin_PluginCode
#define JucePlugin_AUExportPrefix       TalNoiseMakerAU
#define JucePlugin_AUExportPrefixQuoted "TalNoiseMakerAU"
#define JucePlugin_AUManufacturerCode   JucePlugin_ManufacturerCode
#define JucePlugin_CFBundleIdentifier   ch.toguaudioline.talnoisemaker
#define JucePlugin_AUCocoaViewClassName TalNoiseMakerAU
#define JucePlugin_RTASCategory         ePlugInCategory_None
#define JucePlugin_RTASManufacturerCode JucePlugin_ManufacturerCode
#define JucePlugin_RTASProductId        JucePlugin_PluginCode
#define JUCE_USE_VSTSDK_2_4             1

#endif   // __PLUGINCHARACTERISTICS_0NRD9LLGO__
