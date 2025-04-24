#include "ir_encoder.h"

extern TIM_HandleTypeDef htim2;

IR_Encoder myEncoder;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
        IR_Encoder_Update(&myEncoder);
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();  // Pastikan sudah dikonfigurasi
    MX_TIM2_Init();

    IR_Encoder_Init(&myEncoder, &htim2, TIM_CHANNEL_1, 20, 300);  // 20 pulsa per rotasi, timeout 300 ms

    while (1) {
        IR_Encoder_CheckTimeout(&myEncoder);

        float rpm = IR_Encoder_GetRPM(&myEncoder);
        float freq = IR_Encoder_GetFrequency(&myEncoder);

        // Tampilkan atau kirim data
        HAL_Delay(100);
    }
}
