CONTRACT=autodelegate

all: $(CONTRACT).wasm $(CONTRACT).abi

%.wasm: %.cpp
	eosio-cpp -I. -o $@ $<

%.abi: %.cpp
	eosio-cpp -abigen -o $@ $<

clean:
	rm -f $(CONTRACT).wasm $(CONTRACT).abi