#include <string.h>
#include "unity/unity_fixture.h"
#include "spfg/spfg.h"

// ------------------------------------------------------------------------------------------------

TEST_GROUP(initialization);

TEST_SETUP(initialization) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(initialization) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

TEST(initialization, init_twice_should_err)
{
    TEST_ASSERT_EQUAL(SPFG_ERROR_ALREADY_INITIALIZED, spfg_init());
}

TEST(initialization, finish_twice_should_err)
{
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
    TEST_ASSERT_EQUAL(SPFG_ERROR_NOT_INITIALIZED, spfg_finish());
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_GROUP_RUNNER(initialization) {
    RUN_TEST_CASE(initialization, init_twice_should_err);
    RUN_TEST_CASE(initialization, finish_twice_should_err);
}

// ------------------------------------------------------------------------------------------------

TEST_GROUP(grid_creation);

TEST_SETUP(grid_creation) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(grid_creation) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

TEST(grid_creation, create_grid_should_not_err)
{
    spfg_gr_id_t gr_id;
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid grid name"));
}

TEST(grid_creation, create_grid_with_null_param_should_err)
{
    spfg_gr_id_t gr_id;
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_gr_create(NULL, ""));
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_gr_create(&gr_id, NULL));
}

TEST(grid_creation, create_grid_with_empty_name_should_err)
{
    spfg_gr_id_t gr_id;
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_BLOCK_NAME, spfg_gr_create(&gr_id, ""));
}

TEST(grid_creation, create_grid_beyond_max_should_err)
{
    spfg_gr_id_t gr_id;

    for (int i = 0 ; i < SPFG_MAX_GRID_CNT; i++) {
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid name"));
    }
    TEST_ASSERT_EQUAL(SPFG_ERROR_OUT_OF_SLOTS, spfg_gr_create(&gr_id, "valid name"));
}

TEST(grid_creation, remove_grid)
{
    spfg_gr_id_t gr_id;
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid grid name"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_INVALID_GR_ID, spfg_gr_remove(-1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_remove(gr_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "another grid name"));
}


TEST(grid_creation, get_grid_ids)
{
    spfg_gr_id_t gr_id_1;
    spfg_gr_id_t gr_id_2;
    spfg_gr_cnt_t count = 0;
    spfg_gr_id_t out_ids[100];

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id_1, "gr1"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_get_ids(out_ids, 100, &count));
    TEST_ASSERT_EQUAL(1, count);
    TEST_ASSERT_EQUAL(out_ids[0], gr_id_1);

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id_2, "gr2"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_get_ids(out_ids, 100, &count));
    TEST_ASSERT_EQUAL(2, count);
    TEST_ASSERT_EQUAL(gr_id_1, out_ids[0]);
    TEST_ASSERT_EQUAL(gr_id_2, out_ids[1]);
}


TEST_GROUP_RUNNER(grid_creation) {
    RUN_TEST_CASE(grid_creation, create_grid_should_not_err);
    RUN_TEST_CASE(grid_creation, create_grid_with_empty_name_should_err);
    RUN_TEST_CASE(grid_creation, create_grid_with_null_param_should_err);
    RUN_TEST_CASE(grid_creation, create_grid_beyond_max_should_err);
    RUN_TEST_CASE(grid_creation, remove_grid);
    RUN_TEST_CASE(grid_creation, get_grid_ids);
}

// ------------------------------------------------------------------------------------------------

TEST_GROUP(function_creation);

TEST_SETUP(function_creation) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(function_creation) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

TEST(function_creation, create_fn_parameter_validation)
{
    spfg_gr_id_t gr_id;
    spfg_fn_id_t fn_id;
    spfg_dp_id_t dp0p0_id;
    spfg_dp_id_t dp0p1_id;
    spfg_dp_id_t dp1p0_id;

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid name"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p0", &dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p1", &dp0p1_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p0", &dp1p0_id));

    spfg_dp_id_t in_dps[] = {dp0p0_id, dp0p1_id, dp0p1_id};
    spfg_dp_id_t out_dps[] = {dp1p0_id, dp1p0_id};
    TEST_ASSERT_EQUAL(SPFG_ERROR_VALIDATE_FN, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 3, out_dps, 1, "fn1", &fn_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_VALIDATE_FN, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 1, out_dps, 1, "fn1", &fn_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_VALIDATE_FN, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 2, out_dps, 0, "fn1", &fn_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_VALIDATE_FN, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 2, out_dps, 2, "fn1", &fn_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 2, out_dps, 1, "fn1", &fn_id));
}

