#pragma once

class SdlMappingParser
{
public:
    SdlMappingParser(const char *mapping);

    enum class InputType {
        kOther,
        kButton,
        kAxis,
        kHat,
    };
    enum class OutputType {
        kOther,
        kButtonA,
        kButtonB,
        kButtonStart,
        kAxisLeftX,
        kAxisLeftY,
        kAxisRightX,
        kAxisRightY,
        kDpadUp,
        kDpadDown,
        kDpadLeft,
        kDpadRight,
        kNumTypes = kDpadRight,
    };
    enum class Range {
        kFull,
        kPositiveOnly,
        kNegativeOnly,
    };

    using BindingCallback = std::function<void(OutputType output, InputType input, int index, int hatMask, Range range, bool inverted)>;
    void traverse(BindingCallback callback) const;

private:
    static void parseMappingEntry(const char *input, int inputLen, const char *output, int outputLen, BindingCallback callback);
    static OutputType parseOutputType(const char *output, int outputLen);

    const char *m_mapping;
};
