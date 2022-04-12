#include "parsec.h"
#include "parsec/parsec_internal.h"
#include "parsec/ayudame.h"
#include "parsec/execution_stream.h"
#if defined(PARSEC_HAVE_CUDA)
#include "parsec/mca/device/cuda/device_cuda.h"
#endif /* defined(PARSEC_HAVE_CUDA) */
#if defined(_MSC_VER) || defined(__MINGW32__)
#include <malloc.h>
#else
#include <alloca.h>
#endif /* defined(_MSC_VER) || defined(__MINGW32__) */

#define PARSEC_cache_NB_TASK_CLASSES 1
#define PARSEC_cache_NB_DATA 1

typedef struct __parsec_cache_internal_taskpool_s __parsec_cache_internal_taskpool_t;
struct parsec_cache_internal_taskpool_s;

/** Predeclarations of the parsec_task_class_t */
static const parsec_task_class_t cache_Task;
/** Predeclarations of the parameters */
static const parsec_flow_t flow_of_cache_Task_for_A;
#line 3 "cache.jdf"


#include "parsec.h"
#include "parsec/data_dist/matrix/two_dim_rectangle_cyclic.h"

#define MY_TYPE parsec_datatype_double_t

#line 33 "cache.c"
#define PARSEC_cache_DEFAULT_ADT    (&__parsec_tp->super.arenas_datatypes[PARSEC_cache_DEFAULT_ADT_IDX])
#include "cache.h"

struct __parsec_cache_internal_taskpool_s {
    parsec_cache_taskpool_t super;
    volatile int32_t sync_point;
    parsec_task_t *startup_queue;
    /* The ranges to compute the hash key */
    int Task_k_range;
    /* The list of data repositories  Task */
    data_repo_t *repositories[1];
};

#if defined(PARSEC_PROF_TRACE)
#if defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT)
static int cache_profiling_array[4 * PARSEC_cache_NB_TASK_CLASSES] = { -1 };	/* 2 pairs (begin, end) per task, times two because each task class has an internal_init task */
#else /* defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT) */
static int cache_profiling_array[2 * PARSEC_cache_NB_TASK_CLASSES] = { -1 };	/* 2 pairs (begin, end) per task */
#endif /* defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT) */
#endif /* defined(PARSEC_PROF_TRACE) */
/* Globals */
#define Block __parsec_tp->super._g_Block
#define block_size (__parsec_tp->super._g_block_size)
#define N (__parsec_tp->super._g_N)

static inline int parsec_imin(int a, int b)
{
    return (a <= b) ? a : b;
};

static inline int parsec_imax(int a, int b)
{
    return (a >= b) ? a : b;
};

/* Data Access Macros */
#define data_of_Block(Block_d0, Block_d1)  (((parsec_data_collection_t*)__parsec_tp->super._g_Block)->data_of((parsec_data_collection_t*)__parsec_tp->super._g_Block, (Block_d0), (Block_d1)))

#define rank_of_Block(Block_d0, Block_d1)  (((parsec_data_collection_t*)__parsec_tp->super._g_Block)->rank_of((parsec_data_collection_t*)__parsec_tp->super._g_Block, (Block_d0), (Block_d1)))

/* Functions Predicates */
#define Task_pred(k) (((parsec_data_collection_t*)(__parsec_tp->super._g_Block))->myrank == rank_of_Block(0, 0))

/* Data Repositories */
#define Task_repo (__parsec_tp->repositories[0])
/* Release dependencies output macro */
#if defined(PARSEC_DEBUG_NOISIER)
#define RELEASE_DEP_OUTPUT(ES, DEPO, TASKO, DEPI, TASKI, RSRC, RDST, DATA)\
  do { \
    char tmp1[128], tmp2[128]; (void)tmp1; (void)tmp2;\
    PARSEC_DEBUG_VERBOSE(20, parsec_debug_output, "thread %d VP %d explore deps from %s:%s to %s:%s (from rank %d to %d) base ptr %p",\
           (NULL != (ES) ? (ES)->th_id : -1), (NULL != (ES) ? (ES)->virtual_process->vp_id : -1),\
           DEPO, parsec_task_snprintf(tmp1, 128, (parsec_task_t*)(TASKO)),\
           DEPI, parsec_task_snprintf(tmp2, 128, (parsec_task_t*)(TASKI)), (RSRC), (RDST), (DATA));\
  } while(0)
#define ACQUIRE_FLOW(TASKI, DEPI, FUNO, DEPO, LOCALS, PTR)\
  do { \
    char tmp1[128], tmp2[128]; (void)tmp1; (void)tmp2;\
    PARSEC_DEBUG_VERBOSE(20, parsec_debug_output, "task %s acquires flow %s from %s %s data ptr %p",\
           parsec_task_snprintf(tmp1, 128, (parsec_task_t*)(TASKI)), (DEPI),\
           (DEPO), parsec_snprintf_assignments(tmp2, 128, (FUNO), (parsec_assignment_t*)(LOCALS)), (PTR));\
  } while(0)
#else
#define RELEASE_DEP_OUTPUT(ES, DEPO, TASKO, DEPI, TASKI, RSRC, RDST, DATA)
#define ACQUIRE_FLOW(TASKI, DEPI, TASKO, DEPO, LOCALS, PTR)
#endif
static inline parsec_key_t __jdf2c_make_key_Task(const parsec_taskpool_t * tp, const parsec_assignment_t * as)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp = (const __parsec_cache_internal_taskpool_t *) tp;
    __parsec_cache_Task_parsec_assignment_t ascopy, *assignment = &ascopy;
    uintptr_t __parsec_id = 0;
    memcpy(assignment, as, sizeof(__parsec_cache_Task_parsec_assignment_t));
    const int k = assignment->k.value;
    int __jdf2c_k_min = 0;
    __parsec_id += (k - __jdf2c_k_min);
    (void) __parsec_tp;
    return (parsec_key_t) __parsec_id;
}

static char *__jdf2c_key_fns_Task_key_print(char *buffer, size_t buffer_size, parsec_key_t __parsec_key_,
					    void *user_data)
{
    uint64_t __parsec_key = (uint64_t) (uintptr_t) __parsec_key_;
    __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) user_data;
    int __jdf2c_k_min = 0;
    int k = (__parsec_key) % __parsec_tp->Task_k_range + __jdf2c_k_min;
    __parsec_key = __parsec_key / __parsec_tp->Task_k_range;
    snprintf(buffer, buffer_size, "Task(%d)", k);
    return buffer;
}

static parsec_key_fn_t __jdf2c_key_fns_Task = {
    .key_equal = parsec_hash_table_generic_64bits_key_equal,
    .key_print = __jdf2c_key_fns_Task_key_print,
    .key_hash = parsec_hash_table_generic_64bits_key_hash
};

/******                                      Task                                      ******/

static inline int32_t minexpr_of_symb_cache_Task_k_fct(const __parsec_cache_internal_taskpool_t * __parsec_tp,
						       const __parsec_cache_Task_parsec_assignment_t * locals)
{
    (void) __parsec_tp;
    (void) locals;
    return 0;
}

static const parsec_expr_t minexpr_of_symb_cache_Task_k = {
    .op = PARSEC_EXPR_OP_INLINE,
    .u_expr.v_func = {.type = PARSEC_RETURN_TYPE_INT32,
		      .func = {.inline_func_int32 =
			       (parsec_expr_op_int32_inline_func_t) minexpr_of_symb_cache_Task_k_fct}
		      }
};

static inline int32_t maxexpr_of_symb_cache_Task_k_fct(const __parsec_cache_internal_taskpool_t * __parsec_tp,
						       const __parsec_cache_Task_parsec_assignment_t * locals)
{
    (void) __parsec_tp;
    (void) locals;
    return 0;
}

static const parsec_expr_t maxexpr_of_symb_cache_Task_k = {
    .op = PARSEC_EXPR_OP_INLINE,
    .u_expr.v_func = {.type = PARSEC_RETURN_TYPE_INT32,
		      .func = {.inline_func_int32 =
			       (parsec_expr_op_int32_inline_func_t) maxexpr_of_symb_cache_Task_k_fct}
		      }
};
static const parsec_symbol_t symb_cache_Task_k = {.name = "k",.context_index = 0,.min =
	&minexpr_of_symb_cache_Task_k,.max = &maxexpr_of_symb_cache_Task_k,.cst_inc = 1,.expr_inc = NULL,.flags =
	PARSEC_SYMBOL_IS_STANDALONE };

static inline int affinity_of_cache_Task(__parsec_cache_Task_task_t * this_task, parsec_data_ref_t * ref)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp =
	(const __parsec_cache_internal_taskpool_t *) this_task->taskpool;
    const int k = this_task->locals.k.value;
    (void) k;

    /* Silent Warnings: should look into predicate to know what variables are usefull */
    (void) k;
    ref->dc = (parsec_data_collection_t *) __parsec_tp->super._g_Block;
    /* Compute data key */
    ref->key = ref->dc->data_key(ref->dc, 0, 0);
    return 1;
}

static const parsec_property_t properties_of_cache_Task[1] = {
    {.name = NULL,.expr = NULL}
};

static inline int expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iftrue_fct(const
										      __parsec_cache_internal_taskpool_t
										      * __parsec_tp,
										      const
										      __parsec_cache_Task_parsec_assignment_t
										      * locals)
{
    const int k = locals->k.value;

    (void) k;
    (void) __parsec_tp;
    (void) locals;
    return (k) == (0);
}

