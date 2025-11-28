#include "stm32f4xx.h"
#include <stdio.h>

// Функция инициализации USART2
void USART2_Init(void) {
    // Включаем тактирование USART2 и GPIOA
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;     // GPIOA clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;    // USART2 clock
    
    // Настраиваем PA2 (TX) и PA3 (RX) для USART2
    GPIOA->MODER   |= GPIO_MODER_MODER2_1;   // Alternate function for PA2
    GPIOA->MODER   |= GPIO_MODER_MODER3_1;   // Alternate function for PA3
    GPIOA->AFR[0]  |= (7 << GPIO_AFRL_AFSEL2_Pos); // AF7 for PA2
    GPIOA->AFR[0]  |= (7 << GPIO_AFRL_AFSEL3_Pos); // AF7 for PA3
    
    // Настройка USART2
    USART2->BRR = 0x08B; // 115200 baud rate @ 16MHz
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable TX, RX and USART
}

// Функция отправки одного символа
void USART2_SendChar(char ch) {
    while (!(USART2->SR & USART_SR_TXE)); // Ждем пока буфер передатчика освободится
    USART2->DR = (ch & 0xFF);
}

// Функция отправки строки
void USART2_SendString(char *str) {
    while (*str) {
        USART2_SendChar(*str++);
    }
}

// Функция инициализации системного таймера
void SysTick_Init(void) {
    SysTick->LOAD = 16000000 - 1; // 1 секунда при 16MHz
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk | 
                   SysTick_CTRL_ENABLE_Msk;
}

// Задержка в миллисекундах
void Delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        // Примерная задержка (можно заменить на SysTick)
        for (volatile uint32_t j = 0; j < 16000; j++);
    }
}

int main(void) {
    // Инициализация
    USART2_Init();
    SysTick_Init();
    
    // Бесконечный цикл
    while (1) {
        USART2_SendString("👋 Hello, Nikita! 🌟\n");
        Delay_ms(1000); // Задержка 1 секунда
    }
}

// Обработчик прерывания SysTick
void SysTick_Handler(void) {
    // Можно использовать для точных задержек
}