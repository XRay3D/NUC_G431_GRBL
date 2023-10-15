/*
  serial.c - Low level functions for sending and recieving bytes via the serial port
  Part of Grbl

  Copyright (c) 2011-2016 Sungeun K. Jeon for Gnea Research LLC
  Copyright (c) 2009-2011 Simen Svale Skogsrud

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "grbl.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_usart.h"
#include "usart.h"

#define RX_RING_BUFFER (RX_BUFFER_SIZE + 1)
#define TX_RING_BUFFER (TX_BUFFER_SIZE + 1)

template <uint8_t Size>
struct RingBuffer /* : std::integral_constant<uint8_t, Size>*/ {
    static constexpr uint8_t size = Size;
    uint8_t buffer[Size];
    uint8_t head{};
    uint8_t tail{};
    uint8_t available() {
        const uint8_t rtail = tail; // Copy to limit multiple calls to volatile
        if(head >= rtail)
            return ((Size - 1) - (head - rtail));
        return ((rtail - head - 1));
    }
    uint8_t count() {
        const uint8_t rtail = tail; // Copy to limit multiple calls to volatile
        if(head >= rtail)
            return (head - rtail);
        return ((Size - 1) - (rtail - head));
    }
};

RingBuffer<128> serial_rx;
RingBuffer<128> serial_tx;

// uint8_t serial_rx.buffer[serial_rx.size];
// uint8_t serial_rx.head = 0;
// volatile uint8_t serial_rx.tail = 0;

// uint8_t serial_tx.buffer[serial_tx.size];
// uint8_t serial_tx.head = 0;
// volatile uint8_t serial_tx.tail = 0;

// Returns the number of bytes available in the RX serial buffer.
uint8_t serial_get_rx_buffer_available() {
    return serial_rx.available();
}

// Returns the number of bytes used in the RX serial buffer.
// NOTE: Deprecated. Not used unless classic status reports are enabled in config.h.
uint8_t serial_get_rx_buffer_count() {
    return serial_rx.count();
}

// Returns the number of bytes used in the TX serial buffer.
// NOTE: Not used except for debugging and ensuring no TX bottlenecks.
uint8_t serial_get_tx_buffer_count() {
    return serial_tx.count();
}

void serial_init() {

    LL_USART_EnableIT_RXNE(USART2);

    //    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
    //    //    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
    //    LL_DMA_ClearFlag_TC2(DMA1);
    //    LL_DMA_ClearFlag_TE2(DMA1);
    //    //    LL_DMA_ClearFlag_TC5(DMA1);
    //    //    LL_DMA_ClearFlag_TE5(DMA1);
    //    LL_USART_EnableDMAReq_RX(USART2);
    //    LL_USART_EnableDMAReq_TX(USART2);
    //    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_2);
    //    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_2);
    //    //    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
    //    //    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_5);
    //    //    LL_DMA_ClearFlag_GI5(DMA1);
    //    LL_DMA_ClearFlag_GI2(DMA1);
    //    LL_DMA_ClearFlag_TC2(DMA1);
    //    LL_DMA_ClearFlag_TE2(DMA1);
    //    LL_DMA_ConfigTransfer(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH //
    //            | LL_DMA_MODE_CIRCULAR | LL_DMA_PERIPH_NOINCREMENT | LL_DMA_MEMORY_INCREMENT | LL_DMA_PDATAALIGN_BYTE | LL_DMA_MDATAALIGN_BYTE | LL_DMA_PRIORITY_MEDIUM);
    //    //    LL_DMA_ClearFlag_TC5(DMA1);
    //    //    LL_DMA_ClearFlag_TE5(DMA1);
    //    //    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_5, LL_USART_DMA_GetRegAddr(USART2), (uint32_t)&rx_str, LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5));
    //    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_2, (uint32_t)&serial_tx.buffer, LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT), LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2));
}

