#include "../include/libft.h"

/* STRSCPY
** Copy a C-string into a sized buffer
** Returns # of chars copied (excl NULL) (E2BIG if buffer too small)
** Undefined behavior if buffers overlap
** Dest is always NULL terminated unless zero-sized
** Dest[0] MUST be initialized to 0
** Better than strlcpy since mem beyond count bytes is untouched, and
** sensible return value
** Superior to strncpy since no excessive NULL termination, always
** returns C-string
** Must be able to access src and dest within count limit
*/
size_t	ft_strscpy(char *dest, const char *src, size_t size)
{
	size_t	len;

	if (size <= 0 || *dest != 0)
		return (-E2BIG);
	len = ft_strnlen(src, size - 1);
	ft_memcpy(dest, src, len);
	dest[len] = '\0';
	return (len);
}
