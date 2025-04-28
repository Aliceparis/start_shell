#ifndef FT_PRINTF_H
# define FT_PRINTF_H
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

int ft_putchar_fd(char c, int fd);
int ft_putstr_fd(char *str, int fd);
int ft_putnbr_fd(unsigned int nb, int base, int fd);
int ft_print10_fd(int n, int fd);
int ft_print_pointer_fd(void *ptr, int fd);

int ft_putall_fd(char c, va_list *a, int fd);
int ft_fprintf(FILE *stream, const char *format, ...);

#endif