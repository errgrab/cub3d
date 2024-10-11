/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:02:23 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/11 00:11:04 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALLOC_H
# define ALLOC_H

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

# ifdef ALLOC_IMPL

static t__lst	**ft_get_alloc(void)
{
	static t__lst	*alloc = NULL;

	return (&alloc);
}

void	*ft_calloc(size_t num, size_t size)
{
	void *const		ptr = malloc(num * size);
	size_t			i;
	t__lst **const	root = ft_get_alloc();
	t__lst			*new_node;

	if (!ptr)
		return (NULL);
	i = num * size;
	while (i && i--)
		((char *)ptr)[i] = 0;
	new_node = (t__lst *)malloc(sizeof(t__lst));
	if (!new_node)
		return (free(ptr), NULL);
	new_node->ptr = ptr;
	new_node->next = *root;
	*root = new_node;
	return (ptr);
}

void	ft_free(void *ptr)
{
	t__lst	**node;
	t__lst	*prev;
	t__lst	*del;

	node = ft_get_alloc();
	prev = NULL;
	while (*node)
	{
		if ((*node)->ptr == ptr)
		{
			del = *node;
			if (prev)
				prev->next = (*node)->next;
			else
				*node = (*node)->next;
			free(del->ptr);
			free(del);
			return ;
		}
		prev = *node;
		node = &(*node)->next;
	}
}

void	ft_clean(void)
{
	t__lst	**node;
	t__lst	*del;

	node = ft_get_alloc();
	del = NULL;
	while (*node)
	{
		del = *node;
		*node = (*node)->next;
		free(del->ptr);
		free(del);
	}
}

# endif // ALLOC_IMPL
#endif // ALLOC_H
