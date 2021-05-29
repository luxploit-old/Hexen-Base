#pragma once
#include "threadtable.hpp"

/*Main Thread for Executing Natives*/
void CreatedThreads::NativeThread() {

	ClassPointers::cHelpers->gCurrentScript->Wait(0);
}