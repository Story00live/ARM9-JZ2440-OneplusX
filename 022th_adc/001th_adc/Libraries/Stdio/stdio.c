/**
  *********************************************************************************************************
  * @file    stdio.c
  * @author  Devilhart -- KK
  * @version V0.0.1
  * @date    2019-03-03
  * @brief   1TAB = 5Speace
  *********************************************************************************************************
  * @attention
  *
  *
  *
  *********************************************************************************************************
  */

#include "s3c2440_soc.h"

/* -'ntoa' conversion buffer size, this must be big enough to hold one converted-
 * -numeric number including padded zeros (dynamically created on stack)-
 * -default: 32 byte-
 */
#ifndef PRINTF_NTOA_BUFFER_SIZE
#define PRINTF_NTOA_BUFFER_SIZE			32U
#endif

/* -'ftoa' conversion buffer size, this must be big enough to hold one converted-
 * -float number including padded zeros (dynamically created on stack)-
 * -default: 32 byte-
 */
#ifndef PRINTF_FTOA_BUFFER_SIZE
#define PRINTF_FTOA_BUFFER_SIZE			32U
#endif

/* -support for the floating point type (%f)-
 * -default: activated-
 */
#ifndef PRINTF_DISABLE_SUPPORT_FLOAT
#define PRINTF_SUPPORT_FLOAT
#endif

/* -support for exponential floating point notation (%e/%g)-
 * -default: activated-
 */
#ifndef PRINTF_DISABLE_SUPPORT_EXPONENTIAL
#define PRINTF_SUPPORT_EXPONENTIAL
#endif

/* -define the default floating point precision-
 * -default: 6 digits-
 */
#ifndef PRINTF_DEFAULT_FLOAT_PRECISION
#define PRINTF_DEFAULT_FLOAT_PRECISION		6U
#endif

/* -define the largest float suitable to print with %f-
 * -default: 1e9-
 */
#ifndef PRINTF_MAX_FLOAT
#define PRINTF_MAX_FLOAT					1e9
#endif

/* -support for the long long types (%llu or %p)-
 * -default: activated-
 */
#ifndef PRINTF_DISABLE_SUPPORT_LONG_LONG
#define PRINTF_SUPPORT_LONG_LONG
#endif

/* -support for the ptrdiff_t type (%t)-
 * -ptrdiff_t is normally defined in <stddef.h> as long or long long type-
 * -default: activated-
 */
#ifndef PRINTF_DISABLE_SUPPORT_PTRDIFF_T
#define PRINTF_SUPPORT_PTRDIFF_T
#endif

/* Internal flag definitions */
#define FLAGS_ZEROPAD					(1U <<  0U)
#define FLAGS_LEFT						(1U <<  1U)
#define FLAGS_PLUS						(1U <<  2U)
#define FLAGS_SPACE						(1U <<  3U)
#define FLAGS_HASH						(1U <<  4U)
#define FLAGS_UPPERCASE					(1U <<  5U)
#define FLAGS_CHAR						(1U <<  6U)
#define FLAGS_SHORT						(1U <<  7U)
#define FLAGS_LONG						(1U <<  8U)
#define FLAGS_LONG_LONG					(1U <<  9U)
#define FLAGS_PRECISION					(1U << 10U)
#define FLAGS_ADAPT_EXP					(1U << 11U)

#if defined(PRINTF_SUPPORT_FLOAT)
#include <float.h>
#endif

typedef int (*out_putchar)(int cByte);

out_putchar _putchar = NULL;

/* wrapper (used as buffer) for output function type */
typedef struct
{
	void		(*fct)(char character, void* arg);
	void*	arg;
} out_fct_wrap_type;

typedef void (*out_fct_type)(char character, void* buffer, size_t idx, size_t maxlen);

static inline void _out_null(char character, void* buffer, size_t idx, size_t maxlen)
{
	(void)character; (void)buffer; (void)idx; (void)maxlen;
}

static inline void _out_char(char character, void* buffer, size_t idx, size_t maxlen)
{
	(void)buffer; (void)idx; (void)maxlen;
	if (character) _putchar(character);
}

