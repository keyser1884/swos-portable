#include "SdlMappingParser.h"
#include "util.h"

SdlMappingParser::SdlMappingParser(const char *mapping) : m_mapping(mapping)
{
}

void SdlMappingParser::traverse(BindingCallback callback) const
{
    // skip name
    auto p = strchr(m_mapping, ',');
    if (!p)
        return;

    // skip GUID
    p = strchr(p + 1, ',');
    if (!p)
        return;

    while (*p) {
        assert(*p == ',');

        auto output = p + 1;
        if (!output)
            return;

        auto outputEnd = strchr(p, ':');
        if (!outputEnd)
            return;

        auto input = outputEnd + 1;
        auto inputEnd = strchr(input, ',');
        if (!inputEnd)
            return;

        parseMappingEntry(input, inputEnd - input, output, outputEnd - output, callback);
        p = inputEnd;
    }
}

void SdlMappingParser::parseMappingEntry(const char *input, int inputLen, const char *output, int outputLen, BindingCallback callback)
{
    OutputType outputType = parseOutputType(output, outputLen);

    InputType inputType = InputType::kOther;
    int index = 0;
    int hatMask = 0;
    Range range = Range::kFull;
    bool inverted = input[inputLen - 1] == '~';

    if (*input == '+' || *input == '-')
        range = *input++ == '+' ? Range::kPositiveOnly : Range::kNegativeOnly;

    if (*input == 'a' && isDigit(input[1])) {
        inputType = InputType::kAxis;
        index = SDL_atoi(input + 1);
    } else if (*input == 'b' && isDigit(input[1])) {
        inputType = InputType::kButton;
        index = SDL_atoi(input + 1);
    } else if (*input == 'h' && isDigit(input[1])) {
        index = SDL_atoi(++input);
        while (isDigit(*input))
            input++;
        if (*input == '.') {
            inputType = InputType::kHat;
            hatMask = SDL_atoi(input + 1);
        }
    }

    callback(outputType, inputType, index, hatMask, range, inverted);
}

auto SdlMappingParser::parseOutputType(const char *output, int outputLen) -> OutputType
{
    static std::pair<const char *, int> kOutputStrings[] = {
        { "a", 1, },
        { "b", 1, },
        { "start", 5, },
        { "leftx", 5, },
        { "lefty", 5, },
        { "rightx", 6, },
        { "righty", 6, },
        { "dpup", 4, },
        { "dpdown", 6, },
        { "dpleft", 6, },
        { "dpright", 7, },
    };
    static_assert(sizeofarray(kOutputStrings) == static_cast<int>(OutputType::kNumTypes), "strings out of sync with enum");

    OutputType outputType = OutputType::kOther;

    if (*output == '+' || *output == '-') {
        output++;
        outputLen--;
    }

    for (size_t i = 0; i < std::size(kOutputStrings); i++) {
        const auto& outputString = kOutputStrings[i];
        if (outputString.second == outputLen && !memcmp(output, outputString.first, outputLen)) {
            outputType = static_cast<OutputType>(i + 1);
            break;
        }
    }

    return outputType;
}
