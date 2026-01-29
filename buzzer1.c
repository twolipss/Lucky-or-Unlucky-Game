#include "stm32f4xx.h"

/* --- Pin Definitions --- */
#define BUZZER_PIN 2       // PD2
#define BUZZER_PORT GPIOD

#define LED_WIN    11      // PA11 (Active Low)
#define LED_LOSE   15      // PB15 (Active High)

/* --- Function Prototypes --- */
void hardware_init(void);
void buzzer_update(void);
void delay(volatile uint32_t t);

/* --- Main Entry Point --- */
int main(void) {
    hardware_init(); // Set up all GPIO clocks and modes

    while (1) {
        buzzer_update();
        delay(100000); // Small debounce/stability delay
    }
}

/* --- Hardware Initialization --- */
void hardware_init(void) {
    // 1. Enable Clocks for GPIOA, GPIOB, and GPIOD
    RCC->AHB1ENR |= (1 << 0); // GPIOA EN
    RCC->AHB1ENR |= (1 << 1); // GPIOB EN
    RCC->AHB1ENR |= (1 << 3); // GPIOD EN (Buzzer)

    // 2. Set PD2 (Buzzer) as Output
    BUZZER_PORT->MODER &= ~(3 << (BUZZER_PIN * 2)); // Clear bits
    BUZZER_PORT->MODER |=  (1 << (BUZZER_PIN * 2)); // Set as General Purpose Output

    // 3. Configure LED pins as Inputs (to read their state)
    // PA11 and PB15 default to input mode (00) on reset, 
    // but we ensure it here for safety.
    GPIOA->MODER &= ~(3 << (LED_WIN * 2));
    GPIOB->MODER &= ~(3 << (LED_LOSE * 2));
}

/* --- Update Buzzer Based on LED States --- */
void buzzer_update(void) {
    // Read Input Data Register (IDR) to check LED status
    // win_on: PA11 is Active Low (0 = On)
    uint8_t win_on  = !(GPIOA->IDR & (1 << LED_WIN));   
    // lose_on: PB15 is Active High (1 = On)
    uint8_t lose_on =  (GPIOB->IDR & (1 << LED_LOSE));  

    if (win_on && !lose_on) {
        // Winner Melody: 3 short beeps
        for (int i = 0; i < 3; i++) {
            BUZZER_PORT->ODR |= (1 << BUZZER_PIN);
            delay(180000);
            BUZZER_PORT->ODR &= ~(1 << BUZZER_PIN);
            delay(120000);
        }
    }
    else if (!win_on && lose_on) {
        // Loser Tone: 1 long buzz
        BUZZER_PORT->ODR |= (1 << BUZZER_PIN);
        delay(5000000); // Adjusted for typical CPU clock
        BUZZER_PORT->ODR &= ~(1 << BUZZER_PIN);
    }
    else if (win_on && lose_on) {
        // Draw: 1 short beep
        BUZZER_PORT->ODR |= (1 << BUZZER_PIN);
        delay(400000);
        BUZZER_PORT->ODR &= ~(1 << BUZZER_PIN);
    }
}

/* --- Simple Delay --- */
void delay(volatile uint32_t t) {
    while (t--);
}