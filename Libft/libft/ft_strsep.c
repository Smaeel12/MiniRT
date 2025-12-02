/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsep.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iboubkri <iboubkri@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 17:08:50 by iboubkri          #+#    #+#             */
/*   Updated: 2025/11/27 17:10:23 by iboubkri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char *ft_strsep(char **stringp, const char delim)
{
    char *occur;
    int i;

    i = 0;
    if (!stringp)
        return NULL;
    while ((*stringp) && (*stringp)[i] && (*stringp)[i] != delim)
        i++;
    occur = (*stringp);
    if (occur && !*occur)
        return NULL;
    while ((*stringp) && (*stringp)[i] && (*stringp)[i] == delim)
    {
        (*stringp)[i] = '\0';
        i++;
    }
    *stringp = (*stringp) + i;
    return (occur);
}
