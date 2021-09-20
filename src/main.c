/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/13 14:12:21 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/20 12:46:01 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

long	get_current_time(void)
{
	struct timeval	current_time;

	gettimeofday(&current_time, NULL);
	return ((current_time.tv_sec * 1000) + (current_time.tv_usec / 1000));
}

void	print_to_terminal(char *output, t_simulation *simulation, int philo_id,
					   int is_philo_die)
{
	pthread_mutex_lock(&simulation->message);
	if (is_philo_die)
	{
		printf("%ld\t%d%s", get_current_time() - simulation->start_time,
			philo_id, output);
		pthread_mutex_unlock(&simulation->main_lock);
	}
	else
	{
		printf("%ld\t%d%s", get_current_time() - simulation->start_time,
			philo_id, output);
		pthread_mutex_unlock(&simulation->message);
	}
}

void	destroy_simulation(t_simulation *simulation, t_philo *philos_data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&simulation->main_lock);
	pthread_mutex_unlock(&simulation->main_lock);
	pthread_mutex_unlock(&simulation->main_lock);
	while (i < simulation->number_of_philos)
	{
		pthread_mutex_destroy(&simulation->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&simulation->main_lock);
	pthread_mutex_destroy(&simulation->message);
	free(simulation);
	free(philos_data);
}

int	main(int ac, char **av)
{
	int				i;
	t_simulation	*simulation;
	pthread_t		*philos_threads;
//	pthread_t		*philo_watcher;
	t_philo			*philos_data;

	i = 0;
	if (ac >= 5 && check_args(ac, av))
	{
		simulation = ft_parse_args(ac, av);
		pthread_mutex_init(&simulation->message, NULL);
		philos_data = init_simaulation_philos(simulation);
		philos_threads = (pthread_t *)malloc(sizeof(pthread_t)
				* simulation->number_of_philos);
		//philo_watcher = (pthread_t *)malloc(sizeof(pthread_t)
		//		* simulation->number_of_philos);
		while (i < simulation->number_of_philos)
		{
			philos_data[i].limit = get_current_time()
				+ simulation->time_to_die;
			//pthread_create(&philo_watcher[i], NULL,
			//	watch_philo_routine, &philos_data[i]);
			//pthread_detach(philo_watcher[i]);
			pthread_create(&philos_threads[i], NULL,
				philo_routine, &philos_data[i]);
			pthread_detach(philos_threads[i]);
			i++;
			usleep(100);
		}
		destroy_simulation(simulation, philos_data);
	}
	return (0);
}