TEST(function_creation, remove_fn)
{
    spfg_gr_id_t gr_id;
    spfg_fn_id_t fn_id;
    spfg_dp_id_t dp0p0_id;
    spfg_dp_id_t dp0p1_id;
    spfg_dp_id_t dp1p0_id;

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid name"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p0", &dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p1", &dp0p1_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p0", &dp1p0_id));

    spfg_dp_id_t in_dps[] = {dp0p0_id, dp0p1_id};
    spfg_dp_id_t out_dps[] = {dp1p0_id, dp1p0_id};
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 2, out_dps, 1, "fn1", &fn_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_INVALID_GR_ID, spfg_fn_remove(-1, fn_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_INVALID_FN_ID, spfg_fn_remove(gr_id, -1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_remove(gr_id, fn_id));
}

TEST_GROUP_RUNNER(function_creation) {
    RUN_TEST_CASE(function_creation, create_fn_parameter_validation);
    RUN_TEST_CASE(function_creation, remove_fn);
}

// ------------------------------------------------------------------------------------------------

TEST_GROUP(datapoint_creation);

TEST_SETUP(datapoint_creation) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(datapoint_creation) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

TEST(datapoint_creation, remove_dp)
{
    spfg_gr_id_t gr_id;

    spfg_fn_id_t fn0_id;
    spfg_dp_id_t dp0p0_id;
    spfg_dp_id_t dp0p1_id;
    spfg_dp_id_t dp1p0_id;

    spfg_fn_id_t fn1_id;
    spfg_dp_id_t dp0p2_id;
    spfg_dp_id_t dp0p3_id;
    spfg_dp_id_t dp1p1_id;

    {
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid name"));
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p0", &dp0p0_id));
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p1", &dp0p1_id));
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p0", &dp1p0_id));
        spfg_dp_id_t in_dps[] = {dp0p0_id, dp0p1_id};
        spfg_dp_id_t out_dps[] = {dp1p0_id};
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 2, out_dps, 1, "fn1", &fn0_id));
    }

    {
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p2", &dp0p2_id));
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p3", &dp0p3_id));
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p1", &dp1p1_id));
        spfg_dp_id_t in_dps[] = {dp0p2_id, dp0p3_id};
        spfg_dp_id_t out_dps[] = {dp1p1_id};
        TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 1, in_dps, 2, out_dps, 1, "fn2", &fn1_id));
    }

    TEST_ASSERT_EQUAL(SPFG_ERROR_INVALID_GR_ID, spfg_dp_remove(-1, dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_INVALID_DP_ID, spfg_dp_remove(gr_id, -1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_FN_INTEGRITY, spfg_dp_remove(gr_id, dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_FN_INTEGRITY, spfg_dp_remove(gr_id, dp1p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_remove(gr_id, fn0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_remove(gr_id, dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_remove(gr_id, dp0p1_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_remove(gr_id, dp1p0_id));
}

TEST_GROUP_RUNNER(datapoint_creation) {
    RUN_TEST_CASE(datapoint_creation, remove_dp);
}

// ------------------------------------------------------------------------------------------------

TEST_GROUP(cycle_running);

TEST_SETUP(cycle_running) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(cycle_running) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

TEST(cycle_running, run_cycle)
{
    spfg_gr_id_t gr_id;
    spfg_fn_id_t fn_id;
    spfg_dp_id_t dp0p0_id;
    spfg_dp_id_t dp1p0_id;
    spfg_dp_id_t dp0p1_id;
    spfg_boolean_t output;
    spfg_boolean_t emitted;

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid name"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p0", &dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p0", &dp1p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p1", &dp0p1_id));

    spfg_dp_id_t in_dps[] = {dp0p0_id, dp1p0_id};
    spfg_dp_id_t out_dps[] = {dp0p1_id};
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 0, in_dps, 2, out_dps, 1, "fn1", &fn_id));

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_reset_cycle(gr_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_run_cycle(gr_id, 0, NULL, NULL));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr_id, dp0p1_id, &output, &emitted));
    TEST_ASSERT_EQUAL(false, output);
    TEST_ASSERT_EQUAL(false, emitted);

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_reset_cycle(gr_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr_id, dp0p0_id, true));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr_id, dp1p0_id, false));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_run_cycle(gr_id, 0, NULL, NULL));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr_id, dp0p1_id, &output, &emitted));
    TEST_ASSERT_EQUAL(false, output);
    TEST_ASSERT_EQUAL(false, emitted);

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_reset_cycle(gr_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr_id, dp0p0_id, true));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr_id, dp1p0_id, true));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_run_cycle(gr_id, 0, NULL, NULL));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr_id, dp0p1_id, &output, &emitted));
    TEST_ASSERT_EQUAL(true, output);
    TEST_ASSERT_EQUAL(true, emitted);
}

