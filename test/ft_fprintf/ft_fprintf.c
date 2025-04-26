#include "ft_fprintf.h"

int ft_fprintf(FILE *stream, const char *format, ...)
{
    va_list a;
    int cpt;
    int i;
    int fd;
    va_start(a,format);

    cpt = 0;
    i = 0;
    if(format == NULL)
        return (0);
    fd = fileno(stream);
    while (format[i])
    {
        if(format[i] == '%' && format[i+1])
        {
            i++;
            cpt += ft_putall_fd(format[i], &a, fd);
        }
        else 
            cpt += ft_putchar_fd(format[i], fd);
        i++;
    }
    va_end(a);
    return (cpt);
}


int ft_putall_fd(char c, va_list *a, int fd)
{
    int cpt;

    cpt = 0;
    if (c == 'd')
        cpt += ft_print10_fd(va_arg(*a, int), fd);
    else if (c == 'x')
        cpt += ft_putnbr_fd(va_arg(*a, unsigned int), 16, fd);
    else if (c == 's')
        cpt += ft_putstr_fd(va_arg(*a, char *), fd);
    else if (c == 'p')
        cpt += ft_print_pointer_fd(va_arg(*a, void *), fd);
    else if (c == '%')
        cpt += ft_putchar_fd('%', fd);
    else
        cpt += ft_putchar_fd(c, fd);
    return (cpt);
}


int main(int ac, char **av)
{
    FILE *f;
    int nombre;
    char *message;

    if (ac < 3)
    {
        ft_fprintf(stdout, "Usage: %s <nombre> <message>\n", av[0]);
        return (1);
    }
    f = fopen("output.txt", "w");
    if (!f)
    {
        ft_fprintf(stderr, "Erreur: impossible d'ouvrir le fichier\n");
        return (1);
    }
    nombre = atoi(av[1]); // convertir av[1] en int
    message = av[2];       // message simple
    ft_fprintf(stdout, "Terminal -> Nombre: %d, Message: %s\n", nombre, message);
    ft_fprintf(f, "Fichier -> Nombre: %d, Message: %s\n", nombre, message);
    fclose(f);
    return (0);
}
