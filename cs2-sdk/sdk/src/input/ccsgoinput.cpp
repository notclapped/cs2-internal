#include "pch.hpp"

#include <input/ccsgoinput.hpp>

#include <signatures/signatures.hpp>

#include <memory/memory.hpp>
#include <constants/constants.hpp>

CCSGOInput* CCSGOInput::Get() {
    static auto inst = signatures::GetCSGOInput.GetPtrAs<CCSGOInput*>();
    return inst;
}