static inline void _out_fct(char character, void* buffer, size_t idx, size_t maxlen)
{
	(void)idx; (void)maxlen;
	if (character) {
		((out_fct_wrap_type*)buffer)->fct(character, ((out_fct_wrap_type*)buffer)->arg);
	}
}

static inline void _out_buffer(char character, void* buffer, size_t idx, size_t maxlen)
{
	if (idx < maxlen) {
		((char*)buffer)[idx] = character;
	}
}

/**********************************************************************************************************
 @Function			static inline unsigned int _strnlen_s(const char* str, size_t maxsize)
 @Description			_strnlen_s
 @Input				
 @Return				
**********************************************************************************************************/
static inline unsigned int _strnlen_s(const char* str, size_t maxsize)
{
	const char* s;
	
	for (s = str; *s && maxsize--; ++s);
	
	return (unsigned int)(s - str);
}

/**********************************************************************************************************
 @Function			static inline bool _is_digit(char ch)
 @Description			_is_digit			: 检查ch是否为(0-9)数字
 @Input				ch
 @Return				bool
**********************************************************************************************************/
static inline bool _is_digit(char ch)
{
	return (ch >= '0') && (ch <= '9');
}

/**********************************************************************************************************
 @Function			static unsigned int _atoi(const char** str)
 @Description			_atoi			: ASCII字符串转为数值
 @Input				str
 @Return				unsigned int
**********************************************************************************************************/
static unsigned int _atoi(const char** str)
{
	unsigned int i = 0U;
	
	while (_is_digit(**str)) {
		i = i * 10U + (unsigned int)(*((*str)++) - '0');
	}
	
	return i;
}

/**********************************************************************************************************
 @Function			static size_t _out_rev(out_fct_type out, char* buffer, size_t idx, size_t maxlen, const char* buf, size_t len, unsigned int width, unsigned int flags)
 @Description			_out_rev			: output the specified string in reverse, taking care of any zero-padding
 @Input				
 @Return				
**********************************************************************************************************/
static size_t _out_rev(out_fct_type out, char* buffer, size_t idx, size_t maxlen, const char* buf, size_t len, unsigned int width, unsigned int flags)
{
	const size_t start_idx = idx;
	
	/* pad spaces up to given width */
	if (!(flags & FLAGS_LEFT) && !(flags & FLAGS_ZEROPAD)) {
		for (size_t i = len; i < width; i++) {
			out(' ', buffer, idx++, maxlen);
		}
	}
	
	/* reverse string */
	while (len) {
		out(buf[--len], buffer, idx++, maxlen);
	}
	
	/* append pad spaces up to given width */
	if (flags & FLAGS_LEFT) {
		while (idx - start_idx < width) {
			out(' ', buffer, idx++, maxlen);
		}
	}
	
	return idx;
}

