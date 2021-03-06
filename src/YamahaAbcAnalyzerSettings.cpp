#include "YamahaAbcAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

#include <memory>


YamahaAbcAnalyzerSettings::YamahaAbcAnalyzerSettings() : mKC1Channel(UNDEFINED_CHANNEL),
                                                         mKC2Channel(UNDEFINED_CHANNEL),
                                                         mKC3Channel(UNDEFINED_CHANNEL),
                                                         mKC4Channel(UNDEFINED_CHANNEL),
                                                         mClockChannel(UNDEFINED_CHANNEL) {
    mKC1ChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mKC1ChannelInterface->SetTitleAndTooltip("KC1", "REPLACE ME");
    mKC1ChannelInterface->SetChannel(mKC1Channel);

    mKC2ChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mKC2ChannelInterface->SetTitleAndTooltip("KC2", "REPLACE ME");
    mKC2ChannelInterface->SetChannel(mKC2Channel);

    mKC3ChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mKC3ChannelInterface->SetTitleAndTooltip("KC3", "REPLACE ME");
    mKC3ChannelInterface->SetChannel(mKC3Channel);

    mKC4ChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mKC4ChannelInterface->SetTitleAndTooltip("KC4", "REPLACE ME");
    mKC4ChannelInterface->SetChannel(mKC4Channel);

    mClockChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mClockChannelInterface->SetTitleAndTooltip("Clock", "REPLACE ME");
    mClockChannelInterface->SetChannel(mClockChannel);

    AddInterface(mKC1ChannelInterface.get());
    AddInterface(mKC2ChannelInterface.get());
    AddInterface(mKC3ChannelInterface.get());
    AddInterface(mKC4ChannelInterface.get());
    AddInterface(mClockChannelInterface.get());

    AddExportOption(0, "Export as text/csv file");
    AddExportExtension(0, "text", "txt");
    AddExportExtension(0, "csv", "csv");

    AddAllChannels(false);
}

YamahaAbcAnalyzerSettings::~YamahaAbcAnalyzerSettings() = default;

bool YamahaAbcAnalyzerSettings::SetSettingsFromInterfaces() {
    mKC1Channel = mKC1ChannelInterface->GetChannel();
    mKC2Channel = mKC2ChannelInterface->GetChannel();
    mKC3Channel = mKC3ChannelInterface->GetChannel();
    mKC4Channel = mKC4ChannelInterface->GetChannel();
    mClockChannel = mClockChannelInterface->GetChannel();

    AddAllChannels(true);

    return true;
}

void YamahaAbcAnalyzerSettings::LoadSettings(const char *settings) {
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mKC1Channel;
    text_archive >> mKC2Channel;
    text_archive >> mKC3Channel;
    text_archive >> mKC4Channel;
    text_archive >> mClockChannel;

    AddAllChannels(true);

    // Update Interfaces From Settings
    mKC1ChannelInterface->SetChannel(mKC1Channel);
    mKC2ChannelInterface->SetChannel(mKC2Channel);
    mKC3ChannelInterface->SetChannel(mKC3Channel);
    mKC4ChannelInterface->SetChannel(mKC4Channel);
    mClockChannelInterface->SetChannel(mClockChannel);
}

void YamahaAbcAnalyzerSettings::AddAllChannels(bool isUsed) {
    ClearChannels();
    AddChannel(mKC1Channel, "YamahaKC1", isUsed);
    AddChannel(mKC2Channel, "YamahaKC2", isUsed);
    AddChannel(mKC3Channel, "YamahaKC3", isUsed);
    AddChannel(mKC4Channel, "YamahaKC4", isUsed);
    AddChannel(mClockChannel, "YamahaClock", isUsed);
}

const char *YamahaAbcAnalyzerSettings::SaveSettings() {
    SimpleArchive text_archive;

    text_archive << mKC1Channel;
    text_archive << mKC2Channel;
    text_archive << mKC3Channel;
    text_archive << mKC4Channel;
    text_archive << mClockChannel;

    return SetReturnString(text_archive.GetString());
}