typedef struct test_cb_ctl {
    int counter;
    int stop_at;
} test_loop_ctl_t;

spfg_err_t cycle_callback_stop_ctl(spfg_gr_id_t gr_id, spfg_fn_id_t fn_id, spfg_phase_t phase, void *cdata) {
    test_loop_ctl_t *ctl = (test_loop_ctl_t *)cdata;
    if (ctl->stop_at >= 0 && ctl->counter == ctl->stop_at) {
        return SPFG_LOOP_CONTROL_STOP;
    }
    ctl->counter += 1;
    return SPFG_ERROR_NO;
}

TEST(cycle_running, run_cycle_with_callback)
{
    spfg_gr_id_t gr_id;
    spfg_fn_id_t fn0p0_id;
    spfg_fn_id_t fn0p1_id;
    spfg_dp_id_t dp0p0_id;
    spfg_dp_id_t dp1p0_id;
    spfg_dp_id_t dp0p1_id;
    spfg_dp_id_t dp1p1_id;
    spfg_dp_id_t dp0p2_id;
    spfg_boolean_t output;
    spfg_boolean_t emitted;
    test_loop_ctl_t loop_ctl_1 = {0, -1};
    test_loop_ctl_t loop_ctl_2 = {0, 1};

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr_id, "valid name"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p0", &dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p1", &dp0p1_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p0", &dp1p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp1p1", &dp1p1_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr_id, SPFG_DP_BOOL, "dp0p2", &dp0p2_id));

    spfg_dp_id_t in_dps_fn0p0[] = {dp0p0_id, dp1p0_id};
    spfg_dp_id_t out_dps_fn0p0[] = {dp0p1_id};
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 0, in_dps_fn0p0, 2, out_dps_fn0p0, 1, "fn0p0", &fn0p0_id));

    spfg_dp_id_t in_dps_fn0p1[] = {dp0p1_id, dp1p1_id};
    spfg_dp_id_t out_dps_fn0p1[] = {dp0p2_id};
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 0, in_dps_fn0p1, 2, out_dps_fn0p1, 1, "fn0p1", &fn0p1_id));

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_reset_cycle(gr_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_run_cycle(gr_id, 0, cycle_callback_stop_ctl, &loop_ctl_1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr_id, dp0p2_id, &output, &emitted));
    TEST_ASSERT_EQUAL(2, loop_ctl_1.counter);
    TEST_ASSERT_EQUAL(false, output);
    TEST_ASSERT_EQUAL(false, emitted);

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_reset_cycle(gr_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr_id, dp0p0_id, 1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr_id, dp1p0_id, 1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_run_cycle(gr_id, 0, cycle_callback_stop_ctl, &loop_ctl_2));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr_id, dp0p1_id, &output, &emitted));
    TEST_ASSERT_EQUAL(1, loop_ctl_2.counter);
    TEST_ASSERT_EQUAL(true, output);
    TEST_ASSERT_EQUAL(true, emitted);
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr_id, dp0p2_id, &output, &emitted));
    TEST_ASSERT_EQUAL(false, output);
    TEST_ASSERT_EQUAL(false, emitted);
}

TEST_GROUP_RUNNER(cycle_running) {
    RUN_TEST_CASE(cycle_running, run_cycle);
    RUN_TEST_CASE(cycle_running, run_cycle_with_callback);
}

// ------------------------------------------------------------------------------------------------

TEST_GROUP(schema_exporting);

TEST_SETUP(schema_exporting) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(schema_exporting) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

char export_outbuf[1024 * 20];

