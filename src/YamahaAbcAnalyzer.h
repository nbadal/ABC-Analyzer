#ifndef YAMAHAABC_ANALYZER_H
#define YAMAHAABC_ANALYZER_H

#include <Analyzer.h>
#include "YamahaAbcAnalyzerResults.h"
#include "YamahaAbcSimulationDataGenerator.h"

class YamahaAbcAnalyzerSettings;

class ANALYZER_EXPORT YamahaAbcAnalyzer : public Analyzer2 {
public:

    static const U8 FLAG_LAST_IN_PACKET = 1;
    static const U8 FLAG_COMBINED_FRAMES = 2;

    YamahaAbcAnalyzer();

    ~YamahaAbcAnalyzer() override;

    void SetupResults() override;

    [[noreturn]] void WorkerThread() override;

    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate,
                               SimulationChannelDescriptor **simulation_channels) override;

    U32 GetMinimumSampleRateHz() override;

    const char *GetAnalyzerName() const override {
        return "Yamaha ABC Keycodes";
    }

    bool NeedsRerun() override;

protected:
    std::unique_ptr<YamahaAbcAnalyzerSettings> mSettings;
    std::unique_ptr<YamahaAbcAnalyzerResults> mResults;

    U64 last_value1 = -1;
    U64 last_value2 = -1;

    AnalyzerChannelData *mKC1Data;
    AnalyzerChannelData *mKC2Data;
    AnalyzerChannelData *mKC3Data;
    AnalyzerChannelData *mKC4Data;
    AnalyzerChannelData *mClockData;

    YamahaAbcSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitialized;

    //Serial analysis vars:
    U32 mSampleRateHz;
    U32 mStartOfStopBitOffset;
    U32 mEndOfStopBitOffset;

    void AdvanceToNextClock();

    U8 GetKC();

    bool IsAtStartMarker();
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //YAMAHAABC_ANALYZER_H
