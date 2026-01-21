#pragma once

bool showContinueAbortPrompt(const char *header, const char *continueText, const char *abortText,
    const std::vector<const char *>& messageLines, bool abortDefault = true);