TEST(schema_exporting, export_import)
{
    spfg_gr_id_t gr0_id;
    spfg_gr_id_t gr1_id;
    spfg_gr_id_t gr2_id;
    spfg_fn_id_t fn_id;
    spfg_dp_id_t dp0p0_id;
    spfg_dp_id_t dp1p0_id;
    spfg_dp_id_t dp0p1_id;
    spfg_boolean_t output;
    spfg_boolean_t emitted;

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_create(&gr0_id, "gr0"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr0_id, SPFG_DP_BOOL, "dp0p0", &dp0p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr0_id, SPFG_DP_BOOL, "dp1p0", &dp1p0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_create(gr0_id, SPFG_DP_BOOL, "dp0p1", &dp0p1_id));

    spfg_dp_id_t in_dps[] = {dp0p0_id, dp1p0_id};
    spfg_dp_id_t out_dps[] = {dp0p1_id};
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_fn_create(gr0_id, SPFG_FN_AND_BOOL_BOOL_RET_BOOL, 0, in_dps, 2, out_dps, 1, "fn1", &fn_id));

    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_reset_cycle(gr0_id));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr0_id, dp0p0_id, true));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_set_bool(gr0_id, dp1p0_id, true));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_run_cycle(gr0_id, false, NULL, NULL));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_dp_get_bool(gr0_id, dp0p1_id, &output, &emitted));
    TEST_ASSERT_EQUAL(true, output);
    TEST_ASSERT_EQUAL(true, emitted);

    TEST_ASSERT_EQUAL(SPFG_ERROR_BUFFER_OVERFLOW, spfg_gr_export_bin(gr0_id, export_outbuf, 1));
    TEST_ASSERT_EQUAL(SPFG_ERROR_INVALID_GR_ID, spfg_gr_export_bin(-1, export_outbuf, sizeof(export_outbuf)));
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_gr_export_bin(gr0_id, 0, sizeof(export_outbuf)));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_export_bin(gr0_id, export_outbuf, sizeof(export_outbuf)));

    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_gr_import_bin(NULL, sizeof(export_outbuf), &gr1_id, "gr1"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_gr_import_bin(export_outbuf, sizeof(export_outbuf), NULL, "gr1"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_gr_import_bin(export_outbuf, sizeof(export_outbuf), &gr1_id, NULL));
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_INVALID_VALUE, spfg_gr_import_bin(export_outbuf, 1, &gr1_id, "gr1"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_gr_import_bin(export_outbuf, sizeof(export_outbuf), &gr1_id, "gr1"));
    TEST_ASSERT_EQUAL(SPFG_ERROR_OUT_OF_SLOTS, spfg_gr_import_bin(export_outbuf, sizeof(export_outbuf), &gr2_id, "gr2"));

    TEST_ASSERT_NOT_EQUAL(0, gr1_id);
    TEST_ASSERT_NOT_EQUAL(gr0_id, gr1_id);
}

TEST_GROUP_RUNNER(schema_exporting) {
    RUN_TEST_CASE(schema_exporting, export_import);
}

// ------------------------------------------------------------------------------------------------

TEST_GROUP(inspection);

TEST_SETUP(inspection) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_init());
}

TEST_TEAR_DOWN(inspection) {
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_finish());
}

TEST(inspection, spfg_info)
{
    spfg_info_t info;
    TEST_ASSERT_EQUAL(SPFG_ERROR_BAD_PARAM_NULL_POINTER, spfg_info(NULL));
    TEST_ASSERT_EQUAL(SPFG_ERROR_NO, spfg_info(&info));
    TEST_ASSERT_EQUAL(VERSION_MAJOR, info.version_major);
    TEST_ASSERT_EQUAL(VERSION_MINOR, info.version_minor);
    TEST_ASSERT_EQUAL(VERSION_PATCH, info.version_patch);
    TEST_ASSERT_EQUAL(SPFG_MAX_GRID_CNT, info.max_grid_cnt);
    TEST_ASSERT_EQUAL(SPFG_MAX_FN_IN_DPS, info.max_fn_in_dps);
    TEST_ASSERT_EQUAL(SPFG_MAX_FN_OUT_DPS, info.max_fn_out_dps);
    TEST_ASSERT_EQUAL(SPFG_MAX_GRID_FNS, info.max_grid_fns);
    TEST_ASSERT_EQUAL(SPFG_MAX_GRID_DPS, info.max_grid_dps);
    TEST_ASSERT_EQUAL(SPFG_MAX_PHASES, info.max_phases);
}

TEST_GROUP_RUNNER(inspection) {
    RUN_TEST_CASE(inspection, spfg_info);
}

// ---

static void run_all_tests(void)
{
    RUN_TEST_GROUP(initialization);
    RUN_TEST_GROUP(grid_creation);
    RUN_TEST_GROUP(function_creation);
    RUN_TEST_GROUP(datapoint_creation);
    RUN_TEST_GROUP(cycle_running);
    RUN_TEST_GROUP(schema_exporting);
    RUN_TEST_GROUP(inspection);
}

int main(int argc, const char* argv[])
{
    return UnityMain(argc, argv, run_all_tests);
}

