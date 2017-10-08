#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "spfg/spfg.h"
#include "spfg_types.h"
#include "spfg_utils.h"
#include "spfg_index.h"

extern spfg_gr_t global_grs[SPFG_MAX_GRID_CNT];
extern spfg_grx_t global_grxs[SPFG_MAX_GRID_CNT];


spfg_err_t fn_signature_validate(spfg_dp_t *in_dps[], uint8_t in_dps_len,
                                 spfg_dp_t *out_dps[], uint8_t out_dps_len,
                                 spfg_dp_type_t *in_dp_types, uint8_t in_dp_types_len,
                                 spfg_dp_type_t *out_dp_types, uint8_t out_dp_types_len,
                                 const char *fn_name) {

    uint8_t cnt = 0;

    if (in_dps_len < in_dp_types_len) {
        fprintf(stderr, "%d missing input datapoints for function '%s' expecting %d input datapoints\n", in_dp_types_len - in_dps_len, fn_name, in_dp_types_len);
        return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
    }

    for (cnt = 0; cnt < in_dps_len && cnt < SPFG_MAX_FN_IN_DPS; cnt++) {

        if (cnt >= in_dp_types_len) {
            fprintf(stderr, "unexpected input datapoint '%s' (position %d) for function '%s' expecting only %d input datapoints\n", in_dps[cnt]->name.chars, cnt, fn_name, in_dp_types_len);
            return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
        }

        if (in_dps[cnt]->type != in_dp_types[cnt]) {
            fprintf(stderr, "input datapoint '%s' type %d is incompatible with expected input type %d (position %d) for function '%s'\n", in_dps[cnt]->name.chars, in_dps[cnt]->type, in_dp_types[cnt], cnt, fn_name);
            return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
        }
    }

    if (out_dps_len < out_dp_types_len) {
        fprintf(stderr, "%d missing output datapoints for function '%s' expecting %d output datapoints\n", out_dp_types_len - out_dps_len, fn_name, out_dp_types_len);
        return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
    }

    for (cnt = 0; cnt < out_dps_len && cnt < SPFG_MAX_FN_OUT_DPS; cnt++) {

        if (cnt >= out_dp_types_len) {
            fprintf(stderr, "unexpected output datapoint '%s' (position %d) for function '%s' expecting only %d output datapoints\n", out_dps[cnt]->name.chars, cnt, fn_name, out_dp_types_len);
            return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
        }

        if (out_dps[cnt]->type != out_dp_types[cnt]) {
            fprintf(stderr, "output datapoint '%s' type %d is incompatible with expected output type %d (position %d) for function '%s'\n", out_dps[cnt]->name.chars, out_dps[cnt]->type, out_dp_types[cnt], cnt, fn_name);
            return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
        }
    }

    return SPFG_ERROR_NO;
}

spfg_err_t fn_validate(spfg_fn_type_t type,
                       spfg_dp_t *in_dps[], uint8_t in_dps_len,
                       spfg_dp_t *out_dps[], uint8_t out_dps_len,
                       const char *fn_name) {

    switch (type) {

        case SPFG_FN_INVERT_BOOL_RET_BOOL:
        {
            return SPFG_ERROR_UNIMPLEMENTED;
        }

        case SPFG_FN_AND_BOOL_BOOL_RET_BOOL:
        {
            spfg_dp_type_t in_dp_types[] = {SPFG_DP_BOOL, SPFG_DP_BOOL};
            spfg_dp_type_t out_dp_types[] = {SPFG_DP_BOOL};
            return fn_signature_validate(
                in_dps, in_dps_len,
                out_dps, out_dps_len,
                in_dp_types, 2,
                out_dp_types, 1,
                fn_name);
        }

        default:
        {
            return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
        }
    }
}

// ------------------------------------------------------------------------------------------------

spfg_err_t _spfg_gr_create(spfg_gr_id_t *gr_id, const char *name)
{
    spfg_gr_t *gr;
    unsigned int gr_idx;

    if (_spfg_find_free_gr(&gr_idx, &gr) != SPFG_ERROR_NO) {
        return SPFG_ERROR_OUT_OF_SLOTS;
    }

    if (create_name(name, &gr->name) != SPFG_ERROR_NO) {
        return SPFG_ERROR_BAD_BLOCK_NAME;
    }

    global_grxs[gr_idx].gr = gr;
    gr->id = gr_idx + SPFG_GR_ID0;

    (void) _spfg_gr_index_clear(gr);

    *gr_id = gr->id;

    return SPFG_ERROR_NO;
}


