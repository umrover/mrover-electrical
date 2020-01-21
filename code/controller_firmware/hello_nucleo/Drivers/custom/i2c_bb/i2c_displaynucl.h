#include <stdint.h>
#include "main.h"



void writeEncoder_LCD(UART_HandleTypeDef * huart);
void updateEncoder_LCD(UART_HandleTypeDef * huart, uint16_t data_in);
