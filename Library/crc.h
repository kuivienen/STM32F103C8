#pragma once

#include <stdint.h>
#include <stdbool.h>

uint16_t Crc16( uint8_t *pcBlock, uint32_t len );
uint8_t Crc8( uint8_t *pcBlock, uint32_t len);
