#ifndef YAMAHAABC_ANALYZER_SETTINGS
#define YAMAHAABC_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class YamahaAbcAnalyzerSettings : public AnalyzerSettings {
public:
    YamahaAbcAnalyzerSettings();

    ~YamahaAbcAnalyzerSettings() override;

    bool SetSettingsFromInterfaces() override;

    void UpdateInterfacesFromSettings();

    void LoadSettings(const char *settings) override;

    const char *SaveSettings() override;


    Channel mInputChannel;
    U32 mBitRate;

protected:
    std::unique_ptr<AnalyzerSettingInterfaceChannel> mInputChannelInterface;
    std::unique_ptr<AnalyzerSettingInterfaceInteger> mBitRateInterface;
};

#endif //YAMAHAABC_ANALYZER_SETTINGS
