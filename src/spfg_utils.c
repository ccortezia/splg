#include <string.h>
#include <stdio.h>
#include "spfg/spfg.h"
#include "spfg_types.h"
#include "spfg_utils.h"


spfg_err_t _spfg_name_set(spfg_name_t *name, const char *ascii)
{
    if (!*ascii) {
        return SPFG_ERROR_BAD_PARAM_INVALID_VALUE;
    }

    if (strnlen(ascii, sizeof(name->chars)) >= sizeof(name->chars)) {
        return SPFG_ERROR_BUFFER_OVERFLOW;
    }

    memset(name->chars, 0, sizeof(name->chars));
    strcpy(name->chars, ascii);
    name->chars[sizeof(name->chars) - 1] = 0;
    return SPFG_ERROR_NO;
}

spfg_err_t _spfg_resolve_gr_dp(spfg_gr_t *gr, spfg_dp_id_t dp_id, spfg_dp_t **dp)
{
    spfg_gr_dp_cnt_t dp_idx = dp_id - SPFG_GR_DP_ID0;

    if (dp_idx >= SPFG_MAX_GRID_DPS) {
        return SPFG_ERROR_NOT_FOUND;
    }

    if (!gr->dps[dp_idx].name.chars[0]) {
        return SPFG_ERROR_NOT_FOUND;
    }

    *dp = &gr->dps[dp_idx];

    return SPFG_ERROR_NO;
}

spfg_err_t _spfg_resolve_gr_fn(spfg_gr_t *gr, spfg_fn_id_t fn_id, spfg_fn_t **fn)
{
    spfg_gr_fn_cnt_t fn_idx = fn_id - SPFG_GR_DP_ID0;

    if (fn_idx >= SPFG_MAX_GRID_FNS) {
        return SPFG_ERROR_NOT_FOUND;
    }

    if (!gr->fns[fn_idx].name.chars[0]) {
        return SPFG_ERROR_NOT_FOUND;
    }

    *fn = &gr->fns[fn_idx];

    return SPFG_ERROR_NO;
}

spfg_err_t _spfg_resolve_gr_dps(spfg_gr_t *gr, spfg_dp_id_t *dp_ids, spfg_dp_t *dps[], uint8_t length)
{
    for (spfg_gr_dp_cnt_t i = 0; dp_ids[i] && i < length; i++) {

        if (_spfg_resolve_gr_dp(gr, dp_ids[i], &dps[i]) != SPFG_ERROR_NO) {
            return SPFG_ERROR_INVALID_DP_ID;
        }
    }

    return SPFG_ERROR_NO;
}


spfg_err_t _spfg_find_free_gr_dp(spfg_gr_t *gr, uint32_t *idx, spfg_dp_t **dp)
{
    for (spfg_gr_dp_cnt_t i = 0; i < SPFG_MAX_GRID_DPS; i++) {

        if (!gr->dps[i].name.chars[0]) {
            *idx = i;

            if (dp) {
                *dp = &gr->dps[i];
            }

            return SPFG_ERROR_NO;
        }
    }

    return SPFG_ERROR_NOT_FOUND;
}

spfg_err_t _spfg_find_free_gr_fn(spfg_gr_t *gr, uint32_t *idx, spfg_fn_t **fn)
{
    for (spfg_gr_fn_cnt_t i = 0; i < SPFG_MAX_GRID_FNS; i++) {

        if (!gr->fns[i].name.chars[0]) {
            *idx = i;

            if (fn) {
                *fn = &gr->fns[i];
            }

            return SPFG_ERROR_NO;
        }
    }

    return SPFG_ERROR_NOT_FOUND;
}

spfg_err_t _spfg_find_changed_fnx_in_dp(spfg_fnx_t *fnx, uint32_t *idx)
{
    for (spfg_fn_dp_in_cnt_t i = 0; i < SPFG_MAX_FN_IN_DPS && fnx->in_dps[i]; i++) {

        if (!fnx->in_dps[i]->name.chars[0]) {
            continue;
        }

        if (fnx->in_dps[i]->emitted) {
            if (idx) {
                *idx = i;
            }
            return SPFG_ERROR_NO;
        }
    }

    return SPFG_ERROR_NOT_FOUND;
}

spfg_err_t _spfg_ints_have_intersection(int lset[], uint32_t lset_len,
                                        int rset[], uint32_t rset_len,
                                        bool *result)
{
    if (result) {
        *result = false;
    }

    for (uint32_t i = 0; i < lset_len; i++) {
        for (uint32_t j = 0; j < rset_len; j++) {
            if (lset[i] == rset[j]) {
                if (result) {
                    *result = true;
                }
                return SPFG_ERROR_NO;
            }
        }
    }

    return SPFG_ERROR_NO;
}
