#include "YamahaAbcAnalyzer.h"
#include "YamahaAbcAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

#define SKIP_DUPES true
#define SKIP_INIT4 true
#define COMBINE_FRAMES true
#define INCLUDE_START_FRAME true

#if INCLUDE_START_FRAME
    #define FRAME_MAX 22
#else
    #define FRAME_MAX 21
#endif

YamahaAbcAnalyzer::YamahaAbcAnalyzer()
        : Analyzer2(),
          mSimulationInitialized(false) {
    mSettings = std::make_unique<YamahaAbcAnalyzerSettings>();
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

    mKC1Data = GetAnalyzerChannelData(mSettings->mKC1Channel);
    mKC2Data = GetAnalyzerChannelData(mSettings->mKC2Channel);
    mKC3Data = GetAnalyzerChannelData(mSettings->mKC3Channel);
    mKC4Data = GetAnalyzerChannelData(mSettings->mKC4Channel);
    mClockData = GetAnalyzerChannelData(mSettings->mClockChannel);

    // Skip to first start marker:
    while (!IsAtStartMarker()) {
        AdvanceToNextClock();
    }

    U16 dupeCount = 0;
    // Analyze Loop
    while (true) {
        U64 packet_start = mClockData->GetSampleNumber();
        mResults->AddMarker(packet_start, AnalyzerResults::Dot, mSettings->mClockChannel);

        // Get all frames.
        Frame frames[FRAME_MAX] = {};
        U64 framesVal1 = 0;
        U64 framesVal2 = 0;
        U8 frameCount = 0;
        bool skipPacket = false;

        // Move off of start bit.
        AdvanceToNextClock();

#if INCLUDE_START_FRAME
        Frame frame;
        frame.mData1 = 15;
        frame.mFlags = 0;
        frame.mStartingSampleInclusive = packet_start;
        frame.mEndingSampleInclusive = mClockData->GetSampleNumber();
        frames[frameCount++] = frame;
#endif


        do {
            // Sample.
            const U64 start_sample = mClockData->GetSampleNumber() - 1;
            const U8 data = GetKC();

#if SKIP_INIT4
            // On the first frame, skip it if it has a 4 value.
            if (frameCount == 0) {
                skipPacket = (data & 4u);
            }
#endif

            // Advance.
            AdvanceToNextClock();

            // Flags
            U8 flags = 0;
            if (IsAtStartMarker()) flags |= FLAG_LAST_IN_PACKET;

            // Mark frame.
            if (frameCount < FRAME_MAX) {
                Frame frame;
                frame.mData1 = data;
                frame.mFlags = flags;
                frame.mStartingSampleInclusive = start_sample;
                frame.mEndingSampleInclusive = mClockData->GetSampleNumber() - 1;
                frames[frameCount] = frame;

                // Put first half into val1
                if (frameCount < 10) {
                    framesVal1 <<= 4u;
                    framesVal1 += data;
                } else {
                    framesVal2 <<= 4u;
                    framesVal2 += data;
                }
            }

            frameCount++;
        } while (!IsAtStartMarker());

        if (skipPacket) {
            continue;
        }

#if SKIP_DUPES
        if (framesVal1 == last_value1 && framesVal2 == last_value2) {
            // Skip reporting duplicate event.
            dupeCount++;
            continue;
        }
#endif


        if (frameCount == FRAME_MAX) {
            last_value1 = framesVal1;
            last_value2 = framesVal2;

#if COMBINE_FRAMES
            Frame frame;
            frame.mData1 = framesVal1;
            frame.mData2 = framesVal2;
            frame.mFlags = FLAG_COMBINED_FRAMES | FLAG_LAST_IN_PACKET;
            frame.mStartingSampleInclusive = frames[0].mStartingSampleInclusive;
            frame.mEndingSampleInclusive = frames[FRAME_MAX - 1].mEndingSampleInclusive;

            mResults->AddFrame(frame);
            mResults->CommitResults();
            ReportProgress(frame.mEndingSampleInclusive);
#else
            for (const auto& frame : frames) {
                mResults->AddFrame(frame);
                mResults->CommitResults();
                ReportProgress(frame.mEndingSampleInclusive);
            }
            mResults->CommitPacketAndStartNewPacket();
#endif
        } else {
            // Not enough bits! must have encountered a sampling error.
            mResults->CancelPacketAndStartNewPacket();
        }
        dupeCount = 0;
    }
}

bool YamahaAbcAnalyzer::IsAtStartMarker() { return GetKC() == 0b1111u; }

void YamahaAbcAnalyzer::AdvanceToNextClock() {
    // Advance to next Rising Edge
    if (mClockData->GetBitState() == BIT_HIGH) {
        mClockData->AdvanceToNextEdge();
    }
    mClockData->AdvanceToNextEdge();

    // Advance other channels.
    U64 clockSample = mClockData->GetSampleNumber();
    mKC1Data->AdvanceToAbsPosition(clockSample);
    mKC2Data->AdvanceToAbsPosition(clockSample);
    mKC3Data->AdvanceToAbsPosition(clockSample);
    mKC4Data->AdvanceToAbsPosition(clockSample);
}

U8 YamahaAbcAnalyzer::GetKC() {
    U8 kc = 0;
    if (mKC1Data->GetBitState() == BIT_HIGH) kc |= 1u << 0u;
    if (mKC2Data->GetBitState() == BIT_HIGH) kc |= 1u << 1u;
    if (mKC3Data->GetBitState() == BIT_HIGH) kc |= 1u << 2u;
    if (mKC4Data->GetBitState() == BIT_HIGH) kc |= 1u << 3u;
    return kc;
}

bool YamahaAbcAnalyzer::NeedsRerun() {
    return false;
}

U32 YamahaAbcAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate,
                                              SimulationChannelDescriptor **simulation_channels) {
    if (!mSimulationInitialized) {
        mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
        mSimulationInitialized = true;
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