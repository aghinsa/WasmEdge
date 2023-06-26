WASM_BUILD=/Users/ashahali/Documents/Projects/WasmEdge/build
WASMEDGE_TEST=${WASM_BUILD}/tools/wasmedge
WASM_TEST_C_LIB=${WASM_BUILD}/lib/api
WASM_TEST_C_INCLUDE=${WASM_BUILD}/include/api

# gcc test_wasmedge.c cmemory.c -lwasmedge -o test_wasmedge
# gcc test_wasmedge.c cmemory.c -I${WASM_TEST_C_INCLUDE} -L${WASM_TEST_C_LIB} -o test_wasmedge

test_wasmedge: test_wasmedge.c exported.wasm
	gcc test_wasmedge.c cmemory.c -I${WASM_TEST_C_INCLUDE} -L${WASM_TEST_C_LIB} -lwasmedge -o test_wasmedge

test: test_wasmedge
	DYLD_LIBRARY_PATH=${WASM_TEST_C_LIB} ./test_wasmedge

exported.wasm: to_export.c
	emcc to_export.c -s EXPORTED_FUNCTIONS=["_malloc"] -o exported.wasm
clean:
	rm *.wasm test_wasmedge