static const parsec_expr_t expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iftrue = {
    .op = PARSEC_EXPR_OP_INLINE,
    .u_expr.v_func = {.type = PARSEC_RETURN_TYPE_INT32,	/* PARSEC_RETURN_TYPE_INT32 */
		      .func = {.inline_func_int32 =
			       (parsec_expr_op_int32_inline_func_t)
			       expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iftrue_fct}
		      }
};

static parsec_data_t *flow_of_cache_Task_for_A_dep1_atline_23_direct_access(const __parsec_cache_internal_taskpool_t *
									    __parsec_tp,
									    const
									    __parsec_cache_Task_parsec_assignment_t *
									    assignments)
{
    const int k = assignments->k.value;
    (void) k;

    /* Silence Warnings: should look into parameters to know what variables are useful */
    (void) k;
    if (__parsec_tp->super.super.context->my_rank == (int32_t) rank_of_Block(0, 0))
	return data_of_Block(0, 0);
    return NULL;
}

static const parsec_dep_t flow_of_cache_Task_for_A_dep1_atline_23_iftrue = {
    .cond = &expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iftrue,	/* (k) == (0) */
    .ctl_gather_nb = NULL,
    .task_class_id = PARSEC_LOCAL_DATA_TASK_CLASS_ID,	/* cache_Block */
    .direct_data = (parsec_data_lookup_func_t) & flow_of_cache_Task_for_A_dep1_atline_23_direct_access,
    .dep_index = 0,
    .dep_datatype_index = 0,
    .belongs_to = &flow_of_cache_Task_for_A,
};

static inline int expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iffalse_fct(const
										       __parsec_cache_internal_taskpool_t
										       * __parsec_tp,
										       const
										       __parsec_cache_Task_parsec_assignment_t
										       * locals)
{
    const int k = locals->k.value;

    (void) k;
    (void) __parsec_tp;
    (void) locals;
    return !((k) == (0));
}

static const parsec_expr_t expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iffalse = {
    .op = PARSEC_EXPR_OP_INLINE,
    .u_expr.v_func = {.type = PARSEC_RETURN_TYPE_INT32,	/* PARSEC_RETURN_TYPE_INT32 */
		      .func = {.inline_func_int32 =
			       (parsec_expr_op_int32_inline_func_t)
			       expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iffalse_fct}
		      }
};

static const parsec_dep_t flow_of_cache_Task_for_A_dep1_atline_23_iffalse = {
    .cond = &expr_of_cond_for_flow_of_cache_Task_for_A_dep1_atline_23_iffalse,	/* !(k) == (0) */
    .ctl_gather_nb = NULL,
    .task_class_id = 0,		/* cache_Task */
    .direct_data = (parsec_data_lookup_func_t) NULL,
    .flow = &flow_of_cache_Task_for_A,
    .dep_index = 0,
    .dep_datatype_index = 0,
    .belongs_to = &flow_of_cache_Task_for_A,
};

static inline int expr_of_cond_for_flow_of_cache_Task_for_A_dep2_atline_24_fct(const __parsec_cache_internal_taskpool_t
									       * __parsec_tp,
									       const
									       __parsec_cache_Task_parsec_assignment_t *
									       locals)
{
    const int k = locals->k.value;
    (void) k;

    (void) k;
    (void) __parsec_tp;
    (void) locals;
    return (k) < (N);
}

static const parsec_expr_t expr_of_cond_for_flow_of_cache_Task_for_A_dep2_atline_24 = {
    .op = PARSEC_EXPR_OP_INLINE,
    .u_expr.v_func = {.type = PARSEC_RETURN_TYPE_INT32,	/* PARSEC_RETURN_TYPE_INT32 */
		      .func = {.inline_func_int32 =
			       (parsec_expr_op_int32_inline_func_t)
			       expr_of_cond_for_flow_of_cache_Task_for_A_dep2_atline_24_fct}
		      }
};

static const parsec_dep_t flow_of_cache_Task_for_A_dep2_atline_24 = {
    .cond = &expr_of_cond_for_flow_of_cache_Task_for_A_dep2_atline_24,	/* (k) < (N) */
    .ctl_gather_nb = NULL,
    .task_class_id = 0,		/* cache_Task */
    .direct_data = (parsec_data_lookup_func_t) NULL,
    .flow = &flow_of_cache_Task_for_A,
    .dep_index = 0,
    .dep_datatype_index = 0,
    .belongs_to = &flow_of_cache_Task_for_A,
};

#if MAX_DEP_IN_COUNT < 1	/* number of input dependencies */
#error Too many input dependencies (supports up to MAX_DEP_IN_COUNT [=10] but found 1). Fix the code or recompile PaRSEC with a larger MAX_DEP_IN_COUNT.
#endif
#if MAX_DEP_OUT_COUNT < 1	/* number of output dependencies */
#error Too many output dependencies (supports up to MAX_DEP_OUT_COUNT [=10] but found 1). Fix the code or recompile PaRSEC with a larger MAX_DEP_OUT_COUNT.
#endif

static const parsec_flow_t flow_of_cache_Task_for_A = {
    .name = "A",
    .sym_type = PARSEC_SYM_INOUT,
    .flow_flags = PARSEC_FLOW_ACCESS_RW | PARSEC_FLOW_HAS_IN_DEPS,
    .flow_index = 0,
    .flow_datatype_mask = 0x1,
    .dep_in = {&flow_of_cache_Task_for_A_dep1_atline_23_iftrue,
	       &flow_of_cache_Task_for_A_dep1_atline_23_iffalse},
    .dep_out = {&flow_of_cache_Task_for_A_dep2_atline_24}
};

static void
iterate_successors_of_cache_Task(parsec_execution_stream_t * es, const __parsec_cache_Task_task_t * this_task,
				 uint32_t action_mask, parsec_ontask_function_t * ontask, void *ontask_arg)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp =
	(const __parsec_cache_internal_taskpool_t *) this_task->taskpool;
    parsec_task_t nc;		/* generic placeholder for locals */
    parsec_dep_data_description_t data;
    __parsec_cache_Task_parsec_assignment_t __jdf2c__tmp_locals = *(__parsec_cache_Task_parsec_assignment_t *) & this_task->locals;	/* copy of this_task locals in R/W mode to manage local definitions */
    int vpid_dst = -1, rank_src = 0, rank_dst = 0;
    const int k = __jdf2c__tmp_locals.k.value;
    (void) k;
    (void) rank_src;
    (void) rank_dst;
    (void) __parsec_tp;
    (void) vpid_dst;
    data_repo_t *successor_repo;
    parsec_key_t successor_repo_key;
    (void) k;
    nc.taskpool = this_task->taskpool;
    nc.priority = this_task->priority;
    nc.chore_mask = PARSEC_DEV_ALL;
#if defined(DISTRIBUTED)
    rank_src = rank_of_Block(0, 0);
#endif
    if (action_mask & 0x1) {	/* Flow of data A [0] */
	data.data = this_task->data._f_A.data_out;
	if (action_mask &
	    (PARSEC_ACTION_RESHAPE_ON_RELEASE | PARSEC_ACTION_RESHAPE_REMOTE_ON_RELEASE |
	     PARSEC_ACTION_SEND_REMOTE_DEPS)) {
	    data.data_future = NULL;
	    data.local.arena = PARSEC_cache_DEFAULT_ADT->arena;
	    data.local.src_datatype = PARSEC_DATATYPE_NULL;
	    data.local.dst_datatype = PARSEC_DATATYPE_NULL;
	    data.local.src_count = 1;
	    data.local.dst_count = 1;
	    data.local.src_displ = 0;
	    data.local.dst_displ = 0;
	    data.remote.arena = PARSEC_cache_DEFAULT_ADT->arena;
	    data.remote.src_datatype = (data.data != NULL ? data.data->dtt : PARSEC_DATATYPE_NULL);
	    data.remote.dst_datatype = (data.data != NULL ? data.data->dtt : PARSEC_DATATYPE_NULL);
	    data.remote.src_count = 1;
	    data.remote.dst_count = 1;
	    data.remote.src_displ = 0;
	    data.remote.dst_displ = 0;
	}
	if ((k) < (N)) {
	    __parsec_cache_Task_task_t *ncc = (__parsec_cache_Task_task_t *) & nc;
	    nc.task_class = __parsec_tp->super.super.task_classes_array[cache_Task.task_class_id];
	    const int Task_k = (k + 1);
	    if ((Task_k >= (0)) && (Task_k <= (0))) {
		assert(&nc.locals[0].value == &ncc->locals.k.value);
		ncc->locals.k.value = Task_k;
#if defined(DISTRIBUTED)
		rank_dst = rank_of_Block(0, 0);
		if ((NULL != es) && (rank_dst == es->virtual_process->parsec_context->my_rank))
#endif /* DISTRIBUTED */
		    vpid_dst =
			((parsec_data_collection_t *) __parsec_tp->super._g_Block)->
			vpid_of((parsec_data_collection_t *) __parsec_tp->super._g_Block, 0, 0);
		nc.priority = __parsec_tp->super.super.priority;
		successor_repo = Task_repo;
		successor_repo_key =
		    __jdf2c_make_key_Task((const parsec_taskpool_t *) __parsec_tp,
					  (const parsec_assignment_t *) &ncc->locals);
		RELEASE_DEP_OUTPUT(es, "A", this_task, "A", &nc, rank_src, rank_dst, &data);
		if (PARSEC_ITERATE_STOP ==
		    ontask(es, &nc, (const parsec_task_t *) this_task, &flow_of_cache_Task_for_A_dep2_atline_24, &data,
			   rank_src, rank_dst, vpid_dst, successor_repo, successor_repo_key, ontask_arg))
		    return;
	    }
	}
    }
    (void) data;
    (void) nc;
    (void) es;
    (void) ontask;
    (void) ontask_arg;
    (void) rank_dst;
    (void) action_mask;
}

