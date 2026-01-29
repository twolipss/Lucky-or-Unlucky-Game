#include "stm32f4xx.h"

/* --- Enum for Game Choices --- */
typedef enum { ROCK=0, PAPER, SCISSORS, NONE } Choice;

/* --- Global Variables --- */
volatile Choice player_choice = NONE;

/* --- Function Prototypes --- */
void GPIO_Init(void);
void show_player_choice(Choice c);
void show_system_choice(Choice c);
void delay(volatile uint32_t t);

/* --- GPIO Initialization --- */
void GPIO_Init(void) {
    // Enable clocks for GPIOA (player LEDs + buttons) and GPIOB (system LEDs)
    RCC->AHB1ENR |= (1 << 0) | (1 << 1);

    // Player LEDs PA8-PA10 (active low) and PA11 (draw indicator)
    GPIOA->MODER |= (1 << 16) | (1 << 18) | (1 << 20) | (1 << 22);

    // System LEDs PB12-PB14 (active high)
    GPIOB->MODER |= (1 << 24) | (1 << 26) | (1 << 28);

    // Buttons PA2, PA3 and PB0, PB1 as input (not used in this simplified version)
    GPIOA->MODER &= ~((3 << 4) | (3 << 6));
    GPIOB->MODER &= ~((3 << 0) | (3 << 2));

    // Configure pull-ups/pull-downs for inputs (default)
    GPIOA->PUPDR |= ((2 << 4) | (2 << 6));  // Pull-down
    GPIOB->PUPDR |= ((1 << 0) | (1 << 2));  // Pull-up
}

/* --- Display Player Choice on LEDs --- */
void show_player_choice(Choice c) {
    // Turn off all player LEDs first (active low)
    GPIOA->ODR |= (1 << 8) | (1 << 9) | (1 << 10);

    // Turn on LED corresponding to player's choice
    if (c == ROCK) GPIOA->ODR &= ~(1 << 8);        // Rock LED ON
    else if (c == PAPER) GPIOA->ODR &= ~(1 << 9);  // Paper LED ON
    else if (c == SCISSORS) GPIOA->ODR &= ~(1 << 10); // Scissors LED ON
}

/* --- Display System Choice on LEDs --- */
void show_system_choice(Choice c) {
    // Turn off all system LEDs first (active high)
    GPIOB->ODR &= ~((1 << 12) | (1 << 13) | (1 << 14));

    // Turn on LED corresponding to system's choice
    if (c == ROCK) GPIOB->ODR |= (1 << 12);       // Rock LED ON
    else if (c == PAPER) GPIOB->ODR |= (1 << 13); // Paper LED ON
    else if (c == SCISSORS) GPIOB->ODR |= (1 << 14); // Scissors LED ON
}

/* --- Simple Delay --- */
void delay(volatile uint32_t t) { while (t--); }

/* --- Main Loop (LED Test for GitHub Contribution) --- */
int main(void) {
    GPIO_Init(); // Initialize GPIOs and LEDs

    while (1) {
        // --- Test Player LEDs ---
        player_choice = ROCK;
        show_player_choice(player_choice);
        delay(5000000);

        player_choice = PAPER;
        show_player_choice(player_choice);
        delay(5000000);

        player_choice = SCISSORS;
        show_player_choice(player_choice);
        delay(5000000);

        // --- Test System LEDs ---
        show_system_choice(ROCK);
        delay(5000000);

        show_system_choice(PAPER);
        delay(5000000);

        show_system_choice(SCISSORS);
        delay(5000000);
    }
}