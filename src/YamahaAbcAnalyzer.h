#ifndef YAMAHAABC_ANALYZER_H
#define YAMAHAABC_ANALYZER_H

#include <Analyzer.h>
#include "YamahaAbcAnalyzerResults.h"
#include "YamahaAbcSimulationDataGenerator.h"

class YamahaAbcAnalyzerSettings;

class ANALYZER_EXPORT YamahaAbcAnalyzer : public Analyzer2 {
public:
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

protected: //vars
    std::unique_ptr<YamahaAbcAnalyzerSettings> mSettings;
    std::unique_ptr<YamahaAbcAnalyzerResults> mResults;
    AnalyzerChannelData *mSerial;

    YamahaAbcSimulationDataGenerator mSimulationDataGenerator;
    bool mSimulationInitilized;

    //Serial analysis vars:
    U32 mSampleRateHz;
    U32 mStartOfStopBitOffset;
    U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char *__cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer *__cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer *analyzer);

#endif //YAMAHAABC_ANALYZER_H
