#include "YamahaAbcAnalyzerSettings.h"
#include <AnalyzerHelpers.h>

#include <memory>


YamahaAbcAnalyzerSettings::YamahaAbcAnalyzerSettings() : mInputChannel(UNDEFINED_CHANNEL), mBitRate(9600) {
    mInputChannelInterface = std::make_unique<AnalyzerSettingInterfaceChannel>();
    mInputChannelInterface->SetTitleAndTooltip("Serial", "Standard Yamaha ABC Keycodes");
    mInputChannelInterface->SetChannel(mInputChannel);

    mBitRateInterface = std::make_unique<AnalyzerSettingInterfaceInteger>();
    mBitRateInterface->SetTitleAndTooltip("Bbit Rate (Bits/S)", "Specify the bit rate in bits per second.");
    mBitRateInterface->SetMax(6000000);
    mBitRateInterface->SetMin(1);
    mBitRateInterface->SetInteger(mBitRate);

    AddInterface(mInputChannelInterface.get());
    AddInterface(mBitRateInterface.get());

    AddExportOption(0, "Export as text/csv file");
    AddExportExtension(0, "text", "txt");
    AddExportExtension(0, "csv", "csv");

    ClearChannels();
    AddChannel(mInputChannel, "Serial", false);
}

YamahaAbcAnalyzerSettings::~YamahaAbcAnalyzerSettings() {
}

bool YamahaAbcAnalyzerSettings::SetSettingsFromInterfaces() {
    mInputChannel = mInputChannelInterface->GetChannel();
    mBitRate = mBitRateInterface->GetInteger();

    ClearChannels();
    AddChannel(mInputChannel, "Yamaha ABC Keycodes", true);

    return true;
}

void YamahaAbcAnalyzerSettings::UpdateInterfacesFromSettings() {
    mInputChannelInterface->SetChannel(mInputChannel);
    mBitRateInterface->SetInteger(mBitRate);
}

void YamahaAbcAnalyzerSettings::LoadSettings(const char *settings) {
    SimpleArchive text_archive;
    text_archive.SetString(settings);

    text_archive >> mInputChannel;
    text_archive >> mBitRate;

    ClearChannels();
    AddChannel(mInputChannel, "Yamaha ABC Keycodes", true);

    UpdateInterfacesFromSettings();
}

const char *YamahaAbcAnalyzerSettings::SaveSettings() {
    SimpleArchive text_archive;

    text_archive << mInputChannel;
    text_archive << mBitRate;

    return SetReturnString(text_archive.GetString());
}
