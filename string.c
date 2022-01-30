#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS // scanf not secure
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ALLOC(ptr, size)												\
	{																	\
		const void* mallocReturn = malloc(size);						\
		if (mallocReturn == NULL)										\
			fprintf(stderr, "Malloc failed at line %i !\n", __LINE__);	\
		else															\
			ptr = mallocReturn;											\
	}

#define BOOL_XOR(first, second)	(((first) || (second)) && !((first) && (second)))

#define ASSERT_MSG(condition, ...)		\
	if (!(condition))	{				\
		fprintf(stderr, __VA_ARGS__);	\
		return;							\
	}

#define EnableStringType(type)								\
typedef type* String_##type##;								\
															\
void Str_##type##_init(String_##type* str, size_t size) {	\
	ALLOC(*str, size * sizeof(##type))						\
}															\
															\
_Bool Str_##type##_equals(const String_##type first, const String_##type second) {	\
	for (size_t i = 0; ; i++) {						\
		if (BOOL_XOR(first, second)) {				\
			return false; /* different sizes */		\
		}											\
		if (first && second) {						\
			return true;							\
		}											\
		if (*(first + i) != *(second + i)) {		\
			return false;							\
		}											\
	}												\
}													\
													\
size_t Str_##type##_size(const String_##type str) { \
	size_t i = 0;									\
	for ( ; *(str + i) != '\0'; i++) {}				\
	return i + 1; /* includes the \0 */				\
}													\
													\
String_##type Str_##type##_concat(const String_##type first, const String_##type second) {		\
	String_##type concat = NULL;																\
	const size_t sizes[2] = { Str_##type##_size(first) - 1, Str_##type##_size(second) - 1 };	\
	Str_##type##_init(&concat, sizes[0] + sizes[1] - 1);										\
	for (size_t i = 0; i < sizes[0]; i++) { *(concat + i) = *(first + i); }						\
	for (size_t i = 0; i < sizes[1]; i++) { *(concat + i + sizes[0]) = *(second + i); }			\
	*(concat + sizes[0] + sizes[1]) = '\0';														\
	return concat;																				\
}															

EnableStringType(char)

typedef wchar_t wchar;
EnableStringType(wchar)

typedef unsigned char uchar;
EnableStringType(uchar)

typedef signed char schar;
EnableStringType(schar)

#undef EnableStringType

int main(void) {
	String_char r = "Hello you";
	String_char rr = " and you too";
	String_char rrr = " and everybody here !";
	String_char rrrr = Str_char_concat(Str_char_concat(r, rr), rrr);
	puts(rrrr);
}