// Writes one byte to the TX serial buffer. Called by main program.
void serial_write(uint8_t data) {
    //    while (!LL_USART_IsActiveFlag_TXE(USART2))
    //        continue;
    //    USART2->TDR = data;
    // #define MEM_TO_UART_CH2 DMA1, LL_DMA_CHANNEL_2

    //    LL_DMA_DisableChannel(MEM_TO_UART_CH2);
    //    LL_DMA_SetMemoryAddress(MEM_TO_UART_CH2, uint32_t(&data));
    //    LL_DMA_SetPeriphAddress(MEM_TO_UART_CH2, LL_USART_DMA_GetRegAddr(USART2, LL_USART_DMA_REG_DATA_TRANSMIT));
    //    LL_USART_EnableDMAReq_TX(USART2);

    //    const auto size_ = 1;
    //    LL_DMA_SetDataLength(MEM_TO_UART_CH2, size_);
    //    LL_DMA_EnableChannel(MEM_TO_UART_CH2);
    //    while(!LL_DMA_IsActiveFlag_TC2(DMA1)) { }
    //    LL_DMA_ClearFlag_TC2(DMA1);
    //    LL_DMA_DisableChannel(MEM_TO_UART_CH2);
    //    return;
    //    LL_USART_DisableIT_TXE(USART2);

    // Calculate next head
    uint8_t next_head = serial_tx.head + 1;
    if(next_head == serial_tx.size)
        next_head = 0;

    // Wait until there is space in the buffer
//    while(next_head == serial_tx.tail) {
//        // TODO: Restructure st_prep_buffer() calls to be executed here during a long print.
//        if(sys_rt_exec_state & EXEC::RESET) {
//            return;
//        } // Only check for abort to avoid an endless loop.
//    }

    // Store data and advance head
    serial_tx.buffer[serial_tx.head] = data;
    serial_tx.head = next_head;

    // Enable Data Register Empty Interrupt to make sure tx-streaming is running
    //    UCSR0B |= (1 << UDRIE0);
    if(data == '\n') { // && !LL_DMA_IsEnabledChannel(DMA1, LL_DMA_CHANNEL_2)
        LL_USART_EnableIT_TXE(USART2);
        // LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_2, serial_tx.count());
        // LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_2);
    }
}

// extern "C" void DMA1_Channel2_IRQHandler(void) {
//     if(LL_DMA_IsActiveFlag_TC2(DMA1)) {
//         LL_DMA_ClearFlag_TC2(DMA1);
//         DMA1_TransmitComplete();
//         LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
//         serial_tx.tail = LL_DMA_GetDataLength(DMA1, LL_DMA_CHANNEL_2);
//     } else if(LL_DMA_IsActiveFlag_TE2(DMA1)) {
//         LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_2);
//         //        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);
//     }
// }

// Data Register Empty Interrupt handler
extern "C" void ISR_SERIAL_UDRE() {
    if(!(USART2->ISR & USART_ISR_TXE))
        return;
    uint8_t tail = serial_tx.tail; // Temporary serial_tx_buffer_tail (to optimize for volatile)

    // Send a byte from the buffer
    USART2->TDR = serial_tx.buffer[tail];

    // Update tail position
    ++tail;
    if(tail == serial_tx.size)
        tail = 0;

    serial_tx.tail = tail;

    // Turn off Data Register Empty Interrupt to stop tx-streaming if this concludes the transfer
    if(tail == serial_tx.head)
        LL_USART_DisableIT_TXE(USART2);
}

// Fetches the first byte in the serial read buffer. Called by main program.
uint8_t serial_read() {
    uint8_t tail = serial_rx.tail; // Temporary serial_rx_buffer_tail (to optimize for volatile)
    if(serial_rx.head == tail) {
        return SERIAL_NO_DATA;
    } else {
        uint8_t data = serial_rx.buffer[tail];

        tail++;
        if(tail == serial_rx.size)
            tail = 0;
        serial_rx.tail = tail;

        return data;
    }
}

