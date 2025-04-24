#include "ir_encoder.h"

void IR_Encoder_Init(IR_Encoder *encoder, TIM_HandleTypeDef *htim, uint32_t channel, uint16_t pulses_per_rev, uint32_t timeout_ms) {
    encoder->htim = htim;
    encoder->channel = channel;
    encoder->last_capture = 0;
    encoder->frequency = 0.0f;
    encoder->rpm = 0.0f;
    encoder->pulses_per_rev = pulses_per_rev;
    encoder->last_update_tick = HAL_GetTick();
    encoder->timeout_ms = timeout_ms;

    HAL_TIM_IC_Start_IT(htim, channel);
}

void IR_Encoder_Update(IR_Encoder *encoder) {
    uint32_t ic_val = HAL_TIM_ReadCapturedValue(encoder->htim, encoder->channel);
    uint32_t diff;

    if (ic_val >= encoder->last_capture) {
        diff = ic_val - encoder->last_capture;
    } else {
        diff = (encoder->htim->Instance->ARR - encoder->last_capture) + ic_val;
    }

    encoder->last_capture = ic_val;

    // Optional: Ignore very short pulses (anti-noise)
    if (diff < 20) return;

    float timer_clk = HAL_RCC_GetPCLK1Freq();
#if defined(STM32F4xx)
    if ((RCC->CFGR & RCC_CFGR_PPRE1) != RCC_CFGR_PPRE1_DIV1) {
        timer_clk *= 2;
    }
#endif
    uint32_t prescaler = encoder->htim->Instance->PSC + 1;
    float time_period = (float)(diff * prescaler) / timer_clk;

    if (time_period > 0) {
        encoder->frequency = 1.0f / time_period;
        encoder->rpm = (encoder->frequency * 60.0f) / encoder->pulses_per_rev;
        encoder->last_update_tick = HAL_GetTick();
    }
}

void IR_Encoder_CheckTimeout(IR_Encoder *encoder) {
    if ((HAL_GetTick() - encoder->last_update_tick) > encoder->timeout_ms) {
        encoder->rpm = 0;
        encoder->frequency = 0;
    }
}

float IR_Encoder_GetRPM(IR_Encoder *encoder) {
    return encoder->rpm;
}

float IR_Encoder_GetFrequency(IR_Encoder *encoder) {
    return encoder->frequency;
}
