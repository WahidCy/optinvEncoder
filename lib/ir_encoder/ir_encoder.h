#ifndef IR_ENCODER_H
#define IR_ENCODER_H

#include "stm32f4xx_hal.h"  // Ganti sesuai seri STM32 Anda

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    uint32_t last_capture;
    float frequency;
    float rpm;
    uint16_t pulses_per_rev;
    uint32_t last_update_tick;  // timeout tracking
    uint32_t timeout_ms;        // waktu maksimum tanpa pulsa
} IR_Encoder;

void IR_Encoder_Init(IR_Encoder *encoder, TIM_HandleTypeDef *htim, uint32_t channel, uint16_t pulses_per_rev, uint32_t timeout_ms);
void IR_Encoder_Update(IR_Encoder *encoder);
void IR_Encoder_CheckTimeout(IR_Encoder *encoder);
float IR_Encoder_GetRPM(IR_Encoder *encoder);
float IR_Encoder_GetFrequency(IR_Encoder *encoder);

#endif
