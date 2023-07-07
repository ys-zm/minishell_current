/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builder.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/06/25 00:26:11 by fra           #+#    #+#                 */
/*   Updated: 2023/07/08 00:49:58 by fra           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell/minishell.h"

bool	set_cmd(t_list *tokens, t_cmd *cmd)
{
	if (count_words(tokens) != 0)
	{
		cmd->full_cmd = fill_words(tokens);
		if (cmd->full_cmd == NULL)
			return (false);
		cmd->cmd_name = cmd->full_cmd[0];
	}
	return (true);
}

bool	set_reds(t_list *tokens, t_cmd *cmd, uint32_t order, char *hd)
{
	cmd->n_redirect = count_redirections(tokens);
	if (cmd->n_redirect != 0)
	{
		cmd->redirections = \
			get_redirections(tokens, cmd->n_redirect, order, hd);
		if (cmd->redirections == NULL)
			return (false);
		cmd->files = get_files(tokens, cmd->n_redirect);
		if (cmd->files == NULL)
			return (false);
	}
	return (true);
}

t_cmd	*build_cmd(t_var *mini, t_cmd *cmd, char *curr_cmd, uint32_t order_cmd)
{
	t_list		*tokens;

	cmd->fd_in = 0;
	cmd->fd_out = 1;
	cmd->cmd_name = NULL;
	cmd->full_cmd = NULL;
	cmd->n_redirect = 0;
	cmd->redirections = NULL;
	cmd->files = NULL;
	if (ft_is_empty_str(curr_cmd) == false)
	{
		tokens = tokenize(curr_cmd);
		if (tokens == NULL)
			return (NULL);
		if (set_cmd(tokens, cmd) == false)
			return (ft_lstclear(&tokens, ft_free));
		if (set_reds(tokens, cmd, order_cmd, mini->here_doc_path) == false)
			return (ft_lstclear(&tokens, ft_free));
		ft_lstclear(&tokens, ft_free);
	}
	return (cmd);
}
