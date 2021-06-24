build_dir  := ./build
cmake_vars := -DCMAKE_EXPORT_COMPILE_COMMANDS=YES
run_cmd    := ./tests/test_file

all: configure
	@cmake --build $(build_dir)

configure: $(build_dir)/build.ninja compile_commands.json

$(build_dir)/build.ninja: CMakeLists.txt
	@cmake -G Ninja -B $(build_dir) $(cmake_vars) .

compile_commands.json:
	@ln -s $(build_dir)/compile_commands.json

run: all
	@cd $(build_dir) && $(run_cmd)

clean:
	@cd $(build_dir) && ninja clean

.PHONY: all run clean
