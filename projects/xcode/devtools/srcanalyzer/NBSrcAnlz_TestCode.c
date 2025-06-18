
typedef struct point {
	int x, y;
} pointType;

typedef enum ENTest_ {
	ENTest_Zero = 0,
	ENTest_One,
	ENTest_Two
} ENTest;

double sin(double p);

double sin(double p){
	return 0.0;
}

//\
Second line comment\
Third line comment
/*
Multiline comment, 1
Multiline comment, 2
Multiline comment, 3
*/

int a, funcA(int x, int y), b, funcB(int x, int y), funcC(int x, int y);

//declaration-specifiers declarator declaration-list compound-statement

/*int funcWithDecList(a, b) int a; int b; {
	int r = a + b;
	return r;
}*/

/*int funcWithDecLis2(a, b) int a; int b; {
	int r = a + b;
	return r;
}*/

#	define VAL_A	"A"
#	define VAL_B	"B"
#	define VAL_AB	"AB"
#	define ARG1_MERGED(X, Y)	X ## Y
#	define VA_ARGS_STR(MY...)	#MY

void test00(){
	const char* strs[] = {
		ARG1_MERGED(VAL_A, B)" '%s'.\n"
		, VA_ARGS_STR(ARG1_MERGED(VAL_A, B))
	};
	int a = 0;
	int b = 123\
456;
	//no brackets
	do a++; while(a < 10);
	while(a < 10) a++;
	for(a = 0; a < 10; a++);
	if(a < 10) a = 1; else a = 2;
	//with brackets
	do{
		a++;
	} while(a < 20);
	//
	if(a < 10){
		a = 1;
	} else {
		a = 2;
	}
	//
	while(a < 10){
		a++;
	}
	//
	for(a = 0; a < 20; a++){
		//Testing empty 'compound-statement'
	}
	//
	if(a < 10){
		a = 1;
	} else {
		a = 2;
	}
}
	
void test(){
	union {
		struct {
			int    alltypes;
		} n; struct {
			int    type;
			int    intnode;
		} ni; struct {
			int    type;
			double doublenode;
		} nf;
	} u;
	
	u.nf.type = 1; u.nf.doublenode = 3.14; /* ... */
	if (u.n.alltypes == 1)
		if (sin(u.nf.doublenode) == 0.0) (void)(0);
}

void test2(){
	int *a;
	int *p = (int []){2, 4}; //two values are set
	a = (int [2]){*p}; //the array is size 2 but only the first value is set
}

int defVar;

void drawline(struct point p1, struct point p2){
	//
}

void test22(){
	drawline((struct point){.x=1, .y=1}, (struct point){.y=4, .x=3});
}

void drawlinePtr(struct point *p1, struct point* p2){
	//
}

void test222(){
	drawlinePtr(&(struct point){.x=1, .y=1}, &(struct point){.x=3, .y=4});
}

void test3(){
	const float arr[] = (const float []){1e0, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6}; //a read-only compound literal
	const char* a = "/tmp/fileXXXXXX"; //static storage duration
	char b[] = (char []){"/tmp/fileXXXXXX"}; //local scope stored
	const char c[] = (const char []){"/tmp/fileXXXXXX"}; //local scope stored
}

struct int_list { int car; struct int_list *cdr; };


void test4(struct int_list p1){
	//
}

void test44(){
	struct int_list endless_zeros = {0, &endless_zeros};
	test4(endless_zeros);
}

struct s { int i; };

int f (void) {
	struct s *p = 0, *q;
	int j = 0;
	again:
	q = p, p = &((struct s){ j++ });
	if (j < 2) goto again;
	return p == q && q->i == 1;
}

int fsize3(int n) {
	char b[n+3]; // variable length array
	int i = (sizeof b / sizeof b[0]);
	return sizeof b; // execution time sizeof
}

void test5(){
	int n = 4, m = 3;
	int a[n][m];
	int (*p)[m] = a;  // p == &a[0]
	p += 1;
	(*p)[2] = 99;
	n = p - a;
	// p == &a[1] // a[1][2] == 99 //n==1
}

void test6(){
	char c;
	int i;
	long l;
	l = (c = i); //type convertion and multiple assignment (return value when assigning).
}

void test7(){
	const char **cpp;
	char *p;
	const char c = 'A';
	//cpp = &p; //not valid
	*cpp = &c;
	*p = 0;
	// constraint violation // valid
	// valid
}

void test8(int a, int b, int c){
	//
}

void test88(){
	int a, t, c;
	test8(a, (t=3, t+2), c); //the function has three arguments, the second of which has the value 5.
}

//The semantic rules for the evaluation of a constant expression are the same as for nonconstant expressions.
static int i = 2 || 1 / 0; // the expression is a valid integer constant expression with value one.

void* myMalloc(int size){
	return (void*)0;
}

