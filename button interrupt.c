#include "stm32f4xx.h"

/* --- Enum for Game Choices --- */
typedef enum { ROCK=0, PAPER, SCISSORS, NONE } Choice;

/* --- Global Variables --- */
volatile Choice player_choice = NONE;  // Updated by button interrupts
volatile uint8_t confirm_pressed = 0;  // Updated when confirm button is pressed

/* --- Function Prototypes --- */
void Interrupt_Init(void);

/* --- External Interrupt Initialization --- */
void Interrupt_Init(void) {
    // Enable SYSCFG clock for EXTI configuration
    RCC->APB2ENR |= (1 << 14);

    // Connect EXTI lines to GPIO ports
    // PA2 ? EXTI2 (ROCK), PA3 ? EXTI3 (PAPER)
    // PB0 ? EXTI0 (SCISSORS), PB1 ? EXTI1 (CONFIRM)
    SYSCFG->EXTICR[0] &= ~0xFFFF;
    SYSCFG->EXTICR[0] |= 0x0011; // PB0 and PB1 to EXTI0 and EXTI1

    // Enable EXTI lines 0–3
    EXTI->IMR |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);

    // Configure trigger edges
    EXTI->RTSR |= (1 << 2) | (1 << 3); // Rising edge for ROCK, PAPER
    EXTI->FTSR |= (1 << 0) | (1 << 1); // Falling edge for SCISSORS, CONFIRM

    // Enable NVIC interrupts
    NVIC_EnableIRQ(EXTI0_IRQn); // SCISSORS
    NVIC_EnableIRQ(EXTI1_IRQn); // CONFIRM
    NVIC_EnableIRQ(EXTI2_IRQn); // ROCK
    NVIC_EnableIRQ(EXTI3_IRQn); // PAPER
}

/* --- Interrupt Service Routines (ISRs) --- */
void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {
        player_choice = SCISSORS; // SCISSORS button pressed
        EXTI->PR |= (1 << 0);     // Clear pending bit
    }
}

void EXTI1_IRQHandler(void) {
    if (EXTI->PR & (1 << 1)) {
        confirm_pressed = 1;      // CONFIRM button pressed
        EXTI->PR |= (1 << 1);     // Clear pending bit
    }
}

void EXTI2_IRQHandler(void) {
    if (EXTI->PR & (1 << 2)) {
        player_choice = ROCK;     // ROCK button pressed
        EXTI->PR |= (1 << 2);     // Clear pending bit
    }
}

void EXTI3_IRQHandler(void) {
    if (EXTI->PR & (1 << 3)) {
        player_choice = PAPER;    // PAPER button pressed
        EXTI->PR |= (1 << 3);     // Clear pending bit
    }
}

/* --- Main Loop (Interrupt Test Only) --- */
int main(void) {
    Interrupt_Init(); // Initialize button interrupts

    while (1) {
        // Wait for player_choice or confirm_pressed
        // Variables updated automatically by ISR
        if (player_choice != NONE) {
            // For demonstration, could toggle LED or store choice
        }

        if (confirm_pressed) {
            // Confirm pressed — could trigger next step
            confirm_pressed = 0; // Reset for next round
        }
    }
}