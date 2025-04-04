# ===== 기본 설정 =====
CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude -MMD -MP -pthread
LDFLAGS = -lpthread

SRC_DIR_SHARED  = src/shared
SRC_DIR_SERVER  = src/server
SRC_DIR_CLIENT  = src/client

OBJ_DIR_SHARED  = obj/shared
OBJ_DIR_SERVER  = obj/server
OBJ_DIR_CLIENT  = obj/client

BIN_DIR = bin

# 소스 파일
SRCS_SHARED  = $(wildcard $(SRC_DIR_SHARED)/*.c)
SRCS_SERVER  = $(wildcard $(SRC_DIR_SERVER)/*.c)
SRCS_CLIENT  = $(wildcard $(SRC_DIR_CLIENT)/*.c)

# 오브젝트 파일
OBJS_SHARED  = $(patsubst $(SRC_DIR_SHARED)/%.c, $(OBJ_DIR_SHARED)/%.o, $(SRCS_SHARED))
OBJS_SERVER  = $(patsubst $(SRC_DIR_SERVER)/%.c, $(OBJ_DIR_SERVER)/%.o, $(SRCS_SERVER))
OBJS_CLIENT  = $(patsubst $(SRC_DIR_CLIENT)/%.c, $(OBJ_DIR_CLIENT)/%.o, $(SRCS_CLIENT))

DEPS = $(OBJS_SHARED:.o=.d) $(OBJS_SERVER:.o=.d) $(OBJS_CLIENT:.o=.d)

# 실행파일
TARGET_SERVER = $(BIN_DIR)/server
TARGET_CLIENT = $(BIN_DIR)/client

.PHONY: all server client clean

# 기본: 서버 + 클라이언트 빌드
all: $(TARGET_SERVER) $(TARGET_CLIENT)

# 디렉토리 생성
$(BIN_DIR) $(OBJ_DIR_SHARED) $(OBJ_DIR_SERVER) $(OBJ_DIR_CLIENT):
	mkdir -p $@

# 서버 빌드
$(TARGET_SERVER): $(BIN_DIR) $(OBJ_DIR_SHARED) $(OBJ_DIR_SERVER) $(OBJS_SHARED) $(OBJS_SERVER)
	$(CC) -o $@ $(OBJS_SHARED) $(OBJS_SERVER) $(LDFLAGS)

# 클라이언트 빌드
$(TARGET_CLIENT): $(BIN_DIR) $(OBJ_DIR_SHARED) $(OBJ_DIR_CLIENT) $(OBJS_SHARED) $(OBJS_CLIENT)
	$(CC) -o $@ $(OBJS_SHARED) $(OBJS_CLIENT) $(LDFLAGS)

# .c → .o 빌드 규칙
$(OBJ_DIR_SHARED)/%.o: $(SRC_DIR_SHARED)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_SERVER)/%.o: $(SRC_DIR_SERVER)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_CLIENT)/%.o: $(SRC_DIR_CLIENT)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# 의존성 포함
-include $(DEPS)

# 정리
clean:
	rm -rf obj bin

