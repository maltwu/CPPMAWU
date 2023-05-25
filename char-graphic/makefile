dest_dir = build
src_dir = src

obj_dir = $(dest_dir)/objects
bin_dir = $(dest_dir)/bin

cpp_dir = $(src_dir)/cpp
head_dir = $(src_dir)/header

CC=g++
RESULT = demo
CFLAGS = -Wall -O3 -std=c++17 -I ${head_dir} -v
CFILES = $(notdir $(wildcard $(src_dir)/*.cpp)) $(notdir $(wildcard $(src_dir)/*/*.cpp))
#demo.cpp math.cpp  wildcard 在指定目录下搜索所有指定格式的文件
ofiles = $(patsubst %.cpp,$(obj_dir)/%.o,$(CFILES)) # $(CFILES:%.cpp=$(obj_dir)/%.o)

program = $(bin_dir)/$(RESULT)
# 定义了最终Target 和 依赖 此启动make 程序
$(program):$(ofiles)

# $@ 指向的是 target:prerequisites 中的 target $< 指向的是 prerequisites中的第一个依赖 $^指向的是所有依赖
$(bin_dir)/%:
	@echo ">>> Linking" $@ "::" $(ofiles) "::" $(CFILES) "<<<"
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi;
	$(CC) -o $@ $^
	ln -sf $@ $(notdir $@)

# src 所有 cpp 文件
$(obj_dir)/%.o:$(src_dir)/%.cpp
	@echo ">>> Compiling" $< "<<<"
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi;
	$(CC) $(CFLAGS) -c $< -o $@

# src 所有子文件夹中 cpp 文件
$(obj_dir)/%.o:$(src_dir)/*/%.cpp
	@echo ">>> Compiling" $< "<<<"
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi;
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY:clean
clean:
	rm -rf $(dest_dir)
	rm -f $(RESULT)
