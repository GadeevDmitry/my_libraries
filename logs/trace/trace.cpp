#include "trace_static.h"

//--------------------------------------------------------------------------------------------------------------------------------
// source_pos
//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_ctor(source_pos *const src_pos, const char *const file,
                                                              const char *const func,
                                                              const int         line)
{
    log_assert(src_pos != nullptr);
    log_assert(file    != nullptr);
    log_assert(func    != nullptr);

    $file = file;
    $func = func;
    $line = line;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_ctor(source_pos *const src_pos, const source_pos *const sample)
{
    log_assert(src_pos != nullptr);
    log_assert(sample  != nullptr);

    log_assert(sample->file != nullptr);
    log_assert(sample->func != nullptr);

    memcpy(src_pos, sample, sizeof(source_pos));
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_dump(const source_pos *const src_pos)
{
    log_assert(src_pos != nullptr);
    log_assert($file   != nullptr);
    log_assert($func   != nullptr);

    log_param_place($file, $func, $line);
}

//--------------------------------------------------------------------------------------------------------------------------------
// trace
//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_ctor(trace *const trc)
{
    log_assert(trc != nullptr);

    source_pos_ctor(&$front, __FILE__, __PRETTY_FUNCTION__, __LINE__);

    $stk = (source_pos *) log_calloc(DEFAULT_TRACE_CAPACITY, sizeof(source_pos));
    if ($stk == nullptr)
    {
        log_error("can't allocate memory for TRACE.srack_trace");
        return false;
    }

    $size     = 0;
    $capacity = DEFAULT_TRACE_CAPACITY;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_dtor(trace *const trc)
{
    log_assert(trc != nullptr);

    log_free($stk);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_push(trace *const trc)
{
    log_assert(trc != nullptr);

    if (!trace_resize(trc)) return false;

    source_pos_ctor($stk + $size, &$front);
    $size++;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static bool trace_resize(trace *const trc)
{
    log_assert(trc != nullptr);

    if ($size != $capacity) return true;

    size_t new_capacity = 2 * $capacity;

    void  *new_stk = realloc($stk, new_capacity * sizeof(source_pos));
    if    (new_stk == nullptr)
    {
        log_error("can't allocate memory for TRACE.stack_trace");
        return false;
    }

    $capacity =           new_capacity;
    $stk      = (source_pos *) new_stk;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_front_upd(trace *const trc, const char *const file,
                                        const char *const func,
                                        const int         line)
{
    log_assert(trc  != nullptr);
    log_assert(file != nullptr);
    log_assert(func != nullptr);

    source_pos_ctor(&$front, file, func, line);
}

//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_pop(trace *const trc)
{
    log_assert(trc != nullptr);

    $size--;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

void _trace_dump(const trace *const trc)
{
    log_assert(trc != nullptr);

    trace_el_dump(&$front, 0);

    for (size_t i = 1; i <= $size; ++i)
    {
        trace_el_dump($stk + ($size - i), i);
    }
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void trace_el_dump(const source_pos *const src_pos, const size_t index)
{
    log_assert(src_pos != nullptr);

    log_tab_message(HTML_COLOR_MEDIUM_BLUE "#%d:" HTML_COLOR_CANCEL "\n", index);
    source_pos_dump(src_pos);
}