spfg_err_t _spfg_gr_remove(spfg_gr_t *gr)
{
    (void) _spfg_gr_index_clear(gr);

    memset(gr, 0, sizeof(spfg_gr_t));

    return SPFG_ERROR_NO;
}


spfg_err_t _spfg_dp_create(spfg_gr_t *gr, spfg_dp_type_t dp_type, const char *name, spfg_dp_id_t *dp_id)
{
    unsigned int dp_idx;
    spfg_dp_t *dp;

    if (_spfg_find_free_gr_dp(gr, &dp_idx, &dp) != SPFG_ERROR_NO) {
        return SPFG_ERROR_OUT_OF_SLOTS;
    }

    // TODO: dp type validation

    if (create_name(name, &gr->dps[dp_idx].name) != SPFG_ERROR_NO) {
        return SPFG_ERROR_BAD_BLOCK_NAME;
    }

    gr->dps[dp_idx].id = GEN_SPFG_DP_ID(gr->id, dp_idx);
    gr->dps[dp_idx].type = dp_type;

    (void) _spfg_gr_index_clear(gr);

    *dp_id = dp->id;

    return SPFG_ERROR_NO;
}


spfg_err_t _spfg_dp_remove(spfg_gr_t *gr, spfg_dp_t *dp)
{
    memset(dp, 0, sizeof(spfg_dp_t));

    (void) _spfg_gr_index_clear(gr);

    return SPFG_ERROR_NO;
}


spfg_err_t _spfg_fn_create(spfg_gr_t *gr,
                           spfg_fn_type_t type,
                           spfg_phase_t phase,
                           spfg_dp_id_t in_dp_ids[], uint8_t in_dp_ids_len,
                           spfg_dp_id_t out_dp_ids[], uint8_t out_dp_ids_len,
                           const char *name,
                           spfg_fn_id_t *fn_id)
{
    spfg_err_t err = SPFG_ERROR_NO;

    // Validate datapoints against target function signature.

    spfg_dp_t *in_dps[SPFG_MAX_FN_IN_DPS];
    spfg_dp_t *out_dps[SPFG_MAX_FN_OUT_DPS];

    if ((err = _spfg_resolve_gr_dps(gr, in_dp_ids, in_dps, in_dp_ids_len)) != SPFG_ERROR_NO) {
    return SPFG_ERROR_INVALID_DP_ID;
    }

    if ((err = _spfg_resolve_gr_dps(gr, out_dp_ids, out_dps, out_dp_ids_len)) != SPFG_ERROR_NO) {
    return SPFG_ERROR_INVALID_DP_ID;
    }

    if ((err = fn_validate(type, in_dps, in_dp_ids_len, out_dps, out_dp_ids_len, name)) != SPFG_ERROR_NO) {
    return SPFG_ERROR_VALIDATE_FN;
    }

    // Evolve grid schema.

    unsigned int fn_idx;
    spfg_fn_t *fn;

    if ((err = _spfg_find_free_gr_fn(gr, &fn_idx, &fn)) != SPFG_ERROR_NO) {
    return SPFG_ERROR_OUT_OF_SLOTS;
    }

    if ((err = create_name(name, &fn->name)) != SPFG_ERROR_NO) {
    return SPFG_ERROR_BAD_BLOCK_NAME;
    }

    fn->type = type;
    fn->phase = phase;
    fn->id = GEN_SPFG_FN_ID(gr->id, fn_idx);

    memcpy(fn->in_dp_ids, in_dp_ids, in_dp_ids_len * sizeof(spfg_dp_id_t));
    fn->in_dp_ids_len = in_dp_ids_len;

    memcpy(fn->out_dp_ids, out_dp_ids, out_dp_ids_len * sizeof(spfg_dp_id_t));
    fn->out_dp_ids_len = out_dp_ids_len;

    (void) _spfg_gr_index_clear(gr);

    *fn_id = fn->id;

    return SPFG_ERROR_NO;
}


spfg_err_t _spfg_fn_remove(spfg_gr_t *gr, spfg_fn_t *fn)
{
    memset(fn, 0, sizeof(spfg_fn_t));

    (void) _spfg_gr_index_clear(gr);

    return SPFG_ERROR_NO;
}