void test9(){
	//XCode msg: variable lenght array in structure extension will never be supported.
	/*struct s { int n; double d[]; }; //the structure struct s has a flexible array member d
	//
	int m = 2;
	//This ...
	struct s *p = myMalloc(sizeof (struct s) + sizeof (double [m]));
	// ... behaves like.
	struct { int n; double d[m]; } *p2;
	//
	struct s t1 = { 0 }; //valid
	struct s t2 = { 1, { 4.2 }}; t1.n = 4; //invalid
	t1.d[0] = 4.2; //valid*/
}

void test10(){
	enum hue { chartreuse, burgundy, claret=20, winedark };
	enum hue col, *cp;
	col = claret;
	cp = &col;
	if (*cp != burgundy) (void)0;
}

struct tnode2 {
	int count;
	struct tnode2 *left, *right; //self referenced
};

typedef struct tnode TNODE; //typedef
struct tnode {
	int count;
	TNODE *left, *right; //self referenced by typedef
};

TNODE s, *sp;

void test11(){
	const struct s { int mem; } cs = { 1 };
	enum enodeSub;
	struct tnodeSub;
	int enodeSub = 0;
	int tnodeSub = 0;
	struct s ncs; // the object ncs is modifiable
	typedef int A[2][3];
	const A a = {{4, 5, 6}, {7, 8, 9}}; // array of array of const int int *pi;
	int *pi;
	const int *pci;
	ncs = cs; // valid
	//cs = ncs; // violates modifiable lvalue constraint for =
	pi = &ncs.mem; // valid
	//pi = &cs.mem; // violates type constraints for =
	pci = &cs.mem; // valid
	//pi = a[0]; // invalid: a[0] has type ‘‘const int *’’
	enum enodeSub {enodeSub0, enodeSub1, };
	struct tnodeSub { int a, b, c; };
}

void test12(int n, int * restrict p, int * restrict q){
	while (n-- > 0)
		*p++ = *q++;
}

void test13(int n, int * restrict p, int * restrict q, int * restrict r){
	int i;
	for (i = 0; i < n; i++)
		p[i] = q[i] + r[i];
}


typedef struct { int n; float * restrict v; } vector;

vector new_vector(int n){
	vector t;
	t.n = n;
	t.v = myMalloc(n * sizeof (float));
	return t;
}

inline double fahr(double t) {
	return (9.0 * t) / 5.0 + 32.0;
}

inline double cels(double t) {
	return (5.0 * (t - 32.0)) / 9.0;
}

extern double fahr(double); // creates an external definition
extern double cels(double); // creates an external definition

double convert(int is_fahr, double temp){
	/* A translator may perform inline substitutions */
	return is_fahr ? cels(temp) : fahr(temp);
}

void test14(){
	//
	typedef int *int_ptr;
	const int_ptr constant_ptr;
	//declares an array of float numbers and an array of pointers to float numbers.
	float fa[11], *afp[17];
	//
	extern int *x; // to be a pointer to int
	extern int y[]; //array of int of unspecified size (an incomplete type), the storage for which is defined elsewhere.
}

int n = 4;
int m = 5;

void fcompat(void){
	int a[n][6][m];
	int (*p)[4][n+1];
	int c[n][n][6][m];
	int (*r)[n][n][n+1];
	p = a; // invalid: not compatible because 4 != 6
	r = c; // compatible, but defined behavior only if
	// n == 6 and m == n+1
}

//declares 3 functions (normal, pointer returner, and a pointer to a function).
int f(void), *fip(), (*pfi)();

//declares an array apfi of three pointers to functions returning int and having two parameters each.
int (*apfi[3])(int *x, int *y);

//declares a function fpfi that returns a pointer to a function returning an int. The function fpfi has two parameters: a pointer to a function returning an int (with one parameter of type long int), and an int. The pointer returned by fpfi points to a function that has one int parameter and accepts zero or more additional arguments of any type.
int (*fpfi(int (*)(long), int))(int, ...);


void addscalar(int n, int m, double a[n][n*m+300], double x);

int test15() {
	double b[4][308];
	addscalar(4, 2, b, 2.17);
	return 0;
}

void addscalar(int n, int m, double a[n][n*m+300], double x){
	for (int i = 0; i < n; i++)
		for (int j = 0, k = n*m+300; j < k; j++)
			// a is a pointer to a VLA with n*m+300 elements
			a[i][j] += x;
}

void test16(){
	typedef void fv(int), (*pfv)(int);
	void (*signal(int, void (*)(int)))(int);
	fv *signal(int, fv *);
	pfv signal(int, pfv);
}

void copyt(int n){
	typedef int B[n]; // B is n ints, n evaluated now
	n += 1;
	B a; // aisnints,nwithout+= 1
	int b[n]; // a and b are different sizes
	for (int i = 1; i < n; i++)
		a[i-1] = b[i];
}

#define MAX 15

#define VOID_FUNC() (void)0

#define VOID_FUNC_N(...) __VA_ARGS__

#define VOID_FUNC_N2(OPT_NAME...) OPT_NAME

#define VOID_FUNC_N_STR(...) #__VA_ARGS__

#define VOID_FUNC_N_STR2(OPT_NAME...) #OPT_NAME

#define VOID_FUNC_1(X) (void)X