static void
iterate_predecessors_of_cache_Task(parsec_execution_stream_t * es, const __parsec_cache_Task_task_t * this_task,
				   uint32_t action_mask, parsec_ontask_function_t * ontask, void *ontask_arg)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp =
	(const __parsec_cache_internal_taskpool_t *) this_task->taskpool;
    parsec_task_t nc;		/* generic placeholder for locals */
    parsec_dep_data_description_t data;
    __parsec_cache_Task_parsec_assignment_t __jdf2c__tmp_locals = *(__parsec_cache_Task_parsec_assignment_t *) & this_task->locals;	/* copy of this_task locals in R/W mode to manage local definitions */
    int vpid_dst = -1, rank_src = 0, rank_dst = 0;
    const int k = __jdf2c__tmp_locals.k.value;
    (void) k;
    (void) rank_src;
    (void) rank_dst;
    (void) __parsec_tp;
    (void) vpid_dst;
    data_repo_t *successor_repo;
    parsec_key_t successor_repo_key;
    (void) k;
    nc.taskpool = this_task->taskpool;
    nc.priority = this_task->priority;
    nc.chore_mask = PARSEC_DEV_ALL;
#if defined(DISTRIBUTED)
    rank_src = rank_of_Block(0, 0);
#endif
    if (action_mask & 0x1) {	/* Flow of data A [0] */
	data.data = this_task->data._f_A.data_out;
	if (action_mask &
	    (PARSEC_ACTION_RESHAPE_ON_RELEASE | PARSEC_ACTION_RESHAPE_REMOTE_ON_RELEASE |
	     PARSEC_ACTION_SEND_REMOTE_DEPS)) {
	    data.data_future = NULL;
	    data.local.arena = PARSEC_cache_DEFAULT_ADT->arena;
	    data.local.src_datatype = PARSEC_DATATYPE_NULL;
	    data.local.dst_datatype = PARSEC_DATATYPE_NULL;
	    data.local.src_count = 1;
	    data.local.dst_count = 1;
	    data.local.src_displ = 0;
	    data.local.dst_displ = 0;
	    data.remote.arena = PARSEC_cache_DEFAULT_ADT->arena;
	    data.remote.src_datatype = (data.data != NULL ? data.data->dtt : PARSEC_DATATYPE_NULL);
	    data.remote.dst_datatype = (data.data != NULL ? data.data->dtt : PARSEC_DATATYPE_NULL);
	    data.remote.src_count = 1;
	    data.remote.dst_count = 1;
	    data.remote.src_displ = 0;
	    data.remote.dst_displ = 0;
	}
	if (action_mask & 0x1) {
	    if (!((k) == (0))) {
		__parsec_cache_Task_task_t *ncc = (__parsec_cache_Task_task_t *) & nc;
		nc.task_class = __parsec_tp->super.super.task_classes_array[cache_Task.task_class_id];
		const int Task_k = (k - 1);
		if ((Task_k >= (0)) && (Task_k <= (0))) {
		    assert(&nc.locals[0].value == &ncc->locals.k.value);
		    ncc->locals.k.value = Task_k;
#if defined(DISTRIBUTED)
		    rank_dst = rank_of_Block(0, 0);
		    if ((NULL != es) && (rank_dst == es->virtual_process->parsec_context->my_rank))
#endif /* DISTRIBUTED */
			vpid_dst =
			    ((parsec_data_collection_t *) __parsec_tp->super._g_Block)->
			    vpid_of((parsec_data_collection_t *) __parsec_tp->super._g_Block, 0, 0);
		    nc.priority = __parsec_tp->super.super.priority;
		    successor_repo = Task_repo;
		    successor_repo_key =
			__jdf2c_make_key_Task((const parsec_taskpool_t *) __parsec_tp,
					      (const parsec_assignment_t *) &ncc->locals);
		    RELEASE_DEP_OUTPUT(es, "A", this_task, "A", &nc, rank_src, rank_dst, &data);
		    if (PARSEC_ITERATE_STOP ==
			ontask(es, &nc, (const parsec_task_t *) this_task,
			       &flow_of_cache_Task_for_A_dep1_atline_23_iffalse, &data, rank_src, rank_dst, vpid_dst,
			       successor_repo, successor_repo_key, ontask_arg))
			return;
		}
	    }
	}
    }
    (void) data;
    (void) nc;
    (void) es;
    (void) ontask;
    (void) ontask_arg;
    (void) rank_dst;
    (void) action_mask;
}

static int release_deps_of_cache_Task(parsec_execution_stream_t * es, __parsec_cache_Task_task_t * this_task,
				      uint32_t action_mask, parsec_remote_deps_t * deps)
{
    PARSEC_PINS(es, RELEASE_DEPS_BEGIN, (parsec_task_t *) this_task); {
	const __parsec_cache_internal_taskpool_t *__parsec_tp =
	    (const __parsec_cache_internal_taskpool_t *) this_task->taskpool;
	parsec_release_dep_fct_arg_t arg;
	int __vp_id;
	int consume_local_repo = 0;
	arg.action_mask = action_mask;
	arg.output_entry = NULL;
	arg.output_repo = NULL;
#if defined(DISTRIBUTED)
	arg.remote_deps = deps;
#endif /* defined(DISTRIBUTED) */
	assert(NULL != es);
	arg.ready_lists = alloca(sizeof(parsec_task_t *) * es->virtual_process->parsec_context->nb_vp);
	for (__vp_id = 0; __vp_id < es->virtual_process->parsec_context->nb_vp; arg.ready_lists[__vp_id++] = NULL);
	(void) __parsec_tp;
	(void) deps;
	if (action_mask & PARSEC_ACTION_RELEASE_LOCAL_REFS) {
	    if (NULL != this_task->data._f_A.source_repo_entry) {
		data_repo_entry_used_once(this_task->data._f_A.source_repo,
					  this_task->data._f_A.source_repo_entry->ht_item.key);
	    }
	}
	consume_local_repo = (this_task->repo_entry != NULL);
	arg.output_repo = Task_repo;
	arg.output_entry = this_task->repo_entry;
	arg.output_usage = 0;
	if (action_mask & (PARSEC_ACTION_RELEASE_LOCAL_DEPS | PARSEC_ACTION_GET_REPO_ENTRY)) {
	    arg.output_entry =
		data_repo_lookup_entry_and_create(es, arg.output_repo,
						  __jdf2c_make_key_Task((const parsec_taskpool_t *) __parsec_tp,
									(const parsec_assignment_t *) &this_task->
									locals));
	    arg.output_entry->generator = (void *) this_task;	/* for AYU */
#if defined(PARSEC_SIM)
	    assert(arg.output_entry->sim_exec_date == 0);
	    arg.output_entry->sim_exec_date = this_task->sim_exec_date;
#endif
	}
	if (action_mask & (PARSEC_ACTION_RESHAPE_ON_RELEASE | PARSEC_ACTION_RESHAPE_REMOTE_ON_RELEASE)) {
	    /* Generate the reshape promise for thet outputs that need it */
	    iterate_successors_of_cache_Task(es, this_task, action_mask, parsec_set_up_reshape_promise, &arg);
	}
	iterate_successors_of_cache_Task(es, this_task, action_mask, parsec_release_dep_fct, &arg);

#if defined(DISTRIBUTED)
	if ((action_mask & PARSEC_ACTION_SEND_REMOTE_DEPS) && (NULL != arg.remote_deps)) {
	    parsec_remote_dep_activate(es, (parsec_task_t *) this_task, arg.remote_deps,
				       arg.remote_deps->outgoing_mask);
	}
#endif

	if (action_mask & PARSEC_ACTION_RELEASE_LOCAL_DEPS) {
	    struct parsec_vp_s **vps = es->virtual_process->parsec_context->virtual_processes;
	    data_repo_entry_addto_usage_limit(Task_repo, arg.output_entry->ht_item.key, arg.output_usage);
	    for (__vp_id = 0; __vp_id < es->virtual_process->parsec_context->nb_vp; __vp_id++) {
		if (NULL == arg.ready_lists[__vp_id])
		    continue;
		if (__vp_id == es->virtual_process->vp_id) {
		    __parsec_schedule(es, arg.ready_lists[__vp_id], 0);
		} else {
		    __parsec_schedule(vps[__vp_id]->execution_streams[0], arg.ready_lists[__vp_id], 0);
		}
		arg.ready_lists[__vp_id] = NULL;
	    }
	}
	if (consume_local_repo) {
	    data_repo_entry_used_once(Task_repo, this_task->repo_entry->ht_item.key);
	}
	if (action_mask & PARSEC_ACTION_RELEASE_LOCAL_REFS) {
	    if (NULL != this_task->data._f_A.data_in) {
		PARSEC_DATA_COPY_RELEASE(this_task->data._f_A.data_in);
	    }
	}
	PARSEC_PINS(es, RELEASE_DEPS_END, (parsec_task_t *) this_task);
    }
    return 0;
}

