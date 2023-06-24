test_wasmedge: test_wasmedge.c exported.wasm
	gcc test_wasmedge.c cmemory.c -lwasmedge -o test_wasmedge
exported.wasm: to_export.c
	emcc to_export.c -s EXPORTED_FUNCTIONS=["_malloc"] -o exported.wasm
clean:
	rm *.wasm test_wasmedge