/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abombard <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/06 18:17:22 by abombard          #+#    #+#             */
/*   Updated: 2017/02/06 18:17:32 by abombard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

int	ft_printf(const char *fmt, ...) __attribute__((format(printf,1,2)));
int	ft_fprintf(int fd, const char *fmt, \
								...) __attribute__((format(printf,2,3)));

#endif
