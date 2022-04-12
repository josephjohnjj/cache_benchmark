#ifndef _cache_h_
#define _cache_h_
#include "parsec.h"
#include "parsec/parsec_internal.h"

BEGIN_C_DECLS
#define PARSEC_cache_DEFAULT_ADT_IDX    0
#define PARSEC_cache_ADT_IDX_MAX       1
    typedef struct parsec_cache_taskpool_s {
    parsec_taskpool_t super;
    /* The list of globals */
    parsec_data_collection_t *_g_Block /* data Block */ ;
    int _g_block_size;
    int _g_N;
    /* The array of datatypes (DEFAULT and co.) */
    parsec_arena_datatype_t arenas_datatypes[PARSEC_cache_ADT_IDX_MAX];
    uint32_t arenas_datatypes_size;
} parsec_cache_taskpool_t;

/** Beware that this object is not a superset of the internal object, and as a result
 *  we should never create one directly. Instead we should rely on the creation of the
 *  internal object (__parsec_cache_taskpool_t).
 */
PARSEC_OBJ_CLASS_DECLARATION(parsec_cache_taskpool_t);
extern parsec_cache_taskpool_t *parsec_cache_new(parsec_data_collection_t * _g_Block /* data Block */ ,
						 int _g_block_size, int _g_N);

#if MAX_LOCAL_COUNT < 1		/* number of parameters and locals Task */
#error Too many parameters and local variables (1 out of MAX_LOCAL_COUNT) for task Task
#endif /* MAX_PARAM_COUNT */
typedef struct __parsec_cache_Task_assignment_s {
    parsec_assignment_t k;
    parsec_assignment_t reserved[MAX_LOCAL_COUNT - 1];
} __parsec_cache_Task_parsec_assignment_t;

#if MAX_PARAM_COUNT < 1		/* total number of flows for task Task */
#error Too many flows (1 out of MAX_PARAM_COUNT) for task Task
#endif /* MAX_PARAM_COUNT */
typedef struct __parsec_cache_Task_data_s {
    parsec_data_pair_t _f_A;
    parsec_data_pair_t unused[MAX_LOCAL_COUNT - 1];
} __parsec_cache_Task_data_t;

typedef struct __parsec_cache_Task_task_s {
    PARSEC_MINIMAL_EXECUTION_CONTEXT
#if defined(PARSEC_PROF_TRACE)
    parsec_task_prof_info_t prof_info;
#endif				/* defined(PARSEC_PROF_TRACE) */
    struct __parsec_cache_Task_assignment_s locals;
#if defined(PARSEC_SIM)
    int sim_exec_date;
#endif
    struct __parsec_cache_Task_data_s data;
} __parsec_cache_Task_task_t;


END_C_DECLS
#endif /* _cache_h_ */
