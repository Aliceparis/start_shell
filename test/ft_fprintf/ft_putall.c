#include "ft_fprintf.h"

int ft_putchar_fd(char c, int fd)
{
    return (write(fd, &c, 1));
}

int ft_putstr_fd(char *str, int fd)
{
    int i;

    i = 0;
    if(str == NULL)
        str = "(null)";
    while(str[i])
    {
        ft_putchar_fd(str[i], fd);
        i++;
    }
    return (i);
}

int ft_putnbr_fd(unsigned int nb, int base, int fd)
{
    int cpt;
    char *base_s = "0123456789abcdef";

    cpt = 0;
    if (nb/base > 0)
        cpt += ft_putnbr_fd(nb/base, base, fd);
    cpt += ft_putchar_fd(base_s[nb%base], fd);
    return (cpt);
}

int ft_print10_fd(int n, int fd)
{
    long nb;
    int cpt;

    nb = n;
    cpt = 0;
    if (nb<0)
    {
        cpt += ft_putchar_fd('-', fd);
        nb = -nb;
    }
    cpt += ft_putnbr_fd(nb, 10, fd);
    return (cpt);
}


int ft_print_pointer_fd(void *ptr, int fd)
{
    int cpt = 0;

    cpt += ft_putstr_fd("0x", fd);
    cpt += ft_putnbr_fd((unsigned long)ptr, 16, fd);
    return (cpt);
}