static int data_lookup_of_cache_Task(parsec_execution_stream_t * es, __parsec_cache_Task_task_t * this_task)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) this_task->taskpool;
    parsec_assignment_t generic_locals[MAX_PARAM_COUNT];	/* generic task locals */
    int target_device = 0;
    (void) target_device;
    (void) __parsec_tp;
    (void) generic_locals;
    (void) es;
    parsec_data_copy_t *chunk = NULL;
    data_repo_t *reshape_repo = NULL, *consumed_repo = NULL;
    data_repo_entry_t *reshape_entry = NULL, *consumed_entry = NULL;
    parsec_key_t reshape_entry_key = 0, consumed_entry_key = 0;
    uint8_t consumed_flow_index;
    parsec_dep_data_description_t data;
    int ret;
    (void) reshape_repo;
    (void) reshape_entry;
    (void) reshape_entry_key;
    (void) consumed_repo;
    (void) consumed_entry;
    (void) consumed_entry_key;
    (void) consumed_flow_index;
    (void) chunk;
    (void) data;
    (void) ret;
    const int k = this_task->locals.k.value;
    (void) k;
    if (NULL == this_task->repo_entry) {
	this_task->repo_entry =
	    data_repo_lookup_entry_and_create(es, Task_repo,
					      __jdf2c_make_key_Task((const parsec_taskpool_t *) __parsec_tp,
								    (const parsec_assignment_t *) &this_task->locals));
	data_repo_entry_addto_usage_limit(Task_repo, this_task->repo_entry->ht_item.key, 1);
	this_task->repo_entry->generator = (void *) this_task;	/* for AYU */
#if defined(PARSEC_SIM)
	assert(this_task->repo_entry->sim_exec_date == 0);
	this_task->repo_entry->sim_exec_date = this_task->sim_exec_date;
#endif
    }
    /* The reshape repo is the current task repo. */
    reshape_repo = Task_repo;
    reshape_entry_key =
	__jdf2c_make_key_Task((const parsec_taskpool_t *) __parsec_tp,
			      (const parsec_assignment_t *) &this_task->locals);
    reshape_entry = this_task->repo_entry;
    /* Lookup the input data, and store them in the context if any */

    if (!this_task->data._f_A.fulfill) {	/* Flow A */
	consumed_repo = NULL;
	consumed_entry_key = 0;
	consumed_entry = NULL;
	chunk = NULL;

	this_task->data._f_A.data_out = NULL;	/* By default, if nothing matches */
	if ((k) == (0)) {
	    /* Flow A [0] dependency [0] from memory Block */
	    if (NULL == (chunk = this_task->data._f_A.data_in)) {
		/* No data set up by predecessor on this task input flow */
#if defined(PARSEC_PROF_GRAPHER) && defined(PARSEC_PROF_TRACE)
		parsec_prof_grapher_data_input(data_of_Block(0, 0), (parsec_task_t *) this_task,
					       &flow_of_cache_Task_for_A, 1);
#endif
		chunk = parsec_data_get_copy(data_of_Block(0, 0), target_device);
		data.data = chunk;
		data.local.arena = NULL;
		data.local.src_datatype = PARSEC_DATATYPE_NULL;
		data.local.dst_datatype = PARSEC_DATATYPE_NULL;
		data.local.src_count = 1;
		data.local.dst_count = 1;
		data.local.src_displ = 0;
		data.local.dst_displ = 0;
		data.data_future = NULL;
		if ((ret =
		     parsec_get_copy_reshape_from_desc(es, this_task->taskpool, (parsec_task_t *) this_task, 0,
						       reshape_repo, reshape_entry_key, &data, &chunk)) < 0) {
		    return ret;
		}
		this_task->data._f_A.data_out = chunk;
		PARSEC_OBJ_RETAIN(chunk);
	    }
	} else {
	    /* Flow A [0] dependency [0] from predecessor Task */
	    if (NULL == (chunk = this_task->data._f_A.data_in)) {
		/* No data set up by predecessor on this task input flow */
		__parsec_cache_Task_parsec_assignment_t *target_locals =
		    (__parsec_cache_Task_parsec_assignment_t *) & generic_locals;
		const int Taskk = target_locals->k.value = (k - 1);
		(void) Taskk;
		if ((reshape_entry != NULL) && (reshape_entry->data[0] != NULL)) {
		    /* Reshape promise set up on this task repo by predecessor */
		    consumed_repo = reshape_repo;
		    consumed_entry = reshape_entry;
		    consumed_entry_key = reshape_entry_key;
		    consumed_flow_index = 0;
		} else {
		    /* Consume from predecessor's repo */
		    consumed_repo = Task_repo;
		    consumed_entry_key =
			__jdf2c_make_key_Task((const parsec_taskpool_t *) __parsec_tp,
					      (const parsec_assignment_t *) target_locals);
		    consumed_entry = data_repo_lookup_entry(consumed_repo, consumed_entry_key);
		    consumed_flow_index = 0;
		}
		data.data = NULL;
		data.data_future = (parsec_datacopy_future_t *) consumed_entry->data[consumed_flow_index];
		data.local.arena = NULL;
		data.local.src_datatype = PARSEC_DATATYPE_NULL;
		data.local.dst_datatype = PARSEC_DATATYPE_NULL;
		data.local.src_count = 1;
		data.local.dst_count = 1;
		data.local.src_displ = 0;
		data.local.dst_displ = 0;
		if ((ret =
		     parsec_get_copy_reshape_from_dep(es, this_task->taskpool, (parsec_task_t *) this_task, 0,
						      reshape_repo, reshape_entry_key, &data, &chunk)) < 0) {
		    return ret;
		}
		ACQUIRE_FLOW(this_task, "A", &cache_Task, "A", target_locals, chunk);
		this_task->data._f_A.data_out = chunk;
	    } else {
		/* Data set up by predecessor on this task input flow */
		consumed_repo = this_task->data._f_A.source_repo;
		consumed_entry = this_task->data._f_A.source_repo_entry;
		consumed_entry_key = this_task->data._f_A.source_repo_entry->ht_item.key;
		if ((reshape_entry != NULL) && (reshape_entry->data[0] != NULL)) {
		    /* Reshape promise set up on input by predecessor is on this task repo */
		    consumed_flow_index = 0;
		    assert((this_task->data._f_A.source_repo == reshape_repo)
			   && (this_task->data._f_A.source_repo_entry == reshape_entry));
		} else {
		    /* Reshape promise set up on input by predecessor is the predecesssor task repo */
		    consumed_flow_index = 0;
		}
		data.data = NULL;
		data.data_future = (parsec_datacopy_future_t *) consumed_entry->data[consumed_flow_index];
		data.local.arena = NULL;
		data.local.src_datatype = PARSEC_DATATYPE_NULL;
		data.local.dst_datatype = PARSEC_DATATYPE_NULL;
		data.local.src_count = 1;
		data.local.dst_count = 1;
		data.local.src_displ = 0;
		data.local.dst_displ = 0;
		if ((ret =
		     parsec_get_copy_reshape_from_dep(es, this_task->taskpool, (parsec_task_t *) this_task, 0,
						      reshape_repo, reshape_entry_key, &data, &chunk)) < 0) {
		    return ret;
		}
		this_task->data._f_A.data_out = parsec_data_get_copy(chunk->original, target_device);
#if defined(PARSEC_PROF_GRAPHER) && defined(PARSEC_PROF_TRACE)
		parsec_prof_grapher_data_input(chunk->original, (parsec_task_t *) this_task, &flow_of_cache_Task_for_A,
					       0);
#endif
	    }
	}
	this_task->data._f_A.data_in = chunk;
	this_task->data._f_A.source_repo = consumed_repo;
	this_task->data._f_A.source_repo_entry = consumed_entry;
	if (this_task->data._f_A.source_repo_entry == this_task->repo_entry) {
	    /* in case we have consume from this task repo entry for the flow,
	     * it is cleaned up, avoiding having old stuff during release_deps_of
	     */
	    this_task->repo_entry->data[0] = NULL;
	}
	this_task->data._f_A.fulfill = 1;
    }

  /** Generate profiling information */
#if defined(PARSEC_PROF_TRACE)
    this_task->prof_info.desc = (parsec_data_collection_t *) __parsec_tp->super._g_Block;
    this_task->prof_info.priority = this_task->priority;
    this_task->prof_info.data_id =
	((parsec_data_collection_t *) __parsec_tp->super._g_Block)->data_key((parsec_data_collection_t *) __parsec_tp->
									     super._g_Block, 0, 0);
    this_task->prof_info.task_class_id = this_task->task_class->task_class_id;
    this_task->prof_info.task_return_code = -1;
#endif /* defined(PARSEC_PROF_TRACE) */
    return PARSEC_HOOK_RETURN_DONE;
}

