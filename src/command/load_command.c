#include "t_command.h"

#include <stdlib.h>
#include <unistd.h>
#include "libft/ft_string.h"
#include "libft/string.h"

char* full_path(const char* dir, const char* file);
t_error locate_command(const char* name, const t_env* env, char** cmd_out);

void free_str_array(char** arr, size_t n)
{
	size_t i;

	if (!arr)
		return ;
	i = 0;
	while (i < n)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char** duplicate_args(char** args, size_t n_args)
{
	char** out;
	size_t i;

	out = malloc((1 + n_args) * sizeof(char*));
	if (!out)
		return NULL;
	i = 0;
	while (i < n_args)
	{
		out[i] = ft_strdup(args[i]);
		if (!out[i])
		{
			free_str_array(out, i);
			return NULL;
		}
		i++;
	}
	out[i] = NULL;
	return out;
}

t_error load_command(char* name, char** args, size_t n_args, const t_env* env,t_command* cmd_out)
{
	t_error err;

	err = locate_command(name, env, &cmd_out->location);
	if (err != NO_ERROR)
		return err;
	cmd_out->args = duplicate_args(args, n_args);
	if (cmd_out->args == NULL)
		return OOM_ERROR;
	return NO_ERROR;
}

char* full_path(const char* dir, const char* file)
{
	char* dir_with_slash;
	char* path;

	if (!dir || !file)
		return NULL;
	dir_with_slash = ft_strjoin(dir, "/");
	if (!dir_with_slash)
		return NULL;
	path = ft_strjoin(dir_with_slash, file);
	if (!path)
		return NULL;
	free(dir_with_slash);
	return path;
}

t_error locate_command(const char* name, const t_env* env, char** cmd_out)
{
	char* candidate;
	char* dir;
	int i = 0;

	dir = env->path[0];
	while (dir)
	{
		candidate = full_path(dir, name);
		if (!candidate)
			return OOM_ERROR;
		if (access(candidate, X_OK) == 0)
		{
			*cmd_out = candidate;
			return NO_ERROR;
		}
		free(candidate);
		dir = env->path[++i];
	}
	return FILE_NOT_FOUND_ERROR;
}
