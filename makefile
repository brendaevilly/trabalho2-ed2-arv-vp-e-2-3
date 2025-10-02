# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Pastas
DIR_23 = 2-3
DIR_VP = VP

# Executáveis
TARGET_23 = pro2-3
TARGET_VP = proVP

# Fontes de cada pasta
SRC_23 = $(DIR_23)/main.c $(DIR_23)/funcoes.c
SRC_VP = $(DIR_VP)/main.c $(DIR_VP)/funcoes.c

# Objetos
OBJ_23 = $(SRC_23:.c=.o)
OBJ_VP = $(SRC_VP:.c=.o)

# Inclusão de cabeçalhos
INCLUDES = -I$(DIR_23) -I$(DIR_VP)

# ========================
# Regras principais
# ========================

# Compila tudo
all: $(TARGET_23) $(TARGET_VP)

# Compila o projeto 2-3
$(TARGET_23): $(OBJ_23)
	$(CC) $(CFLAGS) -o $(TARGET_23) $(OBJ_23)

# Compila o projeto VP
$(TARGET_VP): $(OBJ_VP)
	$(CC) $(CFLAGS) -o $(TARGET_VP) $(OBJ_VP)

# Compila objetos
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Executa o projeto 2-3
run-23: $(TARGET_23)
	./$(TARGET_23)

# Executa o projeto VP
run-vp: $(TARGET_VP)
	./$(TARGET_VP)

# Limpa arquivos compilados
clean:
	rm -f $(OBJ_23) $(OBJ_VP) $(TARGET_23) $(TARGET_VP)

# Recompila do zero
rebuild: clean all
