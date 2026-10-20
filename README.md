2WD Robot learning and development project

Summary:
- Written in C
- No HAL usage, bare-metal throughout

Components:
- 1x STM32 NUCLEO-F446RE
- 1x TB6612FNG Dual Motor Driver
- 2x FIT0450 TT Motor with Encoder

To-do:
- [Done] Setup UART for debugging and control
- [Done] Init RCC, GPIO, TIM, SysTick, UART
- [Done] Solder breakouts onto the motor driver
- [Done] Wire and write motor drivers
- [In-progress] Wire and write encoder drivers
- [In-progress] Implement PID controller
- Mounting, power supply, buck converter
- Wiring diagrams and tuning

Prior prerequisite learning, custom drivers:
- GPIO output
- Timers, polling and interrupt, clock tree, NVIC
- PWM
- UART RX, TX, polling and interrupt
- I2C
- SysTick for delay