static int datatype_lookup_of_cache_Task(parsec_execution_stream_t * es, const __parsec_cache_Task_task_t * this_task,
					 uint32_t * flow_mask, parsec_dep_data_description_t * data)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) this_task->taskpool;
    (void) __parsec_tp;
    (void) es;
    (void) this_task;
    (void) data;
    const int k = this_task->locals.k.value;
    (void) k;
    data->local.arena = data->remote.arena = NULL;
    data->local.src_datatype = data->local.dst_datatype = PARSEC_DATATYPE_NULL;
    data->local.src_count = data->local.dst_count = 0;
    data->local.src_displ = data->local.dst_displ = 0;
    data->data_future = NULL;
    if ((*flow_mask) & 0x80000000U) {	/* these are the input dependencies remote datatypes  */
	if ((*flow_mask) & 0x1U) {	/* Flow A */
	    if (((*flow_mask) & 0x1U)) {	/* Have unconditional! */
		data->remote.arena = PARSEC_cache_DEFAULT_ADT->arena;
		data->remote.src_datatype = PARSEC_cache_DEFAULT_ADT->opaque_dtt;
		data->remote.dst_datatype = PARSEC_cache_DEFAULT_ADT->opaque_dtt;
		data->remote.src_count = 1;
		data->remote.dst_count = 1;
		data->remote.src_displ = 0;
		data->remote.dst_displ = 0;
		(*flow_mask) &= ~0x1U;
		return PARSEC_HOOK_RETURN_NEXT;
	    }
	}			/* (flow_mask & 0x1U) */
	goto no_mask_match;
    }

    /* these are the output dependencies remote datatypes */
    if ((*flow_mask) & 0x1U) {	/* Flow A */
	if (((*flow_mask) & 0x1U)
	    && ((k) < (N))) {
	    data->remote.arena = PARSEC_cache_DEFAULT_ADT->arena;
	    data->remote.src_datatype = PARSEC_cache_DEFAULT_ADT->opaque_dtt;
	    data->remote.dst_datatype = PARSEC_cache_DEFAULT_ADT->opaque_dtt;
	    data->remote.src_count = 1;
	    data->remote.dst_count = 1;
	    data->remote.src_displ = 0;
	    data->remote.dst_displ = 0;
	    (*flow_mask) &= ~0x1U;
	    return PARSEC_HOOK_RETURN_NEXT;
	}
    }				/* (flow_mask & 0x1U) */
  no_mask_match:
    data->data = NULL;
    data->local.arena = data->remote.arena = NULL;
    data->local.src_datatype = data->local.dst_datatype = PARSEC_DATATYPE_NULL;
    data->remote.src_datatype = data->remote.dst_datatype = PARSEC_DATATYPE_NULL;
    data->local.src_count = data->local.dst_count = 0;
    data->remote.src_count = data->remote.dst_count = 0;
    data->local.src_displ = data->local.dst_displ = 0;
    data->remote.src_displ = data->remote.dst_displ = 0;
    data->data_future = NULL;
    (*flow_mask) = 0;		/* nothing left */
    (void) k;
    return PARSEC_HOOK_RETURN_DONE;
}

static int hook_of_cache_Task(parsec_execution_stream_t * es, __parsec_cache_Task_task_t * this_task)
{
    __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) this_task->taskpool;
    (void) es;
    (void) __parsec_tp;
    const int k = this_task->locals.k.value;
    (void) k;
    (void) k;

  /** Declare the variables that will hold the data, and all the accounting for each */
    parsec_data_copy_t *_f_A = this_task->data._f_A.data_in;
    void *A = PARSEC_DATA_COPY_GET_PTR(_f_A);
    (void) A;

  /** Update starting simulation date */
#if defined(PARSEC_SIM)
    {
	this_task->sim_exec_date = 0;
	data_repo_entry_t *eA = this_task->data._f_A.source_repo_entry;
	if ((NULL != eA) && (eA->sim_exec_date > this_task->sim_exec_date))
	    this_task->sim_exec_date = eA->sim_exec_date;
	if (this_task->task_class->sim_cost_fct != NULL) {
	    this_task->sim_exec_date += this_task->task_class->sim_cost_fct(this_task);
	}
	if (es->largest_simulation_date < this_task->sim_exec_date)
	    es->largest_simulation_date = this_task->sim_exec_date;
    }
#endif
  /** Transfer the ownership to the CPU */
#if defined(PARSEC_HAVE_CUDA)
    if (NULL != _f_A) {
	parsec_data_transfer_ownership_to_copy(_f_A->original, 0 /* device */ ,
					       PARSEC_FLOW_ACCESS_RW);
    }
#endif /* defined(PARSEC_HAVE_CUDA) */
  /** Cache Awareness Accounting */
#if defined(PARSEC_CACHE_AWARENESS)
    cache_buf_referenced(es->closest_cache, A);
#endif /* PARSEC_CACHE_AWARENESS */


#if !defined(PARSEC_PROF_DRY_BODY)

/*-----                                  Task BODY                                    -----*/

#line 28 "cache.jdf"
    {
	//parsec_matrix_block_cyclic_t* Adata = (parsec_matrix_block_cyclic_t *)A;
	printf(" Task %d \n", k);

	//for( int i = 0; i < block_size; i++)
	//    ((int*)Adata->mat)[i] += 1;
    }

#line 870 "cache.c"
/*-----                                END OF Task BODY                                -----*/



#endif /*!defined(PARSEC_PROF_DRY_BODY) */

    return PARSEC_HOOK_RETURN_DONE;
}

static int complete_hook_of_cache_Task(parsec_execution_stream_t * es, __parsec_cache_Task_task_t * this_task)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) this_task->taskpool;
#if defined(DISTRIBUTED)
    const int k = this_task->locals.k.value;
    (void) k;
#endif /* defined(DISTRIBUTED) */
    (void) es;
    (void) __parsec_tp;
    parsec_data_t *data_t_desc = NULL;
    (void) data_t_desc;
    if (NULL != this_task->data._f_A.data_out) {
#if defined(PARSEC_DEBUG_NOISIER)
	char tmp[128];
#endif
	this_task->data._f_A.data_out->version++;	/* A */
	PARSEC_DEBUG_VERBOSE(10, parsec_debug_output,
			     "Complete hook of %s: change Data copy %p to version %d at %s:%d",
			     parsec_task_snprintf(tmp, 128, (parsec_task_t *) (this_task)),
			     this_task->data._f_A.data_out, this_task->data._f_A.data_out->version, __FILE__, __LINE__);
    }
#if defined(DISTRIBUTED)
  /** If not working on distributed, there is no risk that data is not in place */
    (void) k;

#endif /* DISTRIBUTED */
#if defined(PARSEC_PROF_GRAPHER)
    parsec_prof_grapher_task((parsec_task_t *) this_task, es->th_id, es->virtual_process->vp_id,
			     __jdf2c_key_fns_Task.key_hash(this_task->task_class->
							   make_key((parsec_taskpool_t *) this_task->taskpool,
								    ((parsec_task_t *) this_task)->locals), NULL));
#endif /* defined(PARSEC_PROF_GRAPHER) */
    release_deps_of_cache_Task(es, this_task, PARSEC_ACTION_RELEASE_REMOTE_DEPS | PARSEC_ACTION_RELEASE_LOCAL_DEPS | PARSEC_ACTION_RELEASE_LOCAL_REFS | PARSEC_ACTION_RESHAPE_ON_RELEASE | 0x1,	/* mask of all dep_index */
			       NULL);
    return PARSEC_HOOK_RETURN_DONE;
}

static parsec_hook_return_t release_task_of_cache_Task(parsec_execution_stream_t * es, parsec_task_t * this_task)
{
    const __parsec_cache_internal_taskpool_t *__parsec_tp =
	(const __parsec_cache_internal_taskpool_t *) this_task->taskpool;
    parsec_hash_table_t *ht = (parsec_hash_table_t *) __parsec_tp->super.super.dependencies_array[0];
    parsec_key_t key =
	this_task->task_class->make_key((const parsec_taskpool_t *) __parsec_tp,
					(const parsec_assignment_t *) &this_task->locals);
    parsec_hashable_dependency_t *hash_dep = (parsec_hashable_dependency_t *) parsec_hash_table_remove(ht, key);
    parsec_thread_mempool_free(hash_dep->mempool_owner, hash_dep);
    return parsec_release_task_to_mempool_update_nbtasks(es, this_task);
}

static char *cache_cache_Task_internal_init_deps_key_functions_key_print(char *buffer, size_t buffer_size,
									 parsec_key_t __parsec_key_, void *user_data)
{
    uint64_t __parsec_key = (uint64_t) (uintptr_t) __parsec_key_;
    __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) user_data;
    int __jdf2c_k_min = 0;
    int k = (__parsec_key) % __parsec_tp->Task_k_range + __jdf2c_k_min;
    __parsec_key = __parsec_key / __parsec_tp->Task_k_range;
    snprintf(buffer, buffer_size, "Task(%d)", k);
    return buffer;
}

static parsec_key_fn_t cache_cache_Task_internal_init_deps_key_functions = {
    .key_equal = parsec_hash_table_generic_64bits_key_equal,
    .key_print = cache_cache_Task_internal_init_deps_key_functions_key_print,
    .key_hash = parsec_hash_table_generic_64bits_key_hash
};

/* Needs: min-max count-tasks iterate */
static int cache_Task_internal_init(parsec_execution_stream_t * es, __parsec_cache_Task_task_t * this_task)
{
    __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) this_task->taskpool;
    int32_t nb_tasks = 0, saved_nb_tasks = 0;
    int32_t __k_min = 0x7fffffff, __k_max = 0;
    int32_t __jdf2c_k_min = 0x7fffffff, __jdf2c_k_max = 0;
    __parsec_cache_Task_parsec_assignment_t assignments = {.k.value = 0 };
    int32_t k;
    int32_t __jdf2c_k_start, __jdf2c_k_end, __jdf2c_k_inc;
