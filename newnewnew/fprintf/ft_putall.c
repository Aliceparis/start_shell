#include "ft_fprintf.h"

int ft_printchar_fd(char c, int fd)
{
    return (write(fd, &c, 1));
}

int ft_printstr_fd(char *str, int fd)
{
    int i;

    i = 0;
    if(str == NULL)
        str = "(null)";
    while(str[i])
    {
        ft_printchar_fd(str[i], fd);
        i++;
    }
    return (i);
}

int ft_printnbr_fd(unsigned int nb, int base, int fd)
{
    int cpt;
    char *base_s = "0123456789abcdef";

    cpt = 0;
    if (nb/base > 0)
        cpt += ft_printnbr_fd(nb/base, base, fd);
    cpt += ft_printchar_fd(base_s[nb%base], fd);
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
        cpt += ft_printchar_fd('-', fd);
        nb = -nb;
    }
    cpt += ft_printnbr_fd(nb, 10, fd);
    return (cpt);
}


int ft_print_pointer_fd(void *ptr, int fd)
{
    int cpt = 0;

    cpt += ft_printstr_fd("0x", fd);
    cpt += ft_printnbr_fd((unsigned long)ptr, 16, fd);
    return (cpt);
}