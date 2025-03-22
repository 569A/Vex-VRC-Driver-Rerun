# Vex-VRC-Driver-Rerun
Early season code for 2024-25 High Stakes 

The reasoning behind this was that I would have very little time to test match autons because our bot wasn't done yet. In this game, however, auton was very important. The code produced is an overengineered solution which literally allows an auton in any corner of the field to be created in less than a minute.

This code was run at our first two competitions.

### How it works:

The driver selected what auton (corner) to record for, then clicked a button to start recording. The code sampled controller input. It stopped recording when this button was pressed again. All the controller values at every 20 millisecond interval in time were serialized and saved onto the SD card of the brain. To run it back, the autonomous function would run a clone of the driver loop, except the controller was a dummy controller that reported the stored values every 20 milliseconds instead of real controller data. Recordings could also be split into stages if we wanted to run handwritten code in between. Steps were also taken to maximize accuracy including slew rate when recording a driven route. (This was unnecessarily complicated 😞)

### What we learned:
  - Driver rerun itself was decent for making simple autons extremely quickly - the autons were "coded" right before our comp and I think only missed once in all our matches with it. It only scored a single ring at first, but it was really consistent. This was later upgraded to a 2 ring + ladder touch auton the day of comp on the practice field.
  - However, it is just impractical when things like PID and odometry exist. This system does not account for error, so differences in battery voltage or friction could break the auton completely. Autonomous movement is also more efficient than driving.

This was fun to make, but I definitely won't run this ever again.
