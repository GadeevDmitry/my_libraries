#ifndef TRACE_H
#define TRACE_H

//================================================================================================================================

void trace_push();
void trace_pop ();
void trace_upd_pos(const char *file, const char *func, const int line);

#endif // TRACE_H
