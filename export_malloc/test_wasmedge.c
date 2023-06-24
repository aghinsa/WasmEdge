#include <wasmedge/wasmedge.h>
#include <stdio.h>
#include <assert.h>

#include "cmemory.h"

int load_wasm_to_vm(WasmEdge_VMContext *VMCxt, char *path){
    WasmEdge_Result Res;

    /* Load wasm */
    Res = WasmEdge_VMLoadWasmFromFile(VMCxt, path);

    if (!WasmEdge_ResultOK(Res)) {
        WasmEdge_VMDelete(VMCxt);
        printf("Load test.wasm error: %s\n", WasmEdge_ResultGetMessage(Res));
        return -1;
    }

    Res = WasmEdge_VMValidate(VMCxt);
    if (!WasmEdge_ResultOK(Res)) {
        WasmEdge_VMDelete(VMCxt);
        printf("Validate test.wasm error: %s\n",
            WasmEdge_ResultGetMessage(Res));
        return -1;
    }

    Res = WasmEdge_VMInstantiate(VMCxt);
    if (!WasmEdge_ResultOK(Res)) {
        WasmEdge_VMDelete(VMCxt);
        printf("Instantiate test.wasm error: %s\n",
            WasmEdge_ResultGetMessage(Res));
        return -1;
    }
    return 0;
}

int test_wasmedge_malloc(){
    WasmEdge_Result Res;
    int rc;

    WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
    WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
    WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);

    rc = load_wasm_to_vm(VMCxt,"exported.wasm");

    if(rc == -1){
        return -1;
    }

    /* Allocate memory */
    int32_t size = 4;
    int32_t *buffer = (int32_t *)WasmEdge_Malloc(VMCxt, size * sizeof(int32_t));

    for(int i = 0;i<size;i++){
        buffer[i] = i;
    }

    for(int i = 0;i<size;i++){
        assert(buffer[i] == i);
    }


    /* Resources deallocations. */
    WasmEdge_VMDelete(VMCxt);
    WasmEdge_ConfigureDelete(ConfCxt);
    return 0;
}

int test_fillData_same_module(){
    WasmEdge_Result Res;
    int rc;

    WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
    WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
    WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, NULL);

    rc = load_wasm_to_vm(VMCxt,"exported.wasm");

    if(rc == -1){
        return -1;
    }

    /* Allocate memory */
    int32_t size = 4;
    int32_t *buffer = (int32_t *)WasmEdge_Malloc(VMCxt, size * sizeof(int32_t));

    /* Call fillData */
    WasmEdge_String FunctionName = WasmEdge_StringCreateByCString("fillData");
    WasmEdge_Value Params[2] = { WasmEdge_ValueGenI32(buffer),
                                WasmEdge_ValueGenI32(size) };
    
    printf("Pointer passed is %p\n",buffer);

    Res = WasmEdge_VMExecute(VMCxt, FunctionName, Params, 2, NULL, 0);
    if (!WasmEdge_ResultOK(Res))
	{
		printf("WASM filldata(%d) call failed: %s\n", size, WasmEdge_ResultGetMessage(Res));
		return 0;
	}

    /* Resources deallocations */
    WasmEdge_StringDelete(FunctionName);
    WasmEdge_VMDelete(VMCxt);
    WasmEdge_ConfigureDelete(ConfCxt);
    return 0;
}


int main(int argc ,const char *argv[]){
    int rc;

    printf("Testing wasmedge exported malloc\n");
    rc = test_wasmedge_malloc();
    if(rc == -1){
        printf("Error in test_wasmedge_malloc\n");
    }
    else{
        printf("Test successful\n");
    }

    test_fillData_same_module();
}