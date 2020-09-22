#include "YamahaAbcAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "YamahaAbcAnalyzer.h"
#include "YamahaAbcAnalyzerSettings.h"
#include <iostream>
#include <fstream>

YamahaAbcAnalyzerResults::YamahaAbcAnalyzerResults(YamahaAbcAnalyzer *analyzer, YamahaAbcAnalyzerSettings *settings)
        : AnalyzerResults(),
          mSettings(settings),
          mAnalyzer(analyzer) {
}

YamahaAbcAnalyzerResults::~YamahaAbcAnalyzerResults() = default;

void YamahaAbcAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base) {
    // TODO
}

void YamahaAbcAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id) {
    // TODO
}

void YamahaAbcAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base) {
#ifdef SUPPORTS_PROTOCOL_SEARCH
    // TODO
#endif
}

void YamahaAbcAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base) {
    // Stub.
}

void YamahaAbcAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base) {
    // Stub.
}