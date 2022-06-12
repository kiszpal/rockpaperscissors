//
// Created by palkiszel on 2022.06.12..
//


#include "engine.h"
#include "utest_engine.h"
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>

CU_SuiteInfo engine_suites[] = {
        { "engine", NULL, NULL, NULL, NULL, engine_tests },
        CU_SUITE_INFO_NULL
};

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return -1;
    };
    if (CU_register_suites(engine_suites) != CUE_SUCCESS) {
        return -1;
    };
    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
