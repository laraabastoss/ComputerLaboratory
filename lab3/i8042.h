#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define KEYBOARD_IRQ_LINE    1
#define READ_KEYBOARD_STATUS 0x64
#define INPUT_BUFFER_KEYBOARD 0x64
#define OUTPUT_BUFFER_KEYBOARD 0x60
#define BREAK_ESC       0x81
#define BREAK_CODE       BIT(7)
#define TWO_BYTES       0xE0
#define ENABLE_INT      BIT(0)
#define TIMEOUT_ERROR   BIT(6)
#define PARITY_ERROR    BIT(7)
#define OUT_BUFFER_FULL BIT(0)
#define IN_BUFFER_FULL  BIT(1)
#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60
#define DELAY_US        20000

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

int (keyboard_unsubscribe_interrupts)();

int (read_KBC_status)(uint8_t* status);

int (read_KBC_output)(uint8_t port, uint8_t *output);

void (kbc_ih)();

int (enable_interrupts)();


#endif
