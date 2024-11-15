/*
 * common fdt helper function
 *
 * qianfan Zhao <qianfanguijin@163.com>
 */
#ifndef FDT_EXPORT_H
#define FDT_EXPORT_H

#include "imgeditor.h"

/* osc24M: clk@01c20050 {
 * };
 *
 * The first id(osc24M) is @label and the next is @name.
 * label is existed only when '-@|--symbols' is enabled when compile dts.
 */
struct device_node {
	char				name[64];
	char				label[64];
	struct list_head		head;
	struct list_head		child;
	int				is_node;
	size_t				data_size;
	uint8_t				data[0];
};

struct device_node *
	device_node_search_phandle(struct device_node *root, uint32_t phandle);
struct device_node *
	device_node_find_byname(struct device_node *root, const char *name);
struct device_node *
	device_node_find_bypath(struct device_node *root, const char *path);
int device_node_read_u32(struct device_node *node, uint32_t *ret_data);
int device_node_read_u32_prop(struct device_node *node, const char *propname,
			      uint32_t *ret_data);
int device_node_delete(struct device_node *root);
int device_node_delete_bypath(struct device_node *root, const char *path);

typedef __be32				fdt32_t;
typedef __be64				fdt64_t;
#define fdt32_to_cpu(x)			be32_to_cpu(x)
#define cpu_to_fdt32(x)			cpu_to_be32(x)
#define fdt64_to_cpu(x)			be64_to_cpu(x)

struct fdt_header {
	fdt32_t magic;			 /* magic word FDT_MAGIC */
	fdt32_t totalsize;		 /* total size of DT block */
	fdt32_t off_dt_struct;		 /* offset to structure */
	fdt32_t off_dt_strings;		 /* offset to strings */
	fdt32_t off_mem_rsvmap;		 /* offset to memory reserve map */
	fdt32_t version;		 /* format version */
	fdt32_t last_comp_version;	 /* last compatible version */

	/* version 2 fields below */
	fdt32_t boot_cpuid_phys;	 /* Which physical CPU id we're
					    booting on */
	/* version 3 fields below */
	fdt32_t size_dt_strings;	 /* size of the strings block */

	/* version 17 fields below */
	fdt32_t size_dt_struct;		 /* size of the structure block */
};

struct fdt_editor_private_data {
	void				*dtb;
	uint32_t			totalsize;

	struct device_node		*root;

	int				keep_symbols;
	int				keep_aliases;
	int				keep_phandle;
	int				keep_fixups;

	int				is_dtbo;
	/* All extern node referenced by '__fixups__' */
	struct device_node		*dtbo_extern;
};

int fdt_editor_detect(struct fdt_editor_private_data *p, int force_type, int fd);
void fdt_editor_exit(struct fdt_editor_private_data *p);

#endif
