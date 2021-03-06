/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_bonus.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zjamali <zjamali@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:54:01 by zjamali           #+#    #+#             */
/*   Updated: 2021/09/24 12:32:31 by zjamali          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_BONUS_H
# define PHILOSOPHER_BONUS_H
# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <stdlib.h>
# include <semaphore.h>
# include <signal.h>

# define INT_MAX 2147483647
# define INT_MIN -2147483648

typedef struct s_simulation
{
	int				number_of_philos;
	long			start_time;

	int				time_to_die;
	int				time_to_eat;
	int				eating_times;
	int				time_to_sleep;
	int				is_times_to_eat;

	sem_t			*forks;
	sem_t			*message;
	sem_t			*main_lock;
	sem_t			*eating_times_count;
	int				*pid;
}	t_simulation;
typedef struct s_philo_data
{
	int				philo_id;
	int				time_to_sleep;
	int				time_to_die;
	int				time_to_eat;
	int				eating_times;

	int				is_times_to_eat;
	int				times_to_eat;

	long			limit;
	t_simulation	*simulation;
	sem_t			*is_eating;
}	t_philo;

/*
* libft function 
*/
int				ft_atoi(const char *str);

/*
* parse functions
*/
int				check_args(int ac, char **av);
t_simulation	*ft_parse_args(int ac, char **av);

/*
* philo routine
*/
void			*watch_philo_routine(void *philo_data);

/*
* thread watching eating times
*/
void			*watch_eating_times(void *simulation);
/*
*	phlo routine 
*/
void			*philo_routine(t_philo *philo_data);

/*
*
*/
long			get_current_time(void);

/*
*   print to terminal
*/
void			print_to_terminal(char *output, t_simulation *simulation,
					int philo_id, int is_philo_die);
/*
*	init simulation
*/
t_philo			*init_simaulation_philos(t_simulation *simulation);

/*
*
*/
int				handle_errors(void);
#endif