/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/24 12:27:46 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/philosopher_bonus.h"

long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_to_terminal(char *output, t_simulation *simulation, int philo_id,
					   int is_philo_die)
{
	sem_wait(simulation->message);
	if (is_philo_die)
	{
		printf("%ld\t%d%s", get_current_time() - simulation->start_time,
			philo_id, output);
		sem_post(simulation->main_lock);
	}
	else
	{
		printf("%ld\t%d%s", get_current_time() - simulation->start_time,
			philo_id, output);
		sem_post(simulation->message);
	}
}

void	destroy_simulation(t_simulation *simulation, t_philo *philos_data)
{
	int		i;

	sem_wait(simulation->main_lock);
	i = 0;
	while (i < simulation->number_of_philos)
	{
		kill(simulation->pid[i], SIGKILL);
		i++;
	}
	sem_close(simulation->forks);
	sem_close(simulation->main_lock);
	free(simulation->pid);
	free(simulation);
	free(philos_data);
}

void	create_philosophers(t_philo *philos_data, t_simulation *simulation)
{
	int				i;
	pthread_t		eating_times_watcher;

	i = 0;
	if (simulation->is_times_to_eat)
	{
		pthread_create(&eating_times_watcher, NULL,
			watch_eating_times, simulation);
		pthread_detach(eating_times_watcher);
	}
	while (i < simulation->number_of_philos)
	{
		simulation->pid[i] = fork();
		if (!simulation->pid[i])
		{
			sem_unlink("is_eating");
			philos_data[i].is_eating = sem_open("is_eating", O_CREAT, 644, 1);
			philos_data[i].limit = get_current_time() + simulation->time_to_die;
			philo_routine(&philos_data[i]);
			exit (0);
		}
		i++;
		usleep(100);
	}
	destroy_simulation(simulation, philos_data);
}

int	main(int ac, char **av)
{
	int				i;
	t_simulation	*simulation;
	t_philo			*philos_data;

	i = 0;
	if (check_args(ac, av))
	{
		simulation = ft_parse_args(ac, av);
		if (!simulation)
		{
			return (handle_errors());
		}
		philos_data = init_simaulation_philos(simulation);
		if (!philos_data)
		{
			free(simulation);
			return (handle_errors());
		}
		create_philosophers(philos_data, simulation);
		return (0);
	}
	else
		return (1);
	return (0);
}
