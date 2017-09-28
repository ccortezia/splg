#ifndef __SPFG_UTILS_H__
#define __SPFG_UTILS_H__

#include "spfg/spfg.h"
#include "spfg_types.h"

// -------------------------------------------------------------------------------------------------
// Local macros
// -------------------------------------------------------------------------------------------------

#define SPFG_GR_ID0 1
#define GEN_SPFG_DP_ID(gr_id, idx) ((gr_id - SPFG_GR_ID0) * SPFG_MAX_GRID_DPS + idx + 1)
#define GEN_SPFG_FN_ID(gr_id, idx) ((gr_id - SPFG_GR_ID0) * SPFG_MAX_GRID_FNS + idx + 1)

#ifdef __cplusplus
extern "C" {
#endif

spfg_err_t find_gr(spfg_gr_id_t gr_id, uint32_t *idx);
spfg_err_t find_free_gr(uint32_t *idx, spfg_gr_t **gr);
spfg_err_t find_free_gr_dp(spfg_gr_t *gr, uint32_t *idx, spfg_dp_t **dp);
spfg_err_t find_free_gr_fn(spfg_gr_t *gr, uint32_t *idx, spfg_fn_t **fn);
spfg_err_t find_changed_in_dp_for_fnx(spfg_fnx_t *fnx, uint32_t *idx);

spfg_err_t resolve_gr(spfg_gr_id_t gr_id, spfg_gr_t **gr);
spfg_err_t resolve_gr_dp(spfg_gr_t *gr, spfg_dp_id_t dp_id, spfg_dp_t **dp);
spfg_err_t resolve_gr_fn(spfg_gr_t *gr, spfg_fn_id_t fn_id, spfg_fn_t **fn);
spfg_err_t resolve_dps(spfg_gr_t *gr, spfg_dp_id_t *dp_ids, spfg_dp_t *dps[], uint8_t length);

spfg_err_t create_name(const char *ascii, spfg_block_name_t *name);
spfg_err_t clear_changed_fnx_inputs(spfg_fnx_t *fnx);

#ifdef __cplusplus
}
#endif

#endif // __SPFG_UTILS_H__
