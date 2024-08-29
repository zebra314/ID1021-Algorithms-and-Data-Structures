#ifndef TOOL_FUNC_H
#define TOOL_FUNC_H

#include <time.h>
#include <stdlib.h>

extern struct timespec t_start, t_stop;
long nano_seconds(struct timespec *t_start, struct timespec *t_stop);
long bench(int n, int loop);

#endif // TOOL_FUNC_H