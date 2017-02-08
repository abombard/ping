#include "strerror.h"
#include "printf.h"

#ifdef __LINUX__
static char *err_str[] = {
	[ESUCCESS] = "SUCCESS",
	[E2BIG] = "Argument list too long ",
	[EACCES] = "Permission denied ",
	[EADDRINUSE] = "Address already in use ",
	[EADDRNOTAVAIL] = "Address not available ",
	[EAFNOSUPPORT] = "Address family not supported ",
	[EAGAIN] = "Resource temporarily unavailable ",
	[EALREADY] = "Connection already in progress ",
	[EBADE] = "Invalid exchange",
	[EBADF] = "Bad file descriptor ",
	[EBADFD] = "File descriptor in bad state",
	[EBADMSG] = "Bad message ",
	[EBADR] = "Invalid request descriptor",
	[EBADRQC] = "Invalid request code",
	[EBADSLT] = "Invalid slot",
	[EBUSY] = "Device or resource busy ",
	[ECANCELED] = "Operation canceled ",
	[ECHILD] = "No child processes ",
	[ECHRNG] = "Channel number out of range",
	[ECOMM] = "Communication error on send",
	[ECONNABORTED] = "Connection aborted ",
	[ECONNREFUSED] = "Connection refused ",
	[ECONNRESET] = "Connection reset ",
	[EDEADLK] = "Resource deadlock avoided ",
	[EDESTADDRREQ] = "Destination address required ",
	[EDOM] = "Mathematics argument out of domain of function ",
	[EDQUOT] = "Disk quota exceeded ",
	[EEXIST] = "File exists ",
	[EFAULT] = "Bad address ",
	[EFBIG] = "File too large ",
	[EHOSTDOWN] = "Host is down",
	[EHOSTUNREACH] = "Host is unreachable ",
	[EIDRM] = "Identifier removed ",
	[EILSEQ] = "Illegal byte sequence ",
	[EINPROGRESS] = "Operation in progress ",
	[EINTR] = "Interrupted function call .",
	[EINVAL] = "Invalid argument ",
	[EIO] = "Input/output error ",
	[EISCONN] = "Socket is connected ",
	[EISDIR] = "Is a directory ",
	[EISNAM] = "Is a named type file",
	[EKEYEXPIRED] = "Key has expired",
	[EKEYREJECTED] = "Key was rejected by service",
	[EKEYREVOKED] = "Key has been revoked",
	[EL2HLT] = "       EL2HLT          Level 2 halted",
	[EL2NSYNC] = "       EL2NSYNC        Level 2 not synchronized",
	[EL3HLT] = "       EL3HLT          Level 3 halted",
	[EL3RST] = "       EL3RST          Level 3 halted",
	[ELIBACC] = "Cannot access a needed shared library",
	[ELIBBAD] = "Accessing a corrupted shared library",
	[ELIBMAX] = "Attempting to link in too many shared libraries",
	[ELIBSCN] = "lib section in a.out corrupted",
	[ELIBEXEC] = "Cannot exec a shared library directly",
	[ELOOP] = "Too many levels of symbolic links ",
	[EMEDIUMTYPE] = "Wrong medium type",
	[EMFILE] = "Too many open files ",
	[EMLINK] = "Too many links ",
	[EMSGSIZE] = "Message too long ",
	[EMULTIHOP] = "Multihop attempted ",
	[ENAMETOOLONG] = "Filename too long ",
	[ENETDOWN] = "Network is down ",
	[ENETRESET] = "Connection aborted by network ",
	[ENETUNREACH] = "Network unreachable ",
	[ENFILE] = "Too many open files in system .",
	[ENOBUFS] = "No buffer space available ",
	[ENODATA] = "No message is available on the STREAM head read queue ",
	[ENODEV] = "No such device ",
	[ENOENT] = "No such file or directory ",
	[ENOEXEC] = "Exec format error ",
	[ENOKEY] = "Required key not available",
	[ENOLCK] = "No locks available ",
	[ENOLINK] = "Link has been severed ",
	[ENOMEDIUM] = "No medium found",
	[ENOMEM] = "Not enough space ",
	[ENOMSG] = "No message of the desired type ",
	[ENONET] = "Machine is not on the network",
	[ENOPKG] = "Package not installed",
	[ENOPROTOOPT] = "Protocol not available ",
	[ENOSPC] = "No space left on device ",
	[ENOSR] = "No STREAM resources ",
	[ENOSTR] = "Not a STREAM ",
	[ENOSYS] = "Function not implemented ",
	[ENOTBLK] = "Block device required",
	[ENOTCONN] = "The socket is not connected ",
	[ENOTDIR] = "Not a directory ",
	[ENOTEMPTY] = "Directory not empty ",
	[ENOTSOCK] = "Not a socket ",
	[ENOTSUP] = "Operation not supported ",
	[ENOTTY] = "Inappropriate I/O control operation ",
	[ENOTUNIQ] = "Name not unique on network",
	[ENXIO] = "No such device or address ",
	[EOVERFLOW] = "Value too large to be stored in data type ",
	[EPERM] = "Operation not permitted ",
	[EPFNOSUPPORT] = "Protocol family not supported",
	[EPIPE] = "Broken pipe ",
	[EPROTO] = "Protocol error ",
	[EPROTONOSUPPORT] = "Protocol not supported ",
	[EPROTOTYPE] = "Protocol wrong type for socket ",
	[ERANGE] = "Result too large ",
	[EREMCHG] = "Remote address changed",
	[EREMOTE] = "Object is remote",
	[EREMOTEIO] = "Remote I/O error",
	[ERESTART] = "Interrupted system call should be restarted",
	[EROFS] = "Read-only filesystem ",
	[ESHUTDOWN] = "Cannot send after transport endpoint shutdown",
	[ESPIPE] = "Invalid seek ",
	[ESOCKTNOSUPPORT] = "Socket type not supported",
	[ESRCH] = "No such process ",
	[ESTALE] = "Stale file handle ",
	[ESTRPIPE] = "Streams pipe error",
	[ETIME] = "Timer expired ",
	[ETIMEDOUT] = "Connection timed out ",
	[ETXTBSY] = "Text file busy ",
	[EUCLEAN] = "Structure needs cleaning",
	[EUNATCH] = "Protocol driver not attached",
	[EUSERS] = "Too many users",
	[EXDEV] = "Improper link ",
	[EXFULL] = "Exchange full",
	[ENOTREGISTER] = "Not registered",
	[EFTYPE] = "Inappropriate file type or format"
};
#endif

