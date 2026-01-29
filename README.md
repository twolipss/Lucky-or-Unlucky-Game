# Win-or-Lose-Game
**Project Description**

This project implements a Rock-Paper-Scissors game using the STM32 microcontroller. The player selects Rock, Paper, or Scissors using push buttons, and the system generates a random move as its choice. The game provides real-time visual feedback through LEDs and the LCD display, while a buzzer provides audio feedback for win, loss, or draw outcomes. The project demonstrates the integration of multiple input and output devices with an embedded system and showcases interrupt-based programming and modular C code design.

**Hardware Requirements**

The project requires the following hardware components:
* STM32F446 development board
* Push buttons connected to PA2 (Rock), PA3 (Paper), PB0 (Scissors), and PB1 (Confirmation)
* LEDs connected to PA8–PA11 for player feedback and PB12–PB15 for system feedback
* Buzzer connected to PD2
* I2C LCD display for showing player choice, system choice, and game result
* Jumper wires and a breadboard for connections

**Software Requirements**

To develop and run this project, the following software is required:
* Keil uVision IDE for compiling and programming the STM32
* STM32CubeMX (optional, if used for configuration)
* Basic knowledge of the C programming language

**Project Features**

This project includes the following features:
* Players can select Rock, Paper, or Scissors using push buttons.
* Real-time visual feedback is provided using LEDs to indicate selections and results.
* A buzzer provides audio feedback: five short beeps for a player win, one long beep for a player loss, and an optional sound for a draw.
* LCD display shows the player’s selection, system’s choice, and the result of the game.
* Interrupt-based input handling ensures responsive button operation.
* Modular C code structure with subroutines for display, buzzer, and game logic.

**How to Run the Project**
1. Open the project in Keil uVision IDE.
2. Compile the code and flash it to the STM32F446 development board.
3. Power on the board; the three player-choice LEDs (PA8, PA9, PA10) will light up, indicating the system is ready.
4. Use the selection buttons to choose Rock, Paper, or Scissors. The corresponding LED will remain on to confirm the selection.
5. Press the confirmation button (PB1); the PA11 LED will light to indicate that the choice is locked.
6. The system will randomly generate its choice, lighting the corresponding system LED (PB12, PB13, or PB14).
7. The game result will be determined and indicated using LEDs and the buzzer:
   * Draw: PA11 and PB15 light up simultaneously.
   * Player wins: PA11 lights up while PB15 is off; the buzzer emits five short beeps.
   * System wins: PB15 lights up while PA11 is off; the buzzer emits one long beep.
8. After a short delay, the system resets all LEDs and waits for the next round.

**Game Logic**

The outcome of each round is determined based on standard Rock-Paper-Scissors rules:
* Rock beats Scissors.
* Scissors beats Paper.
* Paper beats Rock.
  The LED and buzzer feedback provide immediate indication of the winner, loser, or a draw.

**Contributors**

This project was developed by the following team members:
* NUR HAMIZAH (LED&GPIO)
* ALIFAH ILYANA (LCD)
* UMMI KHADIJAH (BUZZER)
* ZETTY NABILA (BUTTON &EXTI)


