#ifndef XC_MEMORY_H
#define	XC_MEMORY_H

void memory_and_cycle_positions(BattParameters *bat_param);
void check_if_any_changes_in_parameters(BattParameters *bat_param);
void read_parameters_from_flash(BattParameters *bat_param);
void save_parameters_to_flash(BattParameters *bat_param);
#endif	