extern "C" void ISR_SERIAL_RX() {
    if(!(USART2->ISR & USART_ISR_RXNE))
        return;
    uint8_t data = USART2->RDR;
    uint8_t next_head;

    // Pick off realtime command characters directly from the serial stream. These characters are
    // not passed into the main buffer, but these set system state flag bits for realtime execution.
    switch(data) {
    case CMD_RESET:
        mc_reset();
        break; // Call motion control reset routine.
    case CMD_STATUS_REPORT:
        system_set_exec_state_flag(EXEC::STATUS_REPORT);
        break; // Set as true
    case CMD_CYCLE_START:
        system_set_exec_state_flag(EXEC::CYCLE_START);
        break; // Set as true
    case CMD_FEED_HOLD:
        system_set_exec_state_flag(EXEC::FEED_HOLD);
        break; // Set as true
    default:
        if(data > 0x7F) { // Real-time control characters are extended ACSII only.
            switch(data) {
            case CMD_SAFETY_DOOR:
                system_set_exec_state_flag(EXEC::SAFETY_DOOR);
                break; // Set as true
            case CMD_JOG_CANCEL:
                if(sys.state & STATE::JOG) // Block all other states from invoking motion cancel.
                    system_set_exec_state_flag(EXEC::MOTION_CANCEL);
                break;
#ifdef DEBUG
            case CMD_DEBUG_REPORT: {
                uint8_t sreg = SREG;
                cli();
                BIT_SET(sys_rt_exec_debug, EXEC_DEBUG_REPORT);
                SREG = sreg;
            } break;
#endif
            case CMD_FEED_OVR_RESET:
                system_set_exec_motion_override_flag(EXEC_FEED_OVR_RESET);
                break;
            case CMD_FEED_OVR_COARSE_PLUS:
                system_set_exec_motion_override_flag(EXEC_FEED_OVR_COARSE_PLUS);
                break;
            case CMD_FEED_OVR_COARSE_MINUS:
                system_set_exec_motion_override_flag(EXEC_FEED_OVR_COARSE_MINUS);
                break;
            case CMD_FEED_OVR_FINE_PLUS:
                system_set_exec_motion_override_flag(EXEC_FEED_OVR_FINE_PLUS);
                break;
            case CMD_FEED_OVR_FINE_MINUS:
                system_set_exec_motion_override_flag(EXEC_FEED_OVR_FINE_MINUS);
                break;
            case CMD_RAPID_OVR_RESET:
                system_set_exec_motion_override_flag(EXEC_RAPID_OVR_RESET);
                break;
            case CMD_RAPID_OVR_MEDIUM:
                system_set_exec_motion_override_flag(EXEC_RAPID_OVR_MEDIUM);
                break;
            case CMD_RAPID_OVR_LOW:
                system_set_exec_motion_override_flag(EXEC_RAPID_OVR_LOW);
                break;
            case CMD_SPINDLE_OVR_RESET:
                system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_RESET);
                break;
            case CMD_SPINDLE_OVR_COARSE_PLUS:
                system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_COARSE_PLUS);
                break;
            case CMD_SPINDLE_OVR_COARSE_MINUS:
                system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_COARSE_MINUS);
                break;
            case CMD_SPINDLE_OVR_FINE_PLUS:
                system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_FINE_PLUS);
                break;
            case CMD_SPINDLE_OVR_FINE_MINUS:
                system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_FINE_MINUS);
                break;
            case CMD_SPINDLE_OVR_STOP:
                system_set_exec_accessory_override_flag(EXEC_SPINDLE_OVR_STOP);
                break;
            case CMD_COOLANT_FLOOD_OVR_TOGGLE:
                system_set_exec_accessory_override_flag(EXEC_COOLANT_FLOOD_OVR_TOGGLE);
                break;
#ifdef ENABLE_M7
            case CMD_COOLANT_MIST_OVR_TOGGLE:
                system_set_exec_accessory_override_flag(EXEC_COOLANT_MIST_OVR_TOGGLE);
                break;
#endif
            }
            // Throw away any unfound extended-ASCII character by not passing it to the serial buffer.
        } else { // Write character to buffer
            next_head = serial_rx.head + 1;
            if(next_head == serial_rx.size)
                next_head = 0;

            // Write data to buffer unless it is full.
            if(next_head != serial_rx.tail) {
                serial_rx.buffer[serial_rx.head] = data;
                serial_rx.head = next_head;
            }
        }
    }
}

void serial_reset_read_buffer() {
    serial_rx.tail = serial_rx.head;
}
