typedef struct {
    int error_occurred;
    int line_number;
    char error_message[100];
} ErrorInfo;

void init_error_info(ErrorInfo *info);
void set_error(ErrorInfo *info, int line_number, const char *message);