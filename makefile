# ===== 기본 설정 =====
CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude -MMD -MP -pthread
LDFLAGS = -lpthread

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# 소스 및 오브젝트 파일
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

# 최종 실행 파일
TARGET = $(BIN_DIR)/ball_game

.PHONY: all clean

# 기본 타겟
all: $(BIN_DIR) $(OBJ_DIR) $(TARGET)

# 디렉토리 없으면 생성
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# 실행파일 생성
$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

# .c → .o 규칙
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 자동 의존성 파일 포함
-include $(DEPS)

# 정리
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
