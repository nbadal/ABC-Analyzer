#ifndef YAMAHAABC_ANALYZER_RESULTS
#define YAMAHAABC_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class YamahaAbcAnalyzer;

class YamahaAbcAnalyzerSettings;

class YamahaAbcAnalyzerResults : public AnalyzerResults {
public:
    YamahaAbcAnalyzerResults(YamahaAbcAnalyzer *analyzer, YamahaAbcAnalyzerSettings *settings);

    ~YamahaAbcAnalyzerResults() override;


    void GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base) override;

    void GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id) override;

    void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base) override;

    void GeneratePacketTabularText(U64 packet_id, DisplayBase display_base) override;

    void GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base) override;

protected: //functions

protected:  //vars
    YamahaAbcAnalyzerSettings *mSettings;
    YamahaAbcAnalyzer *mAnalyzer;
};

#endif //YAMAHAABC_ANALYZER_RESULTS
