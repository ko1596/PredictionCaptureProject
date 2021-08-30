
CC = gcc
INC = -I. -I./Ta5320G_TIMER
LIB = -pthread -ljson-c
JSON_C_DIR = /usr/local
CFLAGS += -I$(JSON_C_DIR)/include/json-c
LDFLAGS += -L$(JSON_C_DIR)/lib
SRC_1 = cJSON.c PredictionCaptureProject/PreShooting.c PredictionCaptureProject/PreShootingTest.c
DIR = Ta5320G_TIMER
SRC_2 = $(DIR)/Ta53M0.c $(DIR)/m0Uart_rx.c $(DIR)/m0Uart_tx.c $(DIR)/TstA53M0.c

grids_test:
	$(CC) -o grids_test $(LIB) $(LDFAGS) $(CFLAGS) $(INC) main.c $(SRC_1) $(SRC_2) 



.PHONY: clean
clean:
	rm grids_test
