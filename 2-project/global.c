
int	*retrieve_exit_status(void)
{
	static int	exit_value = 0x0;

	return (&exit_value);
}

void	exit_value_set(int exit_value)
{
	*retrieve_exit_status() = exit_value;
}

int	*retrieve_exit_ctl_c(void)
{
	static int	exit_value = 0x0;

	return (&exit_value);
}

void	ctl_c_value_set(int exit_value)
{
	*retrieve_exit_ctl_c() = exit_value;
}

void	shell_exit(int status, char *msg)
{
	exit_value_set(status);
	garbage_exit(status, msg);
}