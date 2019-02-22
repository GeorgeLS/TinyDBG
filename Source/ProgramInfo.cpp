//
// Created by gliontos on 2/22/19.
//

#include "../Include/ProgramInfo.h"

TinyDBG::ProgramInfo::ProgramInfo(TinyDBG::ProgramInfo &&rValue) noexcept { std::move(rValue); }