#ifndef YAMAHAABC_ANALYZER_SETTINGS
#define YAMAHAABC_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class YamahaAbcAnalyzerSettings : public AnalyzerSettings {
public:
    YamahaAbcAnalyzerSettings();

    ~YamahaAbcAnalyzerSettings() override;


    bool SetSettingsFromInterfaces() override;

    void LoadSettings(const char *settings) override;

    const char *SaveSettings() override;

    Channel mKC1Channel;
    Channel mKC2Channel;
    Channel mKC3Channel;
    Channel mKC4Channel;
    Channel mClockChannel;

protected:
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mKC1ChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mKC2ChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mKC3ChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mKC4ChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mClockChannelInterface;
};

#endif //YAMAHAABC_ANALYZER_SETTINGS
