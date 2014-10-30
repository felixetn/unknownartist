#include "FreeRTOS.h"
#include "task.h"

#include <string.h>

#include "runtime_stats.h"
#include "interrupts.h"
#include "wireless/wireless.h"
#include "printf_stdarg.h"

uint32_t runTimerValue;
#if ANALYZE_MEMORY == 1
extern long maxHeapUsage;

static void printTaskList(char* buf) {
	char* start = (char*) buf;
	char* occurence = 0;

	start = (char*) buf + 2;
	do {
		occurence = strchr(start, '\t');
		if (occurence != 0) {
			*occurence = ' ';
			start = occurence + 1;
		}
	} while (occurence != 0);
	start = (char*) buf + 2;
	do {
		occurence = strchr(start, '\n');
		if (occurence != 0) {
			*occurence = 0;
			wirelessDebugMessage(WI_IF_AMB8420, start);
			start = occurence + 1;
		}
	} while (occurence != 0);
}
#endif

void RuntimeStats_Print(void) {
#if ANALYZE_MEMORY == 1
	static signed char buf[400];
	wirelessFormattedDebugMessage(WI_IF_AMB8420, "Heap high: %d", maxHeapUsage);

#if ( configGENERATE_RUN_TIME_STATS == 1 )
	wirelessDebugMessage(WI_IF_AMB8420, "Task time consumption:");
	vTaskGetRunTimeStats(buf);
	printTaskList((char*)buf);
#endif

	wirelessDebugMessage(WI_IF_AMB8420, "Task information:");
	vTaskList(buf);
	printTaskList((char*)buf);

	wirelessFormattedDebugMessage(WI_IF_AMB8420, "Number of tasks: %d", uxTaskGetNumberOfTasks());
#endif
}

#if ( configGENERATE_RUN_TIME_STATS == 1 )
static __interrupt
void vRunTimeStatTimer(void) {
	TMCSR2_UF = 0;
	runTimerValue++;
}

void vConfigureTimerForRunTimeStats(void) {
	TMCSR2_FSEL = 1;
	TMCSR2_CSL0 = 0;
	TMCSR2_CSL1 = 0;
	TMCSR2_MOD0 = 0;
	TMCSR2_MOD1 = 0;
	TMCSR2_MOD2 = 0;
	TMCSR2_OUTE = 0;
	TMCSR2_RELD = 1;
	TMCSR2_INTE = 1;
	TMCSR2_CNTE = 1;
	TMRLR2 = 16000000 / 2 / 100000 - 1;

	TMCSR2_TRG = 1;

	interrupts_setHandler(53, 1, vRunTimeStatTimer);
}
#endif
