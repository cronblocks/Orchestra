#pragma once

// For trimming the char strings
#define RTRIM(x) {size_t __inr_=strlen(x)-1;while(__inr_>0&&x[__inr_]==' ')__inr_--;x[__inr_+1]='\0';}
#define LTRIM(x) {size_t __nsl_=0,__inl_=0;while(x[__nsl_]==' ')__nsl_++;while(x[__nsl_]!='\0'){x[__inl_]=x[__nsl_];__inl_++;__nsl_++;}x[__inl_]='\0';}
#define TRIM_CHARS(x) {size_t __tgt_=0,__src_=0;while(__src_<=strlen(x)){x[__tgt_]=x[__src_];__src_++;if(x[__tgt_]!='"'&&x[__tgt_]!='\''&&x[__tgt_]!='\n')__tgt_++;}}
#define TRIM(x) RTRIM(x) LTRIM(x) TRIM_CHARS(x)

// Find "match" in "target" and replace it with "replacement"
void string_replace(char* target, const char* match, const char* replacement, unsigned int target_size);

// Replace from "start_index" to "end_index" in "target" with "replacement"
void string_replace_at(char* target, int start_index, int end_index, const char* replacement, unsigned int target_size);

// Return a non-zero value if the "target" ends with "ends"
int string_ends_with(const char* target, const char* ends);
