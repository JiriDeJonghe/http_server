#include <string.h>
#include "utils.h"

int string_in_array(char *target, char *arr[], int arraySize) {
	for (int i = 0; i < arraySize; i++) {
		if (strcmp(target, arr[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

