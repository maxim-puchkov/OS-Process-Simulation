//
//  main.c
//  assignment3
//
//  Created by admin on 2018-03-09.
//  Copyright © 2018 Maxim Puchkov. All rights reserved.
//

#include "Simulation.h"

int main(int argc, const char **argv) {
    
    // User help
    SimulationDisplayCommandList();
    
    // Prepare to start
    SimulationBegin();
    
    // Continuously read commands
    while (SimulationIsRunning()) SimulationReadCommand();
    
    // Finish
    SimulationTerminate();
    
    return 0;
    
}
