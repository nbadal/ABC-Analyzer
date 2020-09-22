#ifndef YAMAHAABC_SIMULATION_DATA_GENERATOR
#define YAMAHAABC_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>

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
    void CreateSerialByte();

    std::string mSerialText;
    U32 mStringIndex;

    SimulationChannelDescriptor mSerialSimulationData;

};

#endif //YAMAHAABC_SIMULATION_DATA_GENERATOR