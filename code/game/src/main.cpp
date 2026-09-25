#pragma once

#include <iostream>
#include "OceanOutlaws.h"
#include "EisEngine.h"

using namespace Ocean_Outlaws;
using namespace EisEngine;

int main() {
    try {
        OceanOutlaws oceanOutlaws("Ocean Outlaws");
        oceanOutlaws.run();
    }
    catch (const std::exception &e) {
        std::string errorMessage = e.what();
        DEBUG_RUNTIME_ERROR("Unhandled exception: " + errorMessage)
    }
    return 0;
}
