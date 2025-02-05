#include "bi_int.h"

// Fn to map command names to corresponding built-ins
t_builtin_fn    get_builtin(char *command)
{
    if (NULL != command)
    {
        if (ft_strcmp(command, BI_ECHO) == 0)
            return (&bi_echo);
        if (ft_strcmp(command, BI_PWD) == 0)
            return (&bi_pwd);
        if (ft_strcmp(command, BI_EXIT) == 0)
            return (&bi_exit);
        if (ft_strcmp(command, BI_EXEC) == 0)
            return (&bi_exec);
        if (ft_strcmp(command, BI_ENV) == 0)
            return (&bi_env);
        /* WIP
        if (ft_strcmp(command, BI_CD) == 0)
            return (&builtin_cd);
        if (ft_strcmp(command, BI_UNSET) == 0)
            return (&builtin_unset);
        
        if (ft_strcmp(command, BI_EXPORT) == 0)
            return (&builtin_export);
        if (ft_strcmp(command, BI_EXIT) == 0)
            return (&builtin_exit); */
    }
    debug_print("get_builtin found nothing\n");
    return (NULL); 
}
