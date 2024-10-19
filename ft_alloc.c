/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_alloc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ecarvalh <ecarvalh@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 16:20:28 by ecarvalh          #+#    #+#             */
/*   Updated: 2024/10/19 16:23:41 by ecarvalh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_alloc.h"

void	*ft_calloc(size_t num, size_t size);
void	ft_free(void *ptr);
void	ft_clean(void);

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
