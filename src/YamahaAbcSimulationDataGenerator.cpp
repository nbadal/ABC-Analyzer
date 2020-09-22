#include "YamahaAbcSimulationDataGenerator.h"
#include "YamahaAbcAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

YamahaAbcSimulationDataGenerator::YamahaAbcSimulationDataGenerator() {
}

YamahaAbcSimulationDataGenerator::~YamahaAbcSimulationDataGenerator() = default;

void YamahaAbcSimulationDataGenerator::Initialize(U32 simulation_sample_rate, YamahaAbcAnalyzerSettings *settings) {
    mSimulationSampleRateHz = simulation_sample_rate;
    mSettings = settings;

    mClockGenerator.Init(470 * 1000, simulation_sample_rate); // 470 kHz

    if (settings->mKC1Channel != UNDEFINED_CHANNEL) {
        mKC1Simulation = mSimChannels.Add(settings->mKC1Channel, simulation_sample_rate, BIT_LOW);
    } else {
        mKC1Simulation = nullptr;
    }

    if (settings->mKC2Channel != UNDEFINED_CHANNEL) {
        mKC2Simulation = mSimChannels.Add(settings->mKC2Channel, simulation_sample_rate, BIT_LOW);
    } else {
        mKC2Simulation = nullptr;
    }

    if (settings->mKC3Channel != UNDEFINED_CHANNEL) {
        mKC3Simulation = mSimChannels.Add(settings->mKC3Channel, simulation_sample_rate, BIT_LOW);
    } else {
        mKC3Simulation = nullptr;
    }

    if (settings->mKC4Channel != UNDEFINED_CHANNEL) {
        mKC4Simulation = mSimChannels.Add(settings->mKC4Channel, simulation_sample_rate, BIT_LOW);
    } else {
        mKC4Simulation = nullptr;
    }

    if (settings->mClockChannel != UNDEFINED_CHANNEL) {
        mClockSimulation = mSimChannels.Add(settings->mClockChannel, simulation_sample_rate, BIT_LOW);
    } else {
        mClockSimulation = nullptr;
    }
}

U32 YamahaAbcSimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate,
                                                             SimulationChannelDescriptor **simulation_channel) {
    const U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested,
                                                                                          sample_rate,
                                                                                          mSimulationSampleRateHz);
    if (mClockSimulation->GetCurrentSampleNumber() == 0) {
        // At beginning, add one cycle of idle.
        mSimChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod());
    }

    while (mClockSimulation->GetCurrentSampleNumber() < adjusted_largest_sample_requested) {
        CreateKeycodeFrame();
    }

    *simulation_channel = mSimChannels.GetArray();
    return mSimChannels.GetCount();
}

#define STEP() mSimChannels.AdvanceAll(mClockGenerator.AdvanceByHalfPeriod())

void YamahaAbcSimulationDataGenerator::CreateKeycodeFrame() {

    // START OF FRAME:
    // All bits go high then low for one cycle.
    mKC1Simulation->TransitionIfNeeded(BIT_HIGH);
    mKC2Simulation->TransitionIfNeeded(BIT_HIGH);
    mKC3Simulation->TransitionIfNeeded(BIT_HIGH);
    mKC4Simulation->TransitionIfNeeded(BIT_HIGH);
    mClockSimulation->TransitionIfNeeded(BIT_HIGH);
    STEP();
    mKC1Simulation->Transition();
    mKC2Simulation->Transition();
    mKC3Simulation->Transition();
    mKC4Simulation->Transition();
    mClockSimulation->Transition();
    STEP();
}
