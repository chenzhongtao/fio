#ifndef FIO_OS_NETBSD_H
#define FIO_OS_NETBSD_H

#define	FIO_OS	os_netbsd

#include <errno.h>
#include <lwp.h>
#include <sys/param.h>
#include <sys/statvfs.h>
/* XXX hack to avoid confilcts between rbtree.h and <sys/rb.h> */
#define	rb_node	_rb_node
#include <sys/sysctl.h>
#undef rb_node
#undef rb_left
#undef rb_right

#include "../file.h"

#define FIO_HAVE_ODIRECT
#define FIO_USE_GENERIC_BDEV_SIZE
#define FIO_USE_GENERIC_RAND
#define FIO_USE_GENERIC_INIT_RANDOM_STATE
#define FIO_HAVE_FS_STAT
#define FIO_HAVE_GETTID

#undef	FIO_HAVE_CPU_AFFINITY	/* XXX notyet */

#define OS_MAP_ANON		MAP_ANON

#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 4096
#endif

#define fio_swap16(x)	bswap16(x)
#define fio_swap32(x)	bswap32(x)
#define fio_swap64(x)	bswap64(x)

typedef off_t off64_t;

static inline int blockdev_invalidate_cache(struct fio_file *f)
{
	return EINVAL;
}

static inline unsigned long long os_phys_mem(void)
{
	int mib[2] = { CTL_HW, HW_PHYSMEM64 };
	uint64_t mem;
	size_t len = sizeof(mem);

	sysctl(mib, 2, &mem, &len, NULL, 0);
	return mem;
}

static inline int gettid(void)
{
	return (int) _lwp_self();
}

static inline unsigned long long get_fs_free_size(const char *path)
{
	unsigned long long ret;
	struct statvfs s;

	if (statvfs(path, &s) < 0)
		return -1ULL;

	ret = s.f_frsize;
	ret *= (unsigned long long) s.f_bfree;
	return ret;
}

#ifdef MADV_FREE
#define FIO_MADV_FREE	MADV_FREE
#endif

#endif
