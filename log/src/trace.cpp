#include "trace_static.h"

//================================================================================================================================

struct source_pos
{
    const char *file;
    const char *func;
    int         line;
};

static void source_pos_dump(const void *const src_pos_);

//--------------------------------------------------------------------------------------------------------------------------------

struct trace
{
    source_pos cur_pos;
    stack      stack_trace;
};

//================================================================================================================================

trace TRACE = {};

//================================================================================================================================

bool trace_ctor()
{
    TRACE.cur_pos =
    {
        .file = __FILE__,
        .func = __PRETTY_FUNCTION__,
        .line = __LINE__
    };

    return stack_ctor(&TRACE.stack_trace, sizeof(source_pos), nullptr, nullptr, source_pos_dump);
}

void trace_dtor()
{
    stack_dtor(&TRACE.stack_trace);
}

//--------------------------------------------------------------------------------------------------------------------------------

void trace_push() { stack_push(&TRACE.stack_trace, &TRACE.cur_pos); }
void trace_pop () { stack_pop (&TRACE.stack_trace, &TRACE.cur_pos); }
void trace_dump()
{
    stack_push(&TRACE.stack_trace, &TRACE.cur_pos);
    stack_dump(&TRACE.stack_trace);
    stack_pop (&TRACE.stack_trace);
}

void trace_upd_pos(const char *file, const char *func, const int line)
{
    TRACE.cur_pos =
    {
        .file = file,
        .func = func,
        .line = line
    };
}

size_t trace_get_size() { return TRACE.stack_trace.size; }

//--------------------------------------------------------------------------------------------------------------------------------

static void source_pos_dump(const void *const src_pos_)
{
    const source_pos *src_pos = (const source_pos *) src_pos_;
    LOG_PARAM_PLACE(src_pos->file, src_pos->func, src_pos->line);
}
