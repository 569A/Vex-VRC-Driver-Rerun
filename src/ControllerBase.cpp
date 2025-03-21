
/**
 * @file ControllerBase.cpp
 * @brief This file contains the implementation of the ControllerBase class.
 * 
 * The ControllerBase class is a controller for the robot.
 * It is designed to be a base class for all controllers,
 * and by default is works as a physical controller.
 * 
 * DummyController.cpp is a subclass of this and replaces the logic for seeing real
 * controller input with simulated input.
*/
#include "ControllerBase.h"
#include "main.h"

ControllerBase::ControllerBase(pros::controller_id_e_t id) : pros::Controller(id) {
    // Initialize the controller
}

void ControllerBase::runUpdate() {
    
}
