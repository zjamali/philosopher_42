/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/20 08:52:24 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/20 12:45:44 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/philosopher.h"

void	philo_taken_forks(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_lock(&simulation->forks[philo->philo_id - 1]);
	print_to_terminal("\t taken fork\n", simulation, philo->philo_id, 0);
	pthread_mutex_lock(
		&simulation->forks[philo->philo_id % simulation->number_of_philos]);
	print_to_terminal("\t taken fork\n", simulation, philo->philo_id, 0);
}

void	philo_is_eating(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	//pthread_mutex_lock(&philo->is_eating);
	print_to_terminal("\t\033[0;32m EATING \033[0m\n", simulation,
		philo->philo_id, 0);
	philo->limit = get_current_time() + philo->time_to_die;
	usleep(philo->time_to_eat * 1000);
	//pthread_mutex_unlock(&philo->is_eating);
}

void	philo_start_sleeping(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	pthread_mutex_unlock(&simulation->forks[philo->philo_id - 1]);
	pthread_mutex_unlock(
		&simulation->forks[philo->philo_id % simulation->number_of_philos]);
	print_to_terminal("\t\033[0;34m SLEEPING \033[0m\n", simulation,
		philo->philo_id, 0);
	usleep(philo->time_to_sleep * 1000);
}

void	philo_start_thinking(t_philo *current_philo)
{
	t_philo			*philo;
	t_simulation	*simulation;

	philo = current_philo;
	simulation = philo->simulation;
	print_to_terminal("\t\033[0;33m THINKING \033[0m\n", simulation,
		philo->philo_id, 0);
}

void	*philo_routine(void *philo_data)
{
	pthread_t		philo_watcher;
	t_philo			*philo;
	t_simulation	*simulation;

	philo = philo_data;
	simulation = philo->simulation;
	// philo->limit = get_current_time() + simulation->time_to_die;
		// printf("start|index = %d|%ld\n" ,philo->philo_id, get_current_time());
	
	//if (philo)
	//{
		pthread_create(&philo_watcher, NULL, watch_philo_routine, philo);
		pthread_detach(philo_watcher);
	//}
	while (philo->eating_times)
	{
		// printf("while|index = %d|%ld\n" ,philo->philo_id, get_current_time());
		philo_taken_forks(philo);
		philo_is_eating(philo);
		if (philo->is_times_to_eat)
			philo->eating_times--;
		else
			philo->eating_times = 1;
		philo_start_sleeping(philo);
		philo_start_thinking(philo);
	}
	return (NULL);
}