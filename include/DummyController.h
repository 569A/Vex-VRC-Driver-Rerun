#ifndef DUMMY_CONTROLLER_H
#define DUMMY_CONTROLLER_H
#include "main.h"
#include "ControllerBase.h"
#include "Player.h"


class DummyController : public ControllerBase {
private:
    int twentyMsecIter = 0;


    bool isPlaying = true;
public:
    Player* player;

    DummyController(pros::controller_id_e_t id, bool isPlaying, Player* player);

    int32_t get_analog(pros::controller_analog_e_t channel);

    int32_t get_digital(pros::controller_digital_e_t button);
    void setButtonPressed(bool pressed);
    void runUpdate() override;
};

#endif // DUMMY_CONTROLLER_H