#include "YamahaAbcAnalyzerResults.h"
#include "YamahaAbcAnalyzer.h"
#include "YamahaAbcAnalyzerSettings.h"
#include <iostream>
#include <sstream>
#include <iomanip>

#define REPLACE_ZEROES false

YamahaAbcAnalyzerResults::YamahaAbcAnalyzerResults(YamahaAbcAnalyzer *analyzer, YamahaAbcAnalyzerSettings *settings)
        : AnalyzerResults(),
          mSettings(settings),
          mAnalyzer(analyzer) {
}

YamahaAbcAnalyzerResults::~YamahaAbcAnalyzerResults() = default;

std::stringstream YamahaAbcAnalyzerResults::FormatResult(const Frame &frame) const {
    std::stringstream ss;

    if (frame.mFlags & YamahaAbcAnalyzer::FLAG_COMBINED_FRAMES) {
        // Combined frames, digits are split between hexStr 1 and 2

        // We will need to zero pad in case either block starts with a zero.
        char hexStr[12];

        // Pad Data 1 to 10 digits.
        sprintf(hexStr, "%llX", frame.mData1); // Hex
        ss << std::setfill('0') << std::setw(10) << hexStr;

        // Pad Data 2 to 11 digits
        sprintf(hexStr, "%llX", frame.mData2);
        ss << std::setfill('0') << std::setw(11) << hexStr;
    } else {
        char hexStr[12];
        sprintf(hexStr, "%llX", frame.mData1); // Hex
        ss << hexStr;
    }

    if (frame.mFlags & YamahaAbcAnalyzer::FLAG_LAST_IN_PACKET) {
        // Last frame of packet, add new line.
        ss << "\n";
    }

#if REPLACE_ZEROES
    std::string s = ss.str();
    std::replace(s.begin(), s.end(), '0', '_');
    std::stringstream ss2;
    ss2 << s;
    return ss2;
#else
    return ss;
#endif
}

void YamahaAbcAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel &channel, DisplayBase display_base) {
    ClearResultStrings();
    Frame frame = GetFrame(frame_index);
    std::stringstream ss = FormatResult(frame);
    AddResultString(ss.str().c_str());
}

void YamahaAbcAnalyzerResults::GenerateExportFile(const char *file, DisplayBase display_base, U32 export_type_user_id) {
    // TODO
}

void YamahaAbcAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base) {
#ifdef SUPPORTS_PROTOCOL_SEARCH
    ClearTabularText();
    Frame frame = GetFrame(frame_index);
    std::stringstream ss = FormatResult(frame);
    AddTabularText(ss.str().c_str());
#endif
}

void YamahaAbcAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base) {
#ifdef SUPPORTS_PROTOCOL_SEARCH
    ClearTabularText();
    U64 first, last;
    GetFramesContainedInPacket(packet_id, &first, &last);

    std::stringstream ss;

    for (U64 frame_id = first; frame_id <= last; frame_id++) {
        Frame frame = GetFrame(frame_id);
        ss << frame.mData1;
    }

    ss << "\n";

    AddTabularText(ss.str().c_str());
#endif
}

void YamahaAbcAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base) {
    // Stub.
}