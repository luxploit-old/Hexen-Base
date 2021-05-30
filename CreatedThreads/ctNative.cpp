#pragma once
#include "threadtable.hpp"

/*Main Thread for Executing Natives*/
void CreatedThreads::NativeThread() {
	for (;;) {
		/*Call Graphics Shit Here ig*/
		script::get_current()->yield();
	}	
}