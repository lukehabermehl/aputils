//
//  aputils.cpp
//  libaputils
//
//  Created by Luke on 3/10/18.
//  Copyright © 2018 Luke Habermehl. All rights reserved.
//

#include "aputils.h"
#include "apu_logger.hpp"
#include "apu_thread_manager.hpp"

void APULibShutdown()
{
    APUGetLogger()->shutdown();
    APUThreadManager::sharedThreadManager()->shutdown();
}
