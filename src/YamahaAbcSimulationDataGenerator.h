#ifndef YAMAHAABC_SIMULATION_DATA_GENERATOR
#define YAMAHAABC_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
#include <AnalyzerHelpers.h>

class YamahaAbcAnalyzerSettings;

class YamahaAbcSimulationDataGenerator {
public:
    YamahaAbcSimulationDataGenerator();

    ~YamahaAbcSimulationDataGenerator();


    void Initialize(U32 simulation_sample_rate, YamahaAbcAnalyzerSettings *settings);

    U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate,
                               SimulationChannelDescriptor **simulation_channel);

protected:
    YamahaAbcAnalyzerSettings *mSettings;
    U32 mSimulationSampleRateHz;

protected:
    ClockGenerator mClockGenerator;

    void CreateKeycodeFrame();

    SimulationChannelDescriptorGroup mSimChannels;
    SimulationChannelDescriptor* mKC1Simulation;
    SimulationChannelDescriptor* mKC2Simulation;
    SimulationChannelDescriptor* mKC3Simulation;
    SimulationChannelDescriptor* mKC4Simulation;
    SimulationChannelDescriptor* mClockSimulation;

};

#endif //YAMAHAABC_SIMULATION_DATA_GENERATOR