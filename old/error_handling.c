#include "error_handling.h"
void init_error_info(ErrorInfo *info) {
    info->error_occurred = 0;
    info->line_number = 0;
    info->error_message[0] = '\0';
}

void set_error(ErrorInfo *info, int line_number, const char *message) {
    info->error_occurred = 1;
    info->line_number = line_number;
    strncpy(info->error_message, message, sizeof(info->error_message) - 1);
    info->error_message[sizeof(info->error_message) - 1] = '\0';
}