#define VOID_FUNC_2(X, Y) (void)X

#define VOID_FUNC_3(X, Y, Z) VOID_FUNC_2(X, jajaja)

#define VOID_FUNC_4(X, Y, Z, AA) VOID_FUNC_3(X, (cuanta, locura (en estos, tiempos)), (NAAA, AMIGOS!))

void test__ASM__() __asm__("nop;nop;nop");

void test__ASM() __asm("nop;nop;nop");

void testASM() asm("nop;nop;nop");

void test__ASM__V() __asm__ __volatile__ ("nop;nop;nop");

void test__ASMV() __asm __volatile("nop;nop;nop");

void testASMV() asm volatile("nop;nop;nop");

int __attribute__((aligned(8))) vA;
int __attribute((aligned(8))) vA;
//int attribute((aligned(8))) vA;

void test17(){
	//
	__asm__("nop;nop;nop");
	//__asm("nop;nop;nop");
	//asm("nop;nop;nop");
	//
	//__asm__ __volatile__("nop;nop;nop");
	//__asm __volatile("nop;nop;nop");
	//asm volatile("nop;nop;nop");
	//Space can be ‘‘allocated’’ from both ends of an array by using a single designator:
	int a22[MAX] = {
		1, 3, 5, 7, 9, [MAX-5] = 8, 6, 4 VOID_FUNC_N2(,) 2 VOID_FUNC_N(,) 0
	};
	//
	VOID_FUNC();
	VOID_FUNC_1(1);
	VOID_FUNC_2(2, nada de esto va a a pasar);
	VOID_FUNC_2(3, (nada de esto, va a a pasar));
	VOID_FUNC_3(4, (nada de esto, va a a pasar), (como, dijo?));
	VOID_FUNC_4(5, (nada de esto, va a a pasar), (como, dijo?), "JAJAJA");
	VOID_FUNC_4(VOID_FUNC_4(6, upsy, popsy, nopsy), (nada de esto, va a a pasar), (como, dijo?), "JAJAJA");
	//size was specified
	int x[] = { 1, 3, 5 };
	//fully bracketed initialization
	int y[4][3] = {
		{ 1, 3, 5 },
		{ 2, 4, 6 },
		{ 3, 5, 7 },
	};
	//same effect as before 'y'
	int y2[4][3] = {
		1, 3, 5, 2, 4, 6, 3, 5, 7
	};
	//initializes the first column of z as specified and initializes the rest with zeros
	int z[4][3] = {
		{ 1 }, { 2 }, { 3 }, { 4 }
	};
	//A definition with an inconsistently bracketed initialization.
	//It defines an array with two element structures: w[0].a[0] is 1 and w[1].a[0] is 2; all the other elements are zero.
	struct { int a[3], b; } w[] = { { 1 }, 2 };
	//contains an incompletely but consistently bracketed initialization
	short q[4][3][2] = {
		{ 1 },
		{ 2, 3 },
		{ 4, 5, 6 }
	};
	//
	typedef int A[];
	A a = { 1, 2 }, b = { 3, 4, 5 };
	//Is equivalent to the 'A a = {...}' declaration
	int a2[] = { 1, 2 }, b2[] = { 3, 4, 5 };
	//
	char s[] = VOID_FUNC_N_STR(abc),  t[3] = VOID_FUNC_N_STR2(abc); //first is 4 chars, second is 3 chars.
	char s2[] = { 'a', 'b', 'c', '\0' }, t2[] = { 'a', 'b', 'c' }; //same as before
	char *sp = "abc"; //a pointer
	//Arrays can be initialized to correspond to the elements of an enumeration by using designators
	enum { member_one, member_two };
	const char *nm[] = {
		[member_two] = "member two",
		[member_one] = "member one",
	};
	//Structure members can be initialized to nonzero values without depending on their order
	struct { int a[3], b; } w2[] = { [0].a = {1}, [1].a[0] = 2 };
}

void test18(){
	char *s;
	while (*s++ != '\0'); //empty statmets
	//
	int loop1 = 1;
	while (loop1) { /* ... */
		int loop2 = 1;
		while (loop2) { /* ... */
			int want_out = 1;
			if (want_out)
				goto end_loop1;
		}
		end_loop1: ; //empty statement
	}
}

void test19(){
	int expr = 10;
	switch (expr){
			int i = 4; //code will never be executed
		case 0:
			i = 17;
			/* falls through into default code */
		default:
			printf("%d\n", i);
	}
}

void test20(){
	/* ... */
	goto first_time;
	for (;;) {
		int need_to_reinitialize = 0;
		if (need_to_reinitialize) {
			// reinitialize-only code
			/* ... */
		first_time:
			// general initialization code /* ... */
			continue;
		}
	}
}

struct s4 { double i; } f4(void);

struct s4 f4(void){
	struct s4 r;
	return r;
}

void test21(){
	union {
		struct {
			int f1;
			struct s4 f2;
		} u1;
		struct {
			struct s4 f3;
			int f4;
		} u2;
	} g;
	g.u2.f3 = f4();
}




