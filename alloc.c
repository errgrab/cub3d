/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:02:23 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/01 17:23:37 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "cub3d.h"

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

static t_list	**ft_get_alloc(void)
{
	static t_list	*alloc;

	return (&alloc);
}

void	*ft_calloc(size_t num, size_t size)
{
	auto void *ptr = malloc(num * size);
	if (ptr)
	{
		auto t_list **root = ft_get_alloc();
		auto size_t i = num * size;
		while (i && i--)
			((char *)ptr)[i] = 0;
		auto t_list * new_node = (t_list *)malloc(sizeof(t_list));
		if (new_node)
		{
			new_node->ptr = ptr;
			new_node->next = *root;
			*root = new_node;
		}
		else
			return (free(ptr), NULL);
	}
	return (ptr);
}

void	ft_free(void *ptr)
{
	auto t_list **node = ft_get_alloc();
	auto t_list * prev = NULL;
	while (*node)
	{
		if ((*node)->ptr == ptr)
		{
			auto t_list * to_free = *node;
			if (prev)
				prev->next = (*node)->next;
			else
				*node = (*node)->next;
			free(to_free->ptr);
			free(to_free);
			return ;
		}
		prev = *node;
		node = &(*node)->next;
	}
}

void	ft_clean(void)
{
	auto t_list **node = ft_get_alloc();
	while (*node)
	{
		auto t_list * to_free = *node;
		*node = (*node)->next;
		free(to_free->ptr);
		free(to_free);
	}
}
