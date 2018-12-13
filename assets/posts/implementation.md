# Implementation

Our implementation was based on modifying an off-the-shelf pitching machine so
that we could dynamically modify the speed and timing of the pitch using our
micro-controller. We also created a separate backstop equipped with an
accelerometer to detect whether the ball hits the backstop or not.

<span class="image main">![](assets/images/about/rotoscope.png) *Figure 1:
Diagram of our system.*</span>

## Actuators

Our team has three actuators: a stepper motor to rotate the base, a pair of
flywheels which spin to launch the ball, and a motor to release balls into the
pitching machine (see Figure 4). Our micro-controller actuates all of these motors, so we can
precisely control the speed, angle, and timing of the pitch.

## Sensors

We used two sensors: a limit switch inside the pitching machine to detect when
the ball was loaded into the machine, and an accelerometer on the backstop to detect successful
hits.

<span class="image left">![](assets/images/implementation/base_stepper.jpg) *Figure 2:
Stepper motor which drives the base.*</span>

<span class="image right">![](assets/images/implementation/flywheel.jpg) *Figure 3:
Flywheels which launch the ball.*</span>

<span class="image right">![](assets/images/implementation/accel.jpg) *Figure 5:
Accelerometer mounted on the backstop.*</span>

<span class="image left">![](assets/images/implementation/switch.jpg) *Figure 4:
Gear release the ball into the pitching machine. Limit switch in the pitching
machine which detects the presence of baseballs.*</span>
