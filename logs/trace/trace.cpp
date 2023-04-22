#include "trace_static.h"

//================================================================================================================================
// SOURCE_POS
//================================================================================================================================

//--------------------------------------------------------------------------------------------------------------------------------
// verify
//--------------------------------------------------------------------------------------------------------------------------------

static bool source_pos_verify(const source_pos *const src_pos)
{
    if (src_pos == nullptr) return false;
    if ($file   == nullptr) return false;
    if ($func   == nullptr) return false;
    if ($line   <        0) return false;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void source_pos_debug_dump(const source_pos *const src_pos)
{
    log_tab_service_message("source_pos (addr: ", "");
    if (src_pos == nullptr) log_error_message  ("nullptr", "");
    else                    log_default_message("%p", "" , src_pos);

    log_service_message(")\n{", "\n");
    if (src_pos == nullptr) { log_service_message("}", "\n"); return; }

    LOG_TAB++;

    log_tab_service_message("file = ", "");
    if ($file == nullptr) log_error_message  ("nullptr" , "\n");
    else                  log_default_message("%s", "\n", $file);

    log_tab_service_message("func = ", "");
    if ($func == nullptr) log_error_message  ("nullptr" , "\n");
    else                  log_default_message("%s", "\n", $func);

    log_tab_service_message("line = ", "");
    if ($line <= 0)       log_error_message  ("%d", "\n", $line);
    else                  log_default_message("%d", "\n", $line);

    LOG_TAB--;
    log_tab_service_message("}", "\n");
}

//--------------------------------------------------------------------------------------------------------------------------------
// ctor
//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_ctor(source_pos *const src_pos, const char *const file,
                                                              const char *const func,
                                                              const int         line)
{
    log_assert(src_pos != nullptr);

    $file = file;
    $func = func;
    $line = line;
}

//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_ctor(source_pos *const src_pos, const source_pos *const sample)
{
    log_assert(src_pos != nullptr);
    log_assert(sample  != nullptr);

    memcpy(src_pos, sample, sizeof(source_pos));
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

static inline void source_pos_dump(const source_pos *const src_pos)
{
    log_assert(src_pos != nullptr);

    log_param_place($file, $func, $line);
}

//================================================================================================================================
// TRACE
//================================================================================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

//--------------------------------------------------------------------------------------------------------------------------------
// verify
//--------------------------------------------------------------------------------------------------------------------------------

static bool trace_verify(const trace *const trc)
{
    if (trc   ==   nullptr) return false;
    if ($stk  ==   nullptr) return false;
    if ($size >  $capacity) return false;

    if (!source_pos_verify(&$front)) return false;

    for (size_t i = 0; i < $size; ++i)
    {
        if (!source_pos_verify($stk + i)) return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

static void trace_debug_dump(const trace *const trc)
{
    log_tab_service_message("trace (addr: ", "");
    if (trc == nullptr) log_error_message  ("nullptr", "");
    else                log_default_message("%p",  "", trc);

    log_service_message(")\n{", "\n");
    if (trc == nullptr) { log_service_message("}", "\n"); return; }

    LOG_TAB++;

    log_tab_service_message("size     = ", "");
    if ($size > $capacity) log_error_message  ("%d", "\n", $size);
    else                   log_default_message("%d", "\n", $size);

    log_tab_service_message("capacity = ", "");
    if ($size > $capacity) log_error_message  ("%d", "\n", $capacity);
    else                   log_default_message("%d", "\n", $capacity);

    log_tab_service_message("front_trace", "\n");
    source_pos_debug_dump(&$front);

    trace_stack_debug_dump(trc);
}

//--------------------------------------------------------------------------------------------------------------------------------

static void trace_stack_debug_dump(const trace *const trc)
{
    log_assert(trc != nullptr);

    log_tab_service_message("stack_trace = ", "");
    if ($stk == nullptr) log_error_message  ("nullptr" , "\n");
    else                 log_default_message("%p", "\n", $stk);

    if ($stk ==   nullptr) return;
    if ($size > $capacity) return;

    log_tab_service_message("{", "\n");
    LOG_TAB++;

    for (size_t i = 0; i < $size; ++i)
    {
        log_tab_service_message("#%d", "\n", i);
        source_pos_debug_dump($stk + i);
    }

    LOG_TAB--;
    log_tab_service_message("}", "\n");
}

#pragma GCC diagnostic pop

//--------------------------------------------------------------------------------------------------------------------------------
// ctor, dtor
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
    log_trace_verify(trc, (void) 0);

    #if !defined(NLOG) && !defined(LOG_NVERIFY)
    if ($size != 0)
    {
        log_tab_message("\n" HTML_COLOR_DARK_ORANGE
                        "WARNING:\n"
                        "TRACE.stack_trace is not empty before dtor.\n"
                        "====================\n");
        _trace_dump(trc);
        log_tab_message("===================="
                        HTML_COLOR_CANCEL "\n");
    }
    #endif

    log_free($stk);
}

//--------------------------------------------------------------------------------------------------------------------------------
// query
//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_push(trace *const trc)
{
    log_trace_verify(trc, false);

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
        log_error("Can't allocate memory for TRACE.stack_trace");
        return false;
    }

    $capacity =           new_capacity;
    $stk      = (source_pos *) new_stk;

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_front_upd(trace *const trc, const char *const file,
                                        const char *const func,
                                        const int         line)
{
    log_trace_verify(trc, false);

    source_pos new_pos = {};
    source_pos_ctor(&new_pos, file, func, line);

    log_src_pos_verify(trc , &new_pos, false);
    source_pos_ctor(&$front, &new_pos);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------

bool _trace_pop(trace *const trc)
{
    log_trace_verify(trc, false);

    #if !defined(NLOG) && !defined(LOG_NVERIFY)
    if ($size == 0)
    {
        log_tab_error_message("\n"
                              "ERROR:\n"
                              "TRACE.stack_trace is empty before pop.\n"
                              "====================", "\n");
        _trace_dump(trc);
        log_tab_error_message("====================", "\n\n");

        return false;
    }
    #endif

    $size--;
    source_pos_ctor(&$front, $stk + $size);

    return true;
}

//--------------------------------------------------------------------------------------------------------------------------------
// dump
//--------------------------------------------------------------------------------------------------------------------------------

void _trace_dump(const trace *const trc)
{
    log_trace_verify(trc, (void) 0);

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