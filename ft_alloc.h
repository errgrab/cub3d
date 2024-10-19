/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alloc.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:02:23 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/19 16:23:55 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_ALLOC_H
# define FT_ALLOC_H

# include <stdlib.h>

/*
 * The custom allocator for this project was created for preventing memory
 * leaks. It uses a simple singleton `ft_get_alloc()` to get the pointer to the
 * root of a linked list. Each calloc allocates a new node in the linked list,
 * the custom free only frees a pointer allocated by the custom allocator. The
 * `ft_clean()` function clean all memory allocated by the custom allocator.
 * This ensures that all allocations can be dealocated by the end of the
 * program, just by calling `ft_clean()` on the end of it.
 */

void	*ft_calloc(size_t n, size_t s);
void	ft_free(void *ptr);
void	ft_clean(void);

typedef struct s__lst	t__lst;
struct s__lst
{
	void	*ptr;
	t__lst	*next;
};

#endif // FT_ALLOC_H
