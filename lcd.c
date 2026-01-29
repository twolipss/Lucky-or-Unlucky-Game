#include "stm32f4xx.h"

/* --- I2C Address for LCD --- */
#define LCD_ADDR (0x27 << 1)  // Adjust based on your module

/* --- Function Prototypes --- */
void I2C_Init(void);
void I2C_Write_Wait(uint8_t data);
void LCD_Init(void);
void LCD_SendCommand(uint8_t cmd);
void LCD_SendData(uint8_t data);
void LCD_SendString(char *str);
void LCD_Clear(void);
void LCD_SetCursor(uint8_t row, uint8_t col);
void delay(volatile uint32_t t);

/* --- I2C Initialization --- */
void I2C_Init(void) {
    RCC->AHB1ENR |= (1 << 1);      // Enable GPIOB clock
    RCC->APB1ENR |= (1 << 21);     // Enable I2C1 clock

    // Configure PB8 (SCL) and PB9 (SDA) as alternate function
    GPIOB->MODER |= (2 << 16) | (2 << 18); // AF mode
    GPIOB->OTYPER |= (1 << 8) | (1 << 9);  // Open-drain
    GPIOB->PUPDR |= (1 << 16) | (1 << 18); // Pull-up
    GPIOB->AFR[1] |= (4 << 0) | (4 << 4);  // AF4 for I2C1

    // Reset I2C1
    I2C1->CR1 |= (1 << 15);
    delay(100);
    I2C1->CR1 &= ~(1 << 15);

    I2C1->CR2 = 16;  // Peripheral clock in MHz
    I2C1->CCR = 80;  // Clock control
    I2C1->TRISE = 17; 
    I2C1->CR1 |= 1;  // Enable I2C1
}

/* --- I2C Write Byte --- */
void I2C_Write_Wait(uint8_t data) {
    I2C1->CR1 |= (1 << 8);             // Start
    while (!(I2C1->SR1 & 1));          // Wait for SB
    I2C1->DR = LCD_ADDR;               // Send address
    while (!(I2C1->SR1 & 2));          // Wait for ADDR
    (void)I2C1->SR2;                   // Clear ADDR flag
    I2C1->DR = data;                    // Send data
    while (!(I2C1->SR1 & 0x80));       // Wait for BTF
    I2C1->CR1 |= (1 << 9);             // Stop
}

/* --- LCD Commands --- */
void LCD_SendCommand(uint8_t cmd) {
    uint8_t u = (cmd & 0xF0);
    uint8_t l = (cmd << 4) & 0xF0;
    I2C_Write_Wait(u | 0x0C); delay(1000);
    I2C_Write_Wait(u | 0x08); delay(1000);
    I2C_Write_Wait(l | 0x0C); delay(1000);
    I2C_Write_Wait(l | 0x08); delay(1000);
}

void LCD_SendData(uint8_t data) {
    uint8_t u = (data & 0xF0);
    uint8_t l = (data << 4) & 0xF0;
    I2C_Write_Wait(u | 0x0D); delay(1000);
    I2C_Write_Wait(u | 0x09); delay(1000);
    I2C_Write_Wait(l | 0x0D); delay(1000);
    I2C_Write_Wait(l | 0x09); delay(1000);
}

/* --- LCD Initialization --- */
void LCD_Init(void) {
    delay(1000000);
    LCD_SendCommand(0x33);
    LCD_SendCommand(0x32);
    LCD_SendCommand(0x28); // 4-bit mode, 2 lines
    LCD_SendCommand(0x0C); // Display on, cursor off
    LCD_SendCommand(0x01); // Clear
    delay(200000);
}

/* --- LCD Helper Functions --- */
void LCD_SendString(char *str) { while (*str) LCD_SendData(*str++); }
void LCD_Clear(void) { LCD_SendCommand(0x01); delay(100000); }
void LCD_SetCursor(uint8_t row, uint8_t col) { LCD_SendCommand(row == 0 ? (0x80 + col) : (0xC0 + col)); }

/* --- Simple Delay --- */
void delay(volatile uint32_t t) { while (t--); }

/* --- Main Loop (LCD Test Only) --- */
int main(void) {
    I2C_Init();   // Initialize I2C
    LCD_Init();   // Initialize LCD

    while (1) {
        LCD_Clear();
        LCD_SetCursor(0, 0);
        LCD_SendString("HELLO PLAYER");
        LCD_SetCursor(1, 0);
        LCD_SendString("CHOOSE OPTION");
        delay(5000000);
    }
}