#if defined(PARSEC_PROF_TRACE) && defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT)
    PARSEC_PROFILING_TRACE(es->es_profile,
			   this_task->taskpool->profiling_array[2 * this_task->task_class->task_class_id],
			   0, this_task->taskpool->taskpool_id, NULL);
#endif /* defined(PARSEC_PROF_TRACE) && defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT) */
    __jdf2c_k_start = 0;
    __jdf2c_k_end = 0;
    __jdf2c_k_inc = 1;
    __k_min = parsec_imin(__jdf2c_k_start, __jdf2c_k_end);
    __k_max = parsec_imax(__jdf2c_k_start, __jdf2c_k_end);
    __jdf2c_k_min = parsec_imin(__jdf2c_k_min, __k_min);
    __jdf2c_k_max = parsec_imax(__jdf2c_k_max, __k_max);
    for (k = __jdf2c_k_start; k <= __jdf2c_k_end; k += __jdf2c_k_inc) {
	assignments.k.value = k;
	if (!Task_pred(k))
	    continue;
	nb_tasks++;
    }				/* Loop on normal range k */
    if (0 != nb_tasks) {
	(void) parsec_atomic_fetch_add_int32(&__parsec_tp->super.super.nb_tasks, nb_tasks);
    }
    saved_nb_tasks = nb_tasks;
    /* Set the range variables for the collision-free hash-computation */
    __parsec_tp->Task_k_range = (__jdf2c_k_max - __jdf2c_k_min) + 1;
    do {
	this_task->super.list_next = (parsec_list_item_t *) __parsec_tp->startup_queue;
    } while (!parsec_atomic_cas_ptr
	     (&__parsec_tp->startup_queue, (parsec_list_item_t *) this_task->super.list_next, this_task));
    this_task->status = PARSEC_TASK_STATUS_HOOK;

    PARSEC_AYU_REGISTER_TASK(&cache_Task);
    __parsec_tp->super.super.dependencies_array[0] = PARSEC_OBJ_NEW(parsec_hash_table_t);
    parsec_hash_table_init(__parsec_tp->super.super.dependencies_array[0],
			   offsetof(parsec_hashable_dependency_t, ht_item), 10,
			   cache_cache_Task_internal_init_deps_key_functions, this_task->taskpool);
    __parsec_tp->repositories[0] =
	data_repo_create_nothreadsafe(nb_tasks, __jdf2c_key_fns_Task, (parsec_taskpool_t *) __parsec_tp, 1);
    (void) saved_nb_tasks;
    (void) __k_min;
    (void) __k_max;
    (void) __jdf2c_k_start;
    (void) __jdf2c_k_end;
    (void) __jdf2c_k_inc;
    (void) assignments;
    (void) __parsec_tp;
    (void) es;
    if (1 == parsec_atomic_fetch_dec_int32(&__parsec_tp->sync_point)) {
	/* Last initialization task complete. Update the number of tasks. */
	nb_tasks = parsec_atomic_fetch_dec_int32(&__parsec_tp->super.super.nb_tasks);
	parsec_mfence();	/* write memory barrier to guarantee that the scheduler gets the correct number of tasks */
	parsec_taskpool_enable((parsec_taskpool_t *) __parsec_tp, &__parsec_tp->startup_queue,
			       (parsec_task_t *) this_task, es, (1 <= nb_tasks));
#if defined(PARSEC_PROF_TRACE) && defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT)
	PARSEC_PROFILING_TRACE(es->es_profile,
			       this_task->taskpool->profiling_array[2 * this_task->task_class->task_class_id + 1],
			       0, this_task->taskpool->taskpool_id, NULL);
#endif /* defined(PARSEC_PROF_TRACE) && defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT) */
	if (1 >= nb_tasks) {
	    /* if no tasks will be generated let's prevent the runtime from calling the hook and instead go directly to complete the task */
	    this_task->status = PARSEC_TASK_STATUS_COMPLETE;
	}
	return PARSEC_HOOK_RETURN_DONE;
    }
    return PARSEC_HOOK_RETURN_ASYNC;
}

static int __jdf2c_startup_Task(parsec_execution_stream_t * es, __parsec_cache_Task_task_t * this_task)
{
    __parsec_cache_Task_task_t *new_task;
    __parsec_cache_internal_taskpool_t *__parsec_tp = (__parsec_cache_internal_taskpool_t *) this_task->taskpool;
    parsec_context_t *context = __parsec_tp->super.super.context;
    int vpid = 0, nb_tasks = 0;
    size_t total_nb_tasks = 0;
    parsec_list_item_t *pready_ring[context->nb_vp];
    int restore_context = 0;
    int k = this_task->locals.k.value;	/* retrieve value saved during the last iteration */
    for (int _i = 0; _i < context->nb_vp; pready_ring[_i++] = NULL);
    if (0 != this_task->locals.reserved[0].value) {
	this_task->locals.reserved[0].value = 1;	/* reset the submission process */
	restore_context = 1;
	goto restore_context_0;
    }
    this_task->locals.reserved[0].value = 1;	/* a sane default value */
    for (this_task->locals.k.value = k = 0;
	 this_task->locals.k.value <= 0; this_task->locals.k.value += 1, k = this_task->locals.k.value) {
	if (!Task_pred(k))
	    continue;
	__parsec_cache_Task_parsec_assignment_t __jdf2c__tmp_locals =
	    *(__parsec_cache_Task_parsec_assignment_t *) & this_task->locals;
	(void) __jdf2c__tmp_locals;
	if ((k) == (0))
	    goto __jdf2c_done_check_flow;
	continue;		/* All other cases are not startup tasks */
      __jdf2c_done_check_flow:
	if (NULL != ((parsec_data_collection_t *) __parsec_tp->super._g_Block)->vpid_of) {
	    vpid =
		((parsec_data_collection_t *) __parsec_tp->super._g_Block)->
		vpid_of((parsec_data_collection_t *) __parsec_tp->super._g_Block, 0, 0);
	    assert(context->nb_vp >= vpid);
	}
	new_task =
	    (__parsec_cache_Task_task_t *) parsec_thread_mempool_allocate(context->virtual_processes[vpid]->
									  execution_streams[0]->context_mempool);
	new_task->status = PARSEC_TASK_STATUS_NONE;
	/* Copy only the valid elements from this_task to new_task one */
	new_task->taskpool = this_task->taskpool;
	new_task->task_class = __parsec_tp->super.super.task_classes_array[cache_Task.task_class_id];
	new_task->chore_mask = PARSEC_DEV_ALL;
	new_task->locals.k.value = this_task->locals.k.value;
	PARSEC_LIST_ITEM_SINGLETON(new_task);
	new_task->priority = __parsec_tp->super.super.priority;
	new_task->repo_entry = NULL;
	new_task->data._f_A.source_repo_entry = NULL;
	new_task->data._f_A.source_repo = NULL;
	new_task->data._f_A.data_in = NULL;
	new_task->data._f_A.data_out = NULL;
	new_task->data._f_A.fulfill = 0;
#if defined(PARSEC_DEBUG_NOISIER)
	{
	    char tmp[128];
	    PARSEC_DEBUG_VERBOSE(10, parsec_debug_output, "Add startup task %s to vpid %d",
				 parsec_task_snprintf(tmp, 128, (parsec_task_t *) new_task), vpid);
	}
#endif
	parsec_dependencies_mark_task_as_startup((parsec_task_t *) new_task, es);
	pready_ring[vpid] = parsec_list_item_ring_push_sorted(pready_ring[vpid],
							      (parsec_list_item_t *) new_task,
							      parsec_execution_context_priority_comparator);
	nb_tasks++;
      restore_context_0:	/* we jump here just so that we have code after the label */
	restore_context = 0;
	(void) restore_context;
	if (nb_tasks > this_task->locals.reserved[0].value) {
	    if ((size_t) this_task->locals.reserved[0].value < parsec_task_startup_iter)
		this_task->locals.reserved[0].value <<= 1;
	    for (int _i = 0; _i < context->nb_vp; _i++) {
		if (NULL == pready_ring[_i])
		    continue;
		__parsec_schedule(context->virtual_processes[_i]->execution_streams[0],
				  (parsec_task_t *) pready_ring[_i], 0);
		pready_ring[_i] = NULL;
	    }
	    total_nb_tasks += nb_tasks;
	    nb_tasks = 0;
	    if (total_nb_tasks > parsec_task_startup_chunk) {	/* stop here and request to be rescheduled */
		return PARSEC_HOOK_RETURN_AGAIN;
	    }
	}
    }				/* Loop on normal range k */
    (void) vpid;
    if (0 != nb_tasks) {
	for (int _i = 0; _i < context->nb_vp; _i++) {
	    if (NULL == pready_ring[_i])
		continue;
	    __parsec_schedule(context->virtual_processes[_i]->execution_streams[0],
			      (parsec_task_t *) pready_ring[_i], 0);
	    pready_ring[_i] = NULL;
	}
	nb_tasks = 0;
    }
    return PARSEC_HOOK_RETURN_DONE;
}

static const __parsec_chore_t __cache_Task_chores[] = {
    {.type = PARSEC_DEV_CPU,
     .evaluate = NULL,
     .hook = (parsec_hook_t *) hook_of_cache_Task},
    {.type = PARSEC_DEV_NONE,
     .evaluate = NULL,
     .hook = (parsec_hook_t *) NULL},	/* End marker */
};

static const parsec_task_class_t cache_Task = {
    .name = "Task",
    .task_class_id = 0,
    .nb_flows = 1,
    .nb_parameters = 1,
    .nb_locals = 1,
    .task_class_type = PARSEC_TASK_CLASS_TYPE_PTG,
    .params = {&symb_cache_Task_k, NULL},
    .locals = {&symb_cache_Task_k, NULL},
    .data_affinity = (parsec_data_ref_fn_t *) affinity_of_cache_Task,
    .initial_data = (parsec_data_ref_fn_t *) affinity_of_cache_Task,
    .final_data = (parsec_data_ref_fn_t *) affinity_of_cache_Task,
    .priority = NULL,
    .properties = properties_of_cache_Task,
#if MAX_PARAM_COUNT < 1		/* number of read flows of Task */
#error Too many read flows for task Task
#endif /* MAX_PARAM_COUNT */
#if MAX_PARAM_COUNT < 1		/* number of write flows of Task */
#error Too many write flows for task Task
#endif /* MAX_PARAM_COUNT */
    .in = {&flow_of_cache_Task_for_A, NULL},
    .out = {&flow_of_cache_Task_for_A, NULL},
    .flags = 0x0 | PARSEC_HAS_IN_IN_DEPENDENCIES | PARSEC_USE_DEPS_MASK,
    .dependencies_goal = 0x1,
    .make_key = __jdf2c_make_key_Task,
    .key_functions = &__jdf2c_key_fns_Task,
    .fini = (parsec_hook_t *) NULL,
    .incarnations = __cache_Task_chores,
    .find_deps = parsec_hash_find_deps,
    .iterate_successors = (parsec_traverse_function_t *) iterate_successors_of_cache_Task,
    .iterate_predecessors = (parsec_traverse_function_t *) iterate_predecessors_of_cache_Task,
    .release_deps = (parsec_release_deps_t *) release_deps_of_cache_Task,
    .prepare_input = (parsec_hook_t *) data_lookup_of_cache_Task,
    .prepare_output = (parsec_hook_t *) NULL,
    .get_datatype = (parsec_datatype_lookup_t *) datatype_lookup_of_cache_Task,
    .complete_execution = (parsec_hook_t *) complete_hook_of_cache_Task,
    .release_task = &release_task_of_cache_Task,
#if defined(PARSEC_SIM)
    .sim_cost_fct = (parsec_sim_cost_fct_t *) NULL,
#endif
};


static const parsec_task_class_t *cache_task_classes[] = {
    &cache_Task
};

static void cache_startup(parsec_context_t * context, __parsec_cache_internal_taskpool_t * __parsec_tp,
			  parsec_list_item_t ** ready_tasks)
{
    uint32_t i, supported_dev = 0;

    for (i = 0; i < parsec_nb_devices; i++) {
	if (!(__parsec_tp->super.super.devices_index_mask & (1 << i)))
	    continue;
	parsec_device_module_t *device = parsec_mca_device_get(i);
	parsec_data_collection_t *parsec_dc;

	if (NULL == device)
	    continue;
	if (NULL != device->taskpool_register)
	    if (PARSEC_SUCCESS != device->taskpool_register(device, (parsec_taskpool_t *) __parsec_tp)) {
		parsec_debug_verbose(5, parsec_debug_output, "Device %s refused to register taskpool %p", device->name,
				     __parsec_tp);
		__parsec_tp->super.super.devices_index_mask &= ~(1 << device->device_index);
		continue;
	    }
	if (NULL != device->memory_register) {	/* Register all the data */
	    parsec_dc = (parsec_data_collection_t *) __parsec_tp->super._g_Block;
	    if ((NULL != parsec_dc->register_memory) &&
		(PARSEC_SUCCESS != parsec_dc->register_memory(parsec_dc, device))) {
		parsec_debug_verbose(3, parsec_debug_output,
				     "Device %s refused to register memory for data %s (%p) from taskpool %p",
				     device->name, parsec_dc->key_base, parsec_dc, __parsec_tp);
		__parsec_tp->super.super.devices_index_mask &= ~(1 << device->device_index);
	    }
	}
	supported_dev |= device->type;
    }
    /* Remove all the chores without a backend device */
    for (i = 0; i < PARSEC_cache_NB_TASK_CLASSES; i++) {
	parsec_task_class_t *tc = (parsec_task_class_t *) __parsec_tp->super.super.task_classes_array[i];
	__parsec_chore_t *chores = (__parsec_chore_t *) tc->incarnations;
	uint32_t idx = 0, j;
	for (j = 0; NULL != chores[j].hook; j++) {
	    if (!(supported_dev & chores[j].type))
		continue;
	    if (j != idx) {
		chores[idx] = chores[j];
		parsec_debug_verbose(20, parsec_debug_output, "Device type %i disabledfor function %s", chores[j].type,
				     tc->name);
	    }
	    idx++;
	}
	chores[idx].type = PARSEC_DEV_NONE;
	chores[idx].evaluate = NULL;
	chores[idx].hook = NULL;
	parsec_task_t *task =
	    (parsec_task_t *) parsec_thread_mempool_allocate(context->virtual_processes[0]->execution_streams[0]->
							     context_mempool);
	task->taskpool = (parsec_taskpool_t *) __parsec_tp;
	task->chore_mask = PARSEC_DEV_ALL;
	task->status = PARSEC_TASK_STATUS_NONE;
	memset(&task->locals, 0, sizeof(parsec_assignment_t) * MAX_LOCAL_COUNT);
	PARSEC_LIST_ITEM_SINGLETON(task);
	task->priority = -1;
	task->task_class = task->taskpool->task_classes_array[PARSEC_cache_NB_TASK_CLASSES + i];
	int where = i % context->nb_vp;
	if (NULL == ready_tasks[where])
	    ready_tasks[where] = &task->super;
	else
	    ready_tasks[where] = parsec_list_item_ring_push(ready_tasks[where], &task->super);
    }
}

static void __parsec_cache_internal_destructor(__parsec_cache_internal_taskpool_t * __parsec_tp)
{
    uint32_t i;
    parsec_taskpool_unregister(&__parsec_tp->super.super);
    for (i = 0; i < (uint32_t) (2 * __parsec_tp->super.super.nb_task_classes); i++) {	/* Extra startup function added at the end */
	parsec_task_class_t *tc = (parsec_task_class_t *) __parsec_tp->super.super.task_classes_array[i];
	free((void *) tc->incarnations);
	free(tc);
    }
    free(__parsec_tp->super.super.task_classes_array);
    __parsec_tp->super.super.task_classes_array = NULL;
    __parsec_tp->super.super.nb_task_classes = 0;

    for (i = 0; i < (uint32_t) __parsec_tp->super.arenas_datatypes_size; i++) {
	if (NULL != __parsec_tp->super.arenas_datatypes[i].arena) {
	    PARSEC_OBJ_RELEASE(__parsec_tp->super.arenas_datatypes[i].arena);
	}
    }
    /* Destroy the data repositories for this object */
    data_repo_destroy_nothreadsafe(__parsec_tp->repositories[0]);	/* Task */
    /* Release the dependencies arrays for this object */
    parsec_hash_table_fini((parsec_hash_table_t *) __parsec_tp->super.super.dependencies_array[0]);
    PARSEC_OBJ_RELEASE(__parsec_tp->super.super.dependencies_array[0]);
    __parsec_tp->super.super.dependencies_array[0] = NULL;
    free(__parsec_tp->super.super.dependencies_array);
    __parsec_tp->super.super.dependencies_array = NULL;
    /* Unregister all the data */
    uint32_t _i;
    for (_i = 0; _i < parsec_nb_devices; _i++) {
	parsec_device_module_t *device;
	parsec_data_collection_t *parsec_dc;
	if (!(__parsec_tp->super.super.devices_index_mask & (1 << _i)))
	    continue;
	if ((NULL == (device = parsec_mca_device_get(_i))) || (NULL == device->memory_unregister))
	    continue;
	parsec_dc = (parsec_data_collection_t *) __parsec_tp->super._g_Block;
	if (NULL != parsec_dc->unregister_memory) {
	    (void) parsec_dc->unregister_memory(parsec_dc, device);
	};
    }
    /* Unregister the taskpool from the devices */
    for (i = 0; i < parsec_nb_devices; i++) {
	if (!(__parsec_tp->super.super.devices_index_mask & (1 << i)))
	    continue;
	__parsec_tp->super.super.devices_index_mask ^= (1 << i);
	parsec_device_module_t *device = parsec_mca_device_get(i);
	if ((NULL == device) || (NULL == device->taskpool_unregister))
	    continue;
	if (PARSEC_SUCCESS != device->taskpool_unregister(device, &__parsec_tp->super.super))
	    continue;
    }
    free(__parsec_tp->super.super.taskpool_name);
    __parsec_tp->super.super.taskpool_name = NULL;
}

