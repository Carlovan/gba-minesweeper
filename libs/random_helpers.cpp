#include <random_helpers>

void initialize_random_system() {
	REG_TM2CNT = TM_ENABLE | TM_FREQ_SYS;
}