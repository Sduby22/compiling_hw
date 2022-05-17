extern int stack[10000];
extern int top;

void push(int i);
void pusharr(int *arr, int len);
int pop(int num); 
int get();
void reset();
void printstack(int isll);
void die(const char*);
