#include "bi_int.h"

// Fn to map command names to corresponding built-ins
t_builtin_fn    get_builtin(char *command)
{
    if (ft_strcmp(command, BI_ECHO) == 0)
        return (&bi_echo);
    if (ft_strcmp(command, BI_PWD) == 0)
        return (&bi_pwd);
    //if (ft_strcmp(command, BI_ENV) == 0)
    //    return (&bi_env);
    //if (ft_strcmp(command, BI_EXIT) == 0)
    //    return (&bi_exit);
    /* WIP
    if (ft_strcmp(command, BI_CD) == 0)
        return (&builtin_cd);
    if (ft_strcmp(command, BI_UNSET) == 0)
        return (&builtin_unset);
    
    if (ft_strcmp(command, BI_EXPORT) == 0)
        return (&builtin_export);
    if (ft_strcmp(command, BI_EXIT) == 0)
        return (&builtin_exit); */
    return (NULL); 
}
