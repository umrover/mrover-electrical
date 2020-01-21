




void pwmOut(uint32_t freq, uint32_t duty)
{
    uint16_t auto_reload = 72000000 / freq;
    uint32_t mask_out = 0xFEFFFFFF;
    uint32_t *TIM8 = 0x40013400;
    uint32_t *TIM8_CCMR1 = 0x40013418;
    uint16_t *TIM8_CR1 = 0x40013400;
    uint16_t *TIM8_EGR = 0x40013414;
    uint16_t *TIM8_ARR = 0x4001342C;
    uint16_t *TIM8_CCRx = 0x40013434;
    *TIM8_CCMR1 = *TIM8_CCMR1 & mask_out;
    *TIM8_CCMR1 = *TIM8_CCMR1 | 0x00006800;
    *TIM8_CR1 = *TIM8_CR1 | 0x0080;
    *TIM8_EGR = *TIM8_EGR | 0x0001;
    *TIM8_ARR = auto_reload;
    for(int i = 0; i < 4; i++)
    {
        *TIM8_CCRx = (auto_reload / (duty / 100));
        TIM8_CCRx =  TIM8_CCRx + 4;
    }
    return;

}
