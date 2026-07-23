/* SPDX-License-Identifier: MIT */

#ifndef DART_H
#define DART_H

#include <pongo.h>

typedef uint8_t u8;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int64_t s64;

#define BIT(n) (1ULL << (n))
#define GENMASK(high, low) (((~0ULL) << (low)) & (~0ULL >> (63 - (high))))
#define FIELD_PREP(mask, value) \
    ((((uint64_t)(value)) << __builtin_ctzll(mask)) & (mask))
#define FIELD_GET(mask, value) \
    ((((uint64_t)(value)) & (mask)) >> __builtin_ctzll(mask))

#define DART_PTR_ERR     BIT(63)
#define DART_IS_ERR(val) FIELD_GET(DART_PTR_ERR, val)

typedef struct dart_dev dart_dev_t;

enum dart_type_t {
    DART_T8020,
    DART_T8110,
    DART_T6000,
};

dart_dev_t *dart_init(uintptr_t base, u8 device, bool keep_pts, enum dart_type_t type);
dart_dev_t *dart_init_from_dt_mapper(dt_node_t *mapper, u32 reg_index, bool keep_pts);
dart_dev_t *dart_init_from_dt(dt_node_t *consumer, u32 reg_index, bool keep_pts);
u32 dart_get_error(dart_dev_t *dart);
u64 dart_get_error_address(dart_dev_t *dart);
dart_dev_t *dart_init_adt(const char *path, int instance, int device, bool keep_pts);
void dart_lock_adt(const char *path, int instance);
dart_dev_t *dart_init_fdt(void *dt, u32 phandle, int device, bool keep_pts);
int dart_setup_pt_region(dart_dev_t *dart, const char *path, int device, u64 vm_base);
int dart_map(dart_dev_t *dart, uintptr_t iova, uintptr_t paddr, size_t len);
int dart_map_flags(dart_dev_t *dart, uintptr_t iova, uintptr_t paddr, size_t len, u32 flags);
void dart_unmap(dart_dev_t *dart, uintptr_t iova, size_t len);
void dart_free_l2(dart_dev_t *dart, uintptr_t iova);
void *dart_translate(dart_dev_t *dart, uintptr_t iova);
void *dart_translate_silent(dart_dev_t *dart, uintptr_t iova);
u64 dart_search(dart_dev_t *dart, void *paddr);
u64 dart_find_iova(dart_dev_t *dart, s64 start, size_t len);
void dart_shutdown(dart_dev_t *dart);
u64 dart_vm_base(dart_dev_t *dart);

#endif