#ifdef __MACOSX__
static char *err_str[] = {
	[ESUCCESS] = "Success",
	[EPERM] = "Operation not permitted",
	[ENOENT] = "No such file or directory",
	[ESRCH] = "No such process",
	[EINTR] = "Interrupted system call",
	[EIO] = "Input/output error",
	[ENXIO] = "Device not configured",
	[E2BIG] = "Argument list too long",
	[ENOEXEC] = "Exec format error",
	[EBADF] = "Bad file descriptor",
	[ECHILD] = "No child processes",
	[EDEADLK] = "Resource deadlock avoided",
	[ENOMEM] = "Cannot allocate memory",
	[EACCES] = "Permission denied",
	[EFAULT] = "Bad address",
	[ENOTBLK] = "Block device required",
	[EBUSY] = "Device / Resource busy",
	[EEXIST] = "File exists",
	[EXDEV] = "Cross-device link",
	[ENODEV] = "Operation not supported by device",
	[ENOTDIR] = "Not a directory",
	[EISDIR] = "Is a directory",
	[EINVAL] = "Invalid argument",
	[ENFILE] = "Too many open files in system",
	[EMFILE] = "Too many open files",
	[ENOTTY] = "Inappropriate ioctl for device",
	[ETXTBSY] = "Text file busy",
	[EFBIG] = "File too large",
	[ENOSPC] = "No space left on device",
	[ESPIPE] = "Illegal seek",
	[EROFS] = "Read-only file system",
	[EMLINK] = "Too many links",
	[EPIPE] = "Broken pipe",
	[EDOM] = "Numerical argument out of domain",
	[ERANGE] = "Result too large",
	[EAGAIN] = "Resource temporarily unavailable",
	[EINPROGRESS] = "Operation now in progress",
	[EALREADY] = "Operation already in progress",
	[ENOTSOCK] = "Socket operation on non-socket",
	[EDESTADDRREQ] = "Destination address required",
	[EMSGSIZE] = "Message too long",
	[EPROTOTYPE] = "Protocol wrong type for socket",
	[ENOPROTOOPT] = "Protocol not available",
	[EPROTONOSUPPORT] = "Protocol not supported",
	[ESOCKTNOSUPPORT] = "Socket type not supported",
	[ENOTSUP] = "Operation not supported",
	[EPFNOSUPPORT] = "Protocol family not supported",
	[EAFNOSUPPORT] = "Address family not supported by protocol family",
	[EADDRINUSE] = "Address already in use",
	[EADDRNOTAVAIL] = "Can't assign requested address",
	[ENETDOWN] = "Network is down",
	[ENETUNREACH] = "Network is unreachable",
	[ENETRESET] = "Network dropped connection on reset",
	[ECONNABORTED] = "Software caused connection abort",
	[ECONNRESET] = "Connection reset by peer",
	[ENOBUFS] = "No buffer space available",
	[EISCONN] = "Socket is already connected",
	[ENOTCONN] = "Socket is not connected",
	[ESHUTDOWN] = "Can't send after socket shutdown",
	[ETOOMANYREFS] = "Too many references: can't splice",
	[ETIMEDOUT] = "Operation timed out",
	[ECONNREFUSED] = "Connection refused",
	[ELOOP] = "Too many levels of symbolic links",
	[ENAMETOOLONG] = "File name too long",
	[EHOSTDOWN] = "Host is down",
	[EHOSTUNREACH] = "No route to host",
	[ENOTEMPTY] = "Directory not empty",
	[EPROCLIM] = "Too many processes",
	[EUSERS] = "Too many users",
	[EDQUOT] = "Disc quota exceeded",
	[ESTALE] = "Stale NFS file handle",
	[EREMOTE] = "Too many levels of remote in path",
	[EBADRPC] = "RPC struct is bad",
	[ERPCMISMATCH] = "RPC version wrong",
	[EPROGUNAVAIL] = "RPC prog. not avail",
	[EPROGMISMATCH] = "Program version wrong",
	[EPROCUNAVAIL] = "Bad procedure for program",
	[ENOLCK] = "No locks available",
	[ENOSYS] = "Function not implemented",
	[EFTYPE] = "Inappropriate file type or format",
	[EAUTH] = "Authentication error",
	[ENEEDAUTH] = "Need authenticator",
	[EPWROFF] = "Device power is off",
	[EDEVERR] = "Device error, e.g. paper out",
	[EOVERFLOW] = "Value too large to be stored in data type",
	[EBADEXEC] = "Bad executable",
	[EBADARCH] = "Bad CPU type in executable",
	[ESHLIBVERS] = "Shared library version mismatch",
	[EBADMACHO] = "Malformed Macho file",
	[ECANCELED] = "Operation canceled",
	[EIDRM] = "Identifier removed",
	[ENOMSG] = "No message of desired type",
	[EILSEQ] = "Illegal byte sequence",
	[ENOATTR] = "Attribute not found",
	[EBADMSG] = "Bad message",
	[EMULTIHOP] = "Reserved",
	[ENODATA] = "No message available on STREAM",
	[ENOLINK] = "Reserved",
	[ENOSR] = "No STREAM resources",
	[ENOSTR] = "Not a STREAM",
	[EPROTO] = "Protocol error",
	[ETIME] = "STREAM ioctl timeout",
	[EOPNOTSUPP] = "Operation not supported on socket",
	[ENOPOLICY] = "No such policy registered",
	[ENOTRECOVERABLE] = "State not recoverable",
	[EOWNERDEAD] = "Previous owner died",
	[EQFULL] = "Interface output queue is full",
	[ENOTREGISTER] = "Not registered",
	[EBADRQC] = "Invalid request code"
};
#endif

char	*strerror(int err_num)
{
	if (err_num < 0 || (unsigned long)err_num >= sizeof(err_str) / sizeof(err_str[0]))
		return ((void *)0);
	return (err_str[err_num]);
}

void	perror(char *s, int err)
{
	ft_fprintf(2, "%s: %s\n", s, strerror(err));
}

