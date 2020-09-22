#include "YamahaAbcAnalyzer.h"
#include "YamahaAbcAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

#include <memory>

YamahaAbcAnalyzer::YamahaAbcAnalyzer()
        : Analyzer2(),
          mSettings(new YamahaAbcAnalyzerSettings()),
          mSimulationInitilized(false) {
    SetAnalyzerSettings(mSettings.get());
}

YamahaAbcAnalyzer::~YamahaAbcAnalyzer() {
    KillThread();
}

void YamahaAbcAnalyzer::SetupResults() {
    mResults = std::make_unique<YamahaAbcAnalyzerResults>(this, mSettings.get());
    SetAnalyzerResults(mResults.get());
    mResults->AddChannelBubblesWillAppearOn(mSettings->mKC1Channel);
}

[[noreturn]] void YamahaAbcAnalyzer::WorkerThread() {
    mSampleRateHz = GetSampleRate();

    mSerial = GetAnalyzerChannelData(mSettings->mKC1Channel);

    if (mSerial->GetBitState() == BIT_LOW)
        mSerial->AdvanceToNextEdge();

    U32 samples_per_bit = mSampleRateHz / 1000;
    U32 samples_to_first_center_of_first_data_bit = U32(1.5 * double(mSampleRateHz) / double(1000));

    for (;;) {
        U8 data = 0;
        U8 mask = 1 << 7;

        mSerial->AdvanceToNextEdge(); //falling edge -- beginning of the start bit

        U64 starting_sample = mSerial->GetSampleNumber();

        mSerial->Advance(samples_to_first_center_of_first_data_bit);

        for (U32 i = 0; i < 8; i++) {
            //let's put a dot exactly where we sample this bit:
            mResults->AddMarker(mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mKC1Channel);

            if (mSerial->GetBitState() == BIT_HIGH)
                data |= mask;

            mSerial->Advance(samples_per_bit);

            mask = mask >> 1;
        }


        //we have a byte to save.
        Frame frame;
        frame.mData1 = data;
        frame.mFlags = 0;
        frame.mStartingSampleInclusive = starting_sample;
        frame.mEndingSampleInclusive = mSerial->GetSampleNumber();

        mResults->AddFrame(frame);
        mResults->CommitResults();
        ReportProgress(frame.mEndingSampleInclusive);
    }
}

bool YamahaAbcAnalyzer::NeedsRerun() {
    return false;
}

U32 YamahaAbcAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate,
                                              SimulationChannelDescriptor **simulation_channels) {
    if (!mSimulationInitilized) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate,
                                                           simulation_channels);
}

U32 YamahaAbcAnalyzer::GetMinimumSampleRateHz() {
    return 10000;
}

const char *GetAnalyzerName() {
    return "Yamaha ABC Keycodes";
}

Analyzer *CreateAnalyzer() {
    return new YamahaAbcAnalyzer();
}

void DestroyAnalyzer(Analyzer *analyzer) {
    delete analyzer;
}