/**********************************************************************************************************
 @Function			static size_t _ntoa_format(out_fct_type out, char* buffer, size_t idx, size_t maxlen, char* buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags)
 @Description			_ntoa_format		: internal itoa format
 @Input				
 @Return				
**********************************************************************************************************/
static size_t _ntoa_format(out_fct_type out, char* buffer, size_t idx, size_t maxlen, char* buf, size_t len, bool negative, unsigned int base, unsigned int prec, unsigned int width, unsigned int flags)
{
	if (!(flags & FLAGS_LEFT)) {
		if (width && (flags & FLAGS_ZEROPAD) && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
			width--;
		}
		while ((len < prec) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
		while ((flags & FLAGS_ZEROPAD) && (len < width) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
	}
	
	if (flags & FLAGS_HASH) {
		if (!(flags & FLAGS_PRECISION) && len && ((len == prec) || (len == width))) {
			len--;
			if (len && (base == 16U)) {
				len--;
			}
		}
		if ((base == 16U) && !(flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = 'x';
		}
		else if ((base == 16U) && (flags & FLAGS_UPPERCASE) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = 'X';
		}
		else if ((base == 2U) && (len < PRINTF_NTOA_BUFFER_SIZE)) {
			buf[len++] = 'b';
		}
		if (len < PRINTF_NTOA_BUFFER_SIZE) {
			buf[len++] = '0';
		}
	}
	
	if (len < PRINTF_NTOA_BUFFER_SIZE) {
		if (negative) {
			buf[len++] = '-';
		}
		else if (flags & FLAGS_PLUS) {
			buf[len++] = '+';
		}
		else if (flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}
	
	return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

/**********************************************************************************************************
 @Function			static size_t _ntoa_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, unsigned int flags)
 @Description			_ntoa_long		: internal itoa for 'long' type
 @Input				
 @Return				
**********************************************************************************************************/
static size_t _ntoa_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long value, bool negative, unsigned long base, unsigned int prec, unsigned int width, unsigned int flags)
{
	char buf[PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0U;
	
	if (!value) {
		flags &= ~FLAGS_HASH;
	}
	
	if (!(flags & FLAGS_PRECISION) || value) {
		do {
			const char digit = (char)(value % base);
			buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}
	
	return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}

#if defined(PRINTF_SUPPORT_LONG_LONG)
/**********************************************************************************************************
 @Function			static size_t _ntoa_long_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags)
 @Description			_ntoa_long_long	: internal itoa for 'long long' type
 @Input				
 @Return				
**********************************************************************************************************/
static size_t _ntoa_long_long(out_fct_type out, char* buffer, size_t idx, size_t maxlen, unsigned long long value, bool negative, unsigned long long base, unsigned int prec, unsigned int width, unsigned int flags)
{
	char buf[PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0U;
	
	if (!value) {
		flags &= ~FLAGS_HASH;
	}
	
	if (!(flags & FLAGS_PRECISION) || value) {
		do {
			const char digit = (char)(value % base);
			buf[len++] = digit < 10 ? '0' + digit : (flags & FLAGS_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}
	
	return _ntoa_format(out, buffer, idx, maxlen, buf, len, negative, (unsigned int)base, prec, width, flags);
}
#endif

#if defined(PRINTF_SUPPORT_FLOAT)

#if defined(PRINTF_SUPPORT_EXPONENTIAL)
static size_t _etoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags);
#endif

/**********************************************************************************************************
 @Function			static size_t _ftoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
 @Description			_ftoa			: internal _ftoa
 @Input				
 @Return				
**********************************************************************************************************/
static size_t _ftoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
{
	char buf[PRINTF_FTOA_BUFFER_SIZE];
	size_t len  = 0U;
	double diff = 0.0;
	
	/* powers of 10 */
	static const double pow10[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
	
	/* test for special values */
	if (value != value)
		return _out_rev(out, buffer, idx, maxlen, "nan", 3, width, flags);
	if (value < -DBL_MAX)
		return _out_rev(out, buffer, idx, maxlen, "fni-", 4, width, flags);
	if (value > DBL_MAX)
		return _out_rev(out, buffer, idx, maxlen, (flags & FLAGS_PLUS) ? "fni+" : "fni", (flags & FLAGS_PLUS) ? 4U : 3U, width, flags);
	
	/* test for very large values */
	if ((value > PRINTF_MAX_FLOAT) || (value < -PRINTF_MAX_FLOAT)) {
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
		return _etoa(out, buffer, idx, maxlen, value, prec, width, flags);
#else
		return 0U;
#endif
	}
	
	/* test for negative */
	bool negative = false;
	if (value < 0) {
		negative = true;
		value = 0 - value;
	}
	
	/* set default precision to 6, if not set explicitly */
	if (!(flags & FLAGS_PRECISION)) {
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	}
	/* limit precision to 9, cause a prec >= 10 can lead to overflow errors */
	while ((len < PRINTF_FTOA_BUFFER_SIZE) && (prec > 9U)) {
		buf[len++] = '0';
		prec--;
	}
	
	int whole = (int)value;
	double tmp = (value - whole) * pow10[prec];
	unsigned long frac = (unsigned long)tmp;
	diff = tmp - frac;
	
	if (diff > 0.5) {
		++frac;
		if (frac >= pow10[prec]) {
			frac = 0;
			++whole;
		}
	}
	else if (diff < 0.5) {
	}
	else if ((frac == 0U) || (frac & 1U)) {
		++frac;
	}
	
	if (prec == 0U) {
		diff = value - (double)whole;
		if ((!(diff < 0.5) || (diff > 0.5)) && (whole & 1)) {
			++whole;
		}
	}
	else {
		unsigned int count = prec;
		while (len < PRINTF_FTOA_BUFFER_SIZE) {
			--count;
			buf[len++] = (char)(48U + (frac % 10U));
			if (!(frac /= 10U)) {
				break;
			}
		}
		
		while ((len < PRINTF_FTOA_BUFFER_SIZE) && (count-- > 0U)) {
			buf[len++] = '0';
		}
		
		if (len < PRINTF_FTOA_BUFFER_SIZE) {
			buf[len++] = '.';
		}
	}
	
	while (len < PRINTF_FTOA_BUFFER_SIZE) {
		buf[len++] = (char)(48 + (whole % 10));
		if (!(whole /= 10)) {
			break;
		}
	}
	
	if (!(flags & FLAGS_LEFT) && (flags & FLAGS_ZEROPAD)) {
		if (width && (negative || (flags & (FLAGS_PLUS | FLAGS_SPACE)))) {
			width--;
		}
		while ((len < width) && (len < PRINTF_FTOA_BUFFER_SIZE)) {
			buf[len++] = '0';
		}
	}
	
	if (len < PRINTF_FTOA_BUFFER_SIZE) {
		if (negative) {
			buf[len++] = '-';
		}
		else if (flags & FLAGS_PLUS) {
			buf[len++] = '+';
		}
		else if (flags & FLAGS_SPACE) {
			buf[len++] = ' ';
		}
	}
	
	return _out_rev(out, buffer, idx, maxlen, buf, len, width, flags);
}

#if defined(PRINTF_SUPPORT_EXPONENTIAL)
/**********************************************************************************************************
 @Function			static size_t _etoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
 @Description			_etoa			: internal ftoa variant for exponential floating-point type, contributed by Martijn Jasperse <m.jasperse@gmail.com>
 @Input				
 @Return				
**********************************************************************************************************/
static size_t _etoa(out_fct_type out, char* buffer, size_t idx, size_t maxlen, double value, unsigned int prec, unsigned int width, unsigned int flags)
{
	/* check for NaN and special values */
	if ((value != value) || (value > DBL_MAX) || (value < -DBL_MAX)) {
		return _ftoa(out, buffer, idx, maxlen, value, prec, width, flags);
	}
	
	/* determine the sign */
	const bool negative = value < 0;
	if (negative) {
		value = -value;
	}
	
	/* default precision */
	if (!(flags & FLAGS_PRECISION)) {
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	}
	
	/* determine the decimal exponent */
	union {
		uint64_t U;
		double   F;
	} conv;
	
	conv.F = value;
	int exp2 = (int)((conv.U >> 52U) & 0x07FFU) - 1023;
	conv.U = (conv.U & ((1ULL << 52U) - 1U)) | (1023ULL << 52U);
	/* now approximate log10 from the log2 integer part and an expansion of ln around 1.5 */
	int expval = (int)(0.1760912590558 + exp2 * 0.301029995663981 + (conv.F - 1.5) * 0.289529654602168);
	/* now we want to compute 10^expval but we want to be sure it won't overflow */
	exp2 = (int)(expval * 3.321928094887362 + 0.5);
	const double z  = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
	const double z2 = z * z;
	conv.U = (uint64_t)(exp2 + 1023) << 52U;
	conv.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
	/* correct for rounding errors */
	if (value < conv.F) {
		expval--;
		conv.F /= 10;
	}
	
	/* the exponent format is "%+03d" and largest value is "307", so set aside 4-5 characters */
	unsigned int minwidth = ((expval < 100) && (expval > -100)) ? 4U : 5U;
	
	/* in "%g" mode, "prec" is the number of *significant figures* not decimals */
	if (flags & FLAGS_ADAPT_EXP) {
		/* do we want to fall-back to "%f" mode? */
		if ((value >= 1e-4) && (value < 1e6)) {
			if ((int)prec > expval) {
				prec = (unsigned)((int)prec - expval - 1);
			}
			else {
				prec = 0;
			}
			flags |= FLAGS_PRECISION;
			/* no characters in exponent */
			minwidth = 0U;
			expval   = 0;
		}
		else {
			/* we use one sigfig for the whole part */
			if ((prec > 0) && (flags & FLAGS_PRECISION)) {
				--prec;
			}
		}
	}
	
	/* will everything fit? */
	unsigned int fwidth = width;
	if (width > minwidth) {
		/* we didn't fall-back so subtract the characters required for the exponent */
		fwidth -= minwidth;
	}
	else {
		/* not enough characters, so go back to default sizing */
		fwidth = 0U;
	}
	if ((flags & FLAGS_LEFT) && minwidth) {
		/* if we're padding on the right, DON'T pad the floating part */
		fwidth = 0U;
	}
	
	/* rescale the float value */
	if (expval) {
		value /= conv.F;
	}
	
	/* output the floating part */
	const size_t start_idx = idx;
	idx = _ftoa(out, buffer, idx, maxlen, negative ? -value : value, prec, fwidth, flags & ~FLAGS_ADAPT_EXP);
	
	/* output the exponent part */
	if (minwidth) {
		/* output the exponential symbol */
		out((flags & FLAGS_UPPERCASE) ? 'E' : 'e', buffer, idx++, maxlen);
		/* output the exponent value */
		idx = _ntoa_long(out, buffer, idx, maxlen, (expval < 0) ? -expval : expval, expval < 0, 10, 0, minwidth-1, FLAGS_ZEROPAD | FLAGS_PLUS);
		/* might need to right-pad spaces */
		if (flags & FLAGS_LEFT) {
			while (idx - start_idx < width) out(' ', buffer, idx++, maxlen);
		}
	}
	
	return idx;
}
#endif
#endif

/**********************************************************************************************************
 @Function			static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va)
 @Description			_vsnprintf
 @Input				
 @Return				
**********************************************************************************************************/
static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va)
{
	unsigned int flags, width, precision, n;
	size_t idx = 0U;
	
	/* buffer is NULL of use null output function */
	if (!buffer) out = _out_null;
	
	while (*format) {
		/* format specifier?  %[flags][width][.precision][length] */
		if (*format != '%') {
			/* no, is normal */
			out(*format, buffer, idx++, maxlen);
			format++;
			continue;
		}
		else {
			/* yes, evaluate it */
			format++;
		}
		
		/* evaluate flage */
		flags = 0U;
		do {
			switch (*format) {
				case '0': flags |= FLAGS_ZEROPAD;	format++; n = 1U; break;
				case '-': flags |= FLAGS_LEFT;	format++; n = 1U; break;
				case '+': flags |= FLAGS_PLUS;	format++; n = 1U; break;
				case ' ': flags |= FLAGS_SPACE;	format++; n = 1U; break;
				case '#': flags |= FLAGS_HASH;	format++; n = 1U; break;
				default :								n = 0U; break;
			}
		} while (n);
		
		/* evaluate width field */
		width = 0U;
		if (_is_digit(*format)) {
			width = _atoi(&format);
		}
		else if (*format == '*') {
			const int w = va_arg(va, int);
			if (w < 0) {
				flags |= FLAGS_LEFT;
				width = (unsigned int)-w;
			}
			else {
				width = (unsigned int)w;
			}
			format++;
		}
		
		/* evaluate precision field */
		precision = 0U;
		if (*format == '.') {
			flags |= FLAGS_PRECISION;
			format++;
			if (_is_digit(*format)) {
				precision = _atoi(&format);
			}
			else if (*format == '*') {
				const int prec = (int)va_arg(va, int);
				precision = prec > 0 ? (unsigned int)prec : 0U;
				format++;
			}
		}
		
		/* evaluate length field */
		switch (*format) {
			case 'l' :
				flags |= FLAGS_LONG;
				format++;
				if (*format == 'l') {
					flags |= FLAGS_LONG_LONG;
					format++;
				}
				break;
			
			case 'h' :
				flags |= FLAGS_SHORT;
				format++;
				if (*format == 'h') {
					flags |= FLAGS_CHAR;
					format++;
				}
				break;
#if defined(PRINTF_SUPPORT_PTRDIFF_T)
			case 't' :
				flags |= (sizeof(ptrdiff_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;
#endif
			case 'j' :
				flags |= (sizeof(intmax_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;
			
			case 'z' :
				flags |= (sizeof(size_t) == sizeof(long) ? FLAGS_LONG : FLAGS_LONG_LONG);
				format++;
				break;
			
			default :
				break;
		}
		
		/* evaluate specifier */
		switch (*format) {
			case 'd' :
			case 'i' :
			case 'u' :
			case 'x' :
			case 'X' :
			case 'o' :
			case 'b' : {
				unsigned int base;
				if (*format == 'x' || *format == 'X') {
					base = 16U;
				}
				else if (*format == 'o') {
					base =  8U;
				}
				else if (*format == 'b') {
					base =  2U;
				}
				else {
					base = 10U;
					flags &= ~FLAGS_HASH;
				}
				
				if (*format == 'X') {
					flags |= FLAGS_UPPERCASE;
				}
				
				if ((*format != 'i') && (*format != 'd')) {
					flags &= ~(FLAGS_PLUS | FLAGS_SPACE);
				}
				
				if (flags & FLAGS_PRECISION) {
					flags &= ~FLAGS_ZEROPAD;
				}
				
				if ((*format == 'i') || (*format == 'd')) {
					if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
						const long long value = va_arg(va, long long);
						idx = _ntoa_long_long(out, buffer, idx, maxlen, (unsigned long long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
#endif
					}
					else if (flags & FLAGS_LONG) {
						const long value = va_arg(va, long);
						idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
					}
					else {
						const int value = (flags & FLAGS_CHAR) ? (char)va_arg(va, int) : (flags & FLAGS_SHORT) ? (short int)va_arg(va, int) : va_arg(va, int);
						idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned int)(value > 0 ? value : 0 - value), value < 0, base, precision, width, flags);
					}
				}
				else {
					if (flags & FLAGS_LONG_LONG) {
#if defined(PRINTF_SUPPORT_LONG_LONG)
						idx = _ntoa_long_long(out, buffer, idx, maxlen, va_arg(va, unsigned long long), false, base, precision, width, flags);
#endif
					}
					else if (flags & FLAGS_LONG) {
						idx = _ntoa_long(out, buffer, idx, maxlen, va_arg(va, unsigned long), false, base, precision, width, flags);
					}
					else {
						const unsigned int value = (flags & FLAGS_CHAR) ? (unsigned char)va_arg(va, unsigned int) : (flags & FLAGS_SHORT) ? (unsigned short int)va_arg(va, unsigned int) : va_arg(va, unsigned int);
						idx = _ntoa_long(out, buffer, idx, maxlen, value, false, base, precision, width, flags);
					}
				}
				format++;
				break;
			}
#if defined(PRINTF_SUPPORT_FLOAT)
			case 'f' :
			case 'F' : {
				if (*format == 'F') flags |= FLAGS_UPPERCASE;
				idx = _ftoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
				format++;
				break;
			}
#if defined(PRINTF_SUPPORT_EXPONENTIAL)
			case 'e':
			case 'E':
			case 'g':
			case 'G': {
				if ((*format == 'g')||(*format == 'G')) flags |= FLAGS_ADAPT_EXP;
				if ((*format == 'E')||(*format == 'G')) flags |= FLAGS_UPPERCASE;
				idx = _etoa(out, buffer, idx, maxlen, va_arg(va, double), precision, width, flags);
				format++;
				break;
			}
#endif
#endif
			case 'c' : {
				unsigned int l = 1U;
				if (!(flags & FLAGS_LEFT)) {
					while (l++ < width) {
						out(' ', buffer, idx++, maxlen);
					}
				}
				out((char)va_arg(va, int), buffer, idx++, maxlen);
				if (flags & FLAGS_LEFT) {
					while (l++ < width) {
						out(' ', buffer, idx++, maxlen);
					}
				}
				format++;
				break;
			}
			case 's' : {
				const char* p = va_arg(va, char*);
				unsigned int l = _strnlen_s(p, precision ? precision : (size_t)-1);
				if (flags & FLAGS_PRECISION) {
					l = (l < precision ? l : precision);
				}
				if (!(flags & FLAGS_LEFT)) {
					while (l++ < width) {
						out(' ', buffer, idx++, maxlen);
					}
				}
				while ((*p != 0) && (!(flags & FLAGS_PRECISION) || precision--)) {
					out(*(p++), buffer, idx++, maxlen);
				}
				if (flags & FLAGS_LEFT) {
					while (l++ < width) {
						out(' ', buffer, idx++, maxlen);
					}
				}
				format++;
				break;
			}
			case 'p' : {
				width = sizeof(void*) * 2U;
				flags |= FLAGS_ZEROPAD | FLAGS_UPPERCASE;
#if defined(PRINTF_SUPPORT_LONG_LONG)
				const bool is_ll = sizeof(uintptr_t) == sizeof(long long);
				if (is_ll) {
					idx = _ntoa_long_long(out, buffer, idx, maxlen, (uintptr_t)va_arg(va, void*), false, 16U, precision, width, flags);
				}
				else {
#endif
					idx = _ntoa_long(out, buffer, idx, maxlen, (unsigned long)((uintptr_t)va_arg(va, void*)), false, 16U, precision, width, flags);
#if defined(PRINTF_SUPPORT_LONG_LONG)
				}
#endif
				format++;
				break;
			}
			case '%' : {
				out('%', buffer, idx++, maxlen);
				format++;
				break;
			}
			default : {
				out(*format, buffer, idx++, maxlen);
				format++;
				break;
			}
		}
	}
	
	out((char)0, buffer, idx < maxlen ? idx : maxlen - 1U, maxlen);
	
	return (int)idx;
}

/**********************************************************************************************************
 @Function			int printf_(const char* format, ...)
 @Description			printf_
 @Input				
 @Return				
**********************************************************************************************************/
int printf_(const char* format, ...)
{
	va_list va;
	
	va_start(va, format);
	char buffer[1];
	const int ret = _vsnprintf(_out_char, buffer, (size_t)-1, format, va);
	va_end(va);
	
	return ret;
}

/**********************************************************************************************************
 @Function			int printfln_(const char* format, ...)
 @Description			printfln_
 @Input				
 @Return				
**********************************************************************************************************/
int printfln_(const char* format, ...)
{
	va_list va;
	
	va_start(va, format);
	char buffer[1];
	const int ret = _vsnprintf(_out_char, buffer, (size_t)-1, format, va);
	va_end(va);
	
	_putchar('\r');
	_putchar('\n');
	
	return ret;
}

/**********************************************************************************************************
 @Function			int sprintf_(char* buffer, const char* format, ...)
 @Description			sprintf_
 @Input				
 @Return				
**********************************************************************************************************/
int sprintf_(char* buffer, const char* format, ...)
{
	va_list va;
	
	va_start(va, format);
	const int ret = _vsnprintf(_out_buffer, buffer, (size_t)-1, format, va);
	va_end(va);
	
	return ret;
}

/**********************************************************************************************************
 @Function			int snprintf_(char* buffer, size_t count, const char* format, ...)
 @Description			snprintf_
 @Input				
 @Return				
**********************************************************************************************************/
int snprintf_(char* buffer, size_t count, const char* format, ...)
{
	va_list va;
	
	va_start(va, format);
	const int ret = _vsnprintf(_out_buffer, buffer, count, format, va);
	va_end(va);
	
	return ret;
}

/**********************************************************************************************************
 @Function			void standard_putchar_callback(int (*__putchar)(int cByte))
 @Description			standard_putchar_callback
 @Input				__putchar
 @Return				void
**********************************************************************************************************/
void standard_putchar_callback(int (*__putchar)(int cByte))
{
	_putchar = __putchar;
}

/********************************************** END OF FLEE **********************************************/
