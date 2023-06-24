#include <wasmedge/wasmedge.h>
#include <stdio.h>

#include "cmemory.h"

void* WasmEdge_Malloc(WasmEdge_VMContext *VMCxt, int32_t size){
    WasmEdge_Result Res;

    /* Get the module */
    const WasmEdge_ModuleInstanceContext *module = WasmEdge_VMGetActiveModule(VMCxt);

    if (module == NULL) {
        WasmEdge_VMDelete(VMCxt);
        printf("Error on getting module");
        return NULL;
    }

    /* Get memory of the module */
    WasmEdge_String MemoryName = WasmEdge_StringCreateByCString("memory");
    WasmEdge_MemoryInstanceContext *MemoryCxt = WasmEdge_ModuleInstanceFindMemory(
                                                    module,MemoryName
                                                );
    WasmEdge_StringDelete(MemoryName);

    if (MemoryCxt == NULL){
        printf("Error on 'memory' not in module");
        return NULL;
    }

    /* Get base memory address to add offset */
    uint8_t *memory_base_ptr = WasmEdge_MemoryInstanceGetPointer(MemoryCxt, 0, 1);

    /* Call malloc */
    WasmEdge_String FunctionName = WasmEdge_StringCreateByCString("malloc");
    WasmEdge_Value Returns[1] = { WasmEdge_ValueGenI32(size) };
    WasmEdge_Value Params[1] = { WasmEdge_ValueGenI32(size) };

    Res = WasmEdge_VMExecute(VMCxt, FunctionName, Params, 1, Returns, 1);
    if (!WasmEdge_ResultOK(Res))
	{
		printf("WASM malloc(%d) call failed: %s\n", size, WasmEdge_ResultGetMessage(Res));
		return 0;
	}

    /* Get offset in Memory 'memory' */
    int32_t offset = WasmEdge_ValueGetI32(Returns[0]);

    WasmEdge_StringDelete(FunctionName);
    return (void *)&memory_base_ptr[offset];
}