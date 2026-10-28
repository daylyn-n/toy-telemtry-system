#ifndef UART_H
#define UART_H

void usart2_init(void);
void usart2_write_char(char c);
void usart2_write_str(const char *s);

void usart1_init(void);
void usart1_write_char(char c);
void usart1_write_str(const char *s);

#endif