void __parsec_cache_internal_constructor(__parsec_cache_internal_taskpool_t * __parsec_tp)
{
    parsec_task_class_t *tc;
    uint32_t i, j;

    __parsec_tp->super.super.nb_task_classes = PARSEC_cache_NB_TASK_CLASSES;
    __parsec_tp->super.super.devices_index_mask = PARSEC_DEVICES_ALL;
    __parsec_tp->super.super.taskpool_name = strdup("cache");
    __parsec_tp->super.super.update_nb_runtime_task = parsec_add_fetch_runtime_task;
    __parsec_tp->super.super.dependencies_array = (void **)
	calloc(__parsec_tp->super.super.nb_task_classes, sizeof(void *));
    /* Twice the size to hold the startup tasks function_t */
    __parsec_tp->super.super.task_classes_array = (const parsec_task_class_t **)
	malloc(2 * __parsec_tp->super.super.nb_task_classes * sizeof(parsec_task_class_t *));
    __parsec_tp->super.super.nb_tasks = 1;
    __parsec_tp->super.super.taskpool_type = PARSEC_TASKPOOL_TYPE_PTG;
    __parsec_tp->super.super.nb_pending_actions = 1 + __parsec_tp->super.super.nb_task_classes;	/* for the startup tasks */
    __parsec_tp->sync_point = __parsec_tp->super.super.nb_task_classes;
    __parsec_tp->startup_queue = NULL;
    for (i = 0; i < __parsec_tp->super.super.nb_task_classes; i++) {
	__parsec_tp->super.super.task_classes_array[i] = tc = malloc(sizeof(parsec_task_class_t));
	memcpy(tc, cache_task_classes[i], sizeof(parsec_task_class_t));
	for (j = 0; NULL != tc->incarnations[j].hook; j++);	/* compute the number of incarnations */
	tc->incarnations = (__parsec_chore_t *) malloc((j + 1) * sizeof(__parsec_chore_t));
	memcpy((__parsec_chore_t *) tc->incarnations, cache_task_classes[i]->incarnations,
	       (j + 1) * sizeof(__parsec_chore_t));

	/* Add a placeholder for initialization and startup task */
	__parsec_tp->super.super.task_classes_array[__parsec_tp->super.super.nb_task_classes + i] = tc =
	    (parsec_task_class_t *) malloc(sizeof(parsec_task_class_t));
	memcpy(tc, (void *) &__parsec_generic_startup, sizeof(parsec_task_class_t));
	tc->task_class_id = __parsec_tp->super.super.nb_task_classes + i;
	tc->incarnations = (__parsec_chore_t *) malloc(2 * sizeof(__parsec_chore_t));
	memcpy((__parsec_chore_t *) tc->incarnations, (void *) __parsec_generic_startup.incarnations,
	       2 * sizeof(__parsec_chore_t));
	tc->release_task = parsec_release_task_to_mempool_and_count_as_runtime_tasks;
    }
    /* Startup task for Task */
    tc = (parsec_task_class_t *) __parsec_tp->super.super.task_classes_array[__parsec_tp->super.super.nb_task_classes +
									     0];
    tc->name = "Startup for Task";
    tc->prepare_input = (parsec_hook_t *) cache_Task_internal_init;
    ((__parsec_chore_t *) & tc->incarnations[0])->hook = (parsec_hook_t *) __jdf2c_startup_Task;
    /* Compute the number of arenas_datatypes: */
    /*   PARSEC_cache_DEFAULT_ARENA  ->  0 */
    __parsec_tp->super.arenas_datatypes_size = 1;
    memset(&__parsec_tp->super.arenas_datatypes[0], 0,
	   __parsec_tp->super.arenas_datatypes_size * sizeof(parsec_arena_datatype_t));
    /* If profiling is enabled, the keys for profiling */
#if defined(PARSEC_PROF_TRACE)
    __parsec_tp->super.super.profiling_array = cache_profiling_array;
    if (-1 == cache_profiling_array[0]) {
#if defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT)
	parsec_profiling_add_dictionary_keyword("cache::Task::internal init", "fill:34D8D8",
						0,
						NULL,
						(int *) &__parsec_tp->super.super.profiling_array[0 +
												  2 *
												  cache_Task.
												  task_class_id +
												  2 *
												  PARSEC_cache_NB_TASK_CLASSES
												  /* Task (internal init) start key */
												  ],
						(int *) &__parsec_tp->super.super.profiling_array[1 +
												  2 *
												  cache_Task.
												  task_class_id +
												  2 *
												  PARSEC_cache_NB_TASK_CLASSES
												  /* Task (internal init) end key */
												  ]);
#endif /* defined(PARSEC_PROF_TRACE_PTG_INTERNAL_INIT) */
	parsec_profiling_add_dictionary_keyword("cache::Task", "fill:CC2828",
						sizeof(parsec_task_prof_info_t) + 1 * sizeof(parsec_assignment_t),
						"dc_key{uint64_t};priority{int32_t};dc_dataid{uint32_t};tcid{int32_t};trc{int32_t};k{int32_t}",
						(int *) &__parsec_tp->super.super.profiling_array[0 +
												  2 *
												  cache_Task.
												  task_class_id
												  /* Task start key */
												  ],
						(int *) &__parsec_tp->super.super.profiling_array[1 +
												  2 *
												  cache_Task.
												  task_class_id
												  /* Task end key */ ]);

    }
#endif /* defined(PARSEC_PROF_TRACE) */
    __parsec_tp->super.super.repo_array = __parsec_tp->repositories;
    __parsec_tp->super.super.startup_hook = (parsec_startup_fn_t) cache_startup;
    (void) parsec_taskpool_reserve_id((parsec_taskpool_t *) __parsec_tp);
}

  /** Generate class declaration and instance using the above constructor and destructor */
PARSEC_OBJ_CLASS_DECLARATION(__parsec_cache_internal_taskpool_t);
PARSEC_OBJ_CLASS_INSTANCE(__parsec_cache_internal_taskpool_t, parsec_cache_taskpool_t,
			  __parsec_cache_internal_constructor, __parsec_cache_internal_destructor);

#undef Block
#undef block_size
#undef N

parsec_cache_taskpool_t *parsec_cache_new(parsec_data_collection_t * Block /* data Block */ , int block_size, int N)
{
    __parsec_cache_internal_taskpool_t *__parsec_tp = PARSEC_OBJ_NEW(__parsec_cache_internal_taskpool_t);
    /* Dump the hidden parameters with default values */

    /* Now the Parameter-dependent structures: */
    __parsec_tp->super._g_Block = Block;
    __parsec_tp->super._g_block_size = block_size;
    __parsec_tp->super._g_N = N;
    PARSEC_AYU_REGISTER_TASK(&cache_Task);
    __parsec_tp->super.super.startup_hook = (parsec_startup_fn_t) cache_startup;
    (void) parsec_taskpool_reserve_id((parsec_taskpool_t *) __parsec_tp);
/* Prevent warnings related to not used hidden global variables */
    ;
    return (parsec_cache_taskpool_t *) __parsec_tp;
}

#line 40 "cache.jdf"

#define TYPE  PARSEC_MATRIX_INTEGER

int main(int argc, char *argv[])
{
    parsec_cache_taskpool_t *tp;
    parsec_context_t *parsec;
    int block_size = 10, N = 1, rc, world, rank;
    parsec_matrix_block_cyclic_t descA;



#if defined(PARSEC_HAVE_MPI)
    {
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    }
    MPI_Comm_size(MPI_COMM_WORLD, &world);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#else
    world = 1;
    rank = 0;
#endif

    block_size = atoi(argv[1]);
    int worker_threads = atoi(argv[2]);

    printf("Block size = %d worker threads = %d world %d \n", block_size, worker_threads, world);

    parsec = parsec_init(worker_threads, &argc, &argv);

    parsec_matrix_block_cyclic_init(&descA, TYPE, PARSEC_MATRIX_TILE, 0,	/* rank */
				    block_size, 1,	/* Tile size */
				    block_size, 1,	/* Global matrix size (what is stored) */
				    0, 0,	/* Staring point in the global matrix */
				    block_size, 1,	/* Submatrix size (the one concerned by the computation */
				    world, 1,	/* process process grid */
				    1, 1,	/* k-cyclicity */
				    0, 0);	/* starting point on the process grid */

    descA.mat = parsec_data_allocate(block_size * parsec_datadist_getsizeoftype(TYPE));
    //for( int i = 0; i < block_size; ((int*)descA.mat)[i++] = 1);


    tp = parsec_cache_new((parsec_data_collection_t *) & descA, block_size, N);

    parsec_arena_datatype_construct(&tp->arenas_datatypes[PARSEC_cache_DEFAULT_ADT_IDX], block_size * parsec_datadist_getsizeoftype(TYPE), PARSEC_ARENA_ALIGNMENT_SSE, PARSEC_DATATYPE_NULL);	/* change for distributed cases */

    rc = parsec_context_add_taskpool(parsec, (parsec_taskpool_t *) tp);

    rc = parsec_context_start(parsec);
    PARSEC_CHECK_ERROR(rc, "parsec_context_start");

    rc = parsec_context_add_taskpool(parsec, (parsec_taskpool_t *) tp);
    PARSEC_CHECK_ERROR(rc, "parsec_context_add_taskpool");

    rc = parsec_context_wait(parsec);
    PARSEC_CHECK_ERROR(rc, "parsec_context_wait");

    free(descA.mat);

#ifdef PARSEC_HAVE_MPI
    MPI_Finalize();
#endif
    return 0;
}

#line 1489 "cache.c"
PARSEC_OBJ_CLASS_INSTANCE(parsec_cache_taskpool_t, parsec_taskpool_t, NULL, NULL);
