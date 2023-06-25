/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tools.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: fra <fra@student.42.fr>                      +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/05/20 19:09:49 by fra           #+#    #+#                 */
/*   Updated: 2023/06/25 02:04:58 by fra           ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quotes(char *to_clear, bool free_string)
{
	char		*cleaned_str;
	uint32_t	len;
	uint32_t	i;
	uint32_t	j;

	len = ft_strlen(to_clear);
	i = 0;
	while (to_clear[i])
	{
		if (is_valid_quote(to_clear, i))
			len--;
		i++;
	}
	if (i == len)
		cleaned_str = ft_strdup(to_clear);
	else
	{
		cleaned_str = ft_calloc(len + 1, sizeof(char));
		if (cleaned_str)
		{
			i = 0;
			j = 0;
			while (to_clear[j])
			{
				if (! is_valid_quote(to_clear, j))
					cleaned_str[i++] = to_clear[j];
				j++;
			}
		}
	}
	if (free_string == true)
		ft_free(to_clear);
	return (cleaned_str);
}

bool	is_quote(char to_check)
{
	return ((to_check == '\'') || (to_check == '\"'));
}

bool	is_arrow(char to_check)
{
	return ((to_check == '<') || (to_check == '>'));
}

void	print_cmd(t_var	*mini)
{
	uint32_t	i;
	uint32_t	j;
	
	j = 0;
	if (mini == NULL)
		return;
	while (j < mini->n_cmd)
	{
		ft_printf("COMMAND\n\tcmd name: %s\n", mini->cmd_data[j].cmd_name);
		i = 0;
		while (mini->cmd_data[j].full_cmd && mini->cmd_data[j].full_cmd[i])
			ft_printf("\t\targ: %s\n", mini->cmd_data[j].full_cmd[i++]);
		if (mini->cmd_data[j].redirections)
		{
			// ft_printf("\tn. redirections: %u\n", mini->cmd_data[j].n_redirect);
			i = 0;
			while (i < mini->cmd_data[j].n_redirect)
			{
				if (mini->cmd_data[j].redirections[i] == RED_IN_SINGLE)
					ft_printf("\t\tred type: %s file: %s\n", "<", mini->cmd_data[j].files[i]);
				else if (mini->cmd_data[j].redirections[i] == RED_OUT_SINGLE)
					ft_printf("\t\tred type: %s file: %s\n", ">", mini->cmd_data[j].files[i]);
				else if (mini->cmd_data[j].redirections[i] == RED_IN_DOUBLE)
					ft_printf("\t\tred type: %s file: %s\n", "<<", mini->cmd_data[j].files[i]);
				else if (mini->cmd_data[j].redirections[i] == RED_OUT_DOUBLE)
					ft_printf("\t\tred type: %s file: %s\n", ">>", mini->cmd_data[j].files[i]);
				i++;
			}
		}
		j++;
	}
}

char	*create_file_name(const char *fix_part, int32_t cnt)
{
	char			*file_name;
	char			*char_cnt;

	char_cnt = ft_itoa(cnt);
	if (char_cnt == NULL)
		return (NULL);
	file_name = ft_strjoin((char *) fix_part, char_cnt, "_", false);
	ft_free(char_cnt);
	return (file_name);
}

bool remove_here_docs(t_var *mini)
{
	char		*here_doc_to_drop;
	int32_t		status;
	uint32_t	i;
	uint32_t	j;

	i = 0;
	while (i < mini->n_cmd)
	{
		j = 0;
		while (mini->cmd_data && (j < mini->cmd_data[i].n_redirect))
		{
			if (mini->cmd_data[i].redirections[j++] == RED_IN_DOUBLE)
			{
				here_doc_to_drop = create_file_name(HERE_DOC_FIX, i + 1);
				status = unlink(here_doc_to_drop);
				ft_free(here_doc_to_drop);
				if (status == -1)
					return (false);
				break ;
			}
		}
		i++;
	}
	return (true);
}
