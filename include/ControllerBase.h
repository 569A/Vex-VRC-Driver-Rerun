#include "main.h"

#pragma once

class ControllerBase : public pros::Controller {
public:
    
    ControllerBase(pros::controller_id_e_t id);
    virtual void runUpdate();
        
};
