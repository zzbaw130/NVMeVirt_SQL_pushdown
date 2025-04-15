// SPDX-License-Identifier: GPL-2.0-only

#ifndef _LIB_NVMEV_H
#define _LIB_NVMEV_H

#include <linux/pci.h>
#include <linux/msi.h>
#include <asm/apic.h>

#include "nvme.h"

#define CONFIG_NVMEV_IO_WORKER_BY_SQ
#undef CONFIG_NVMEV_FAST_X86_IRQ_HANDLING

#define CONFIG_NVMEV_VERBOSE
#define CONFIG_NVMEV_DEBUG
#define CONFIG_NVMEV_DEBUG_VERBOSE
#define PERF_DEBUG

/*
 * If CONFIG_NVMEVIRT_IDLE_TIMEOUT is set, sleep for a jiffie after
 * CONFIG_NVMEVIRT_IDLE_TIMEOUT seconds have passed to lower CPU power
 * consumption on idle.
 *
 * This may introduce a (1000/CONFIG_HZ) ms processing latency penalty
 * when exiting an I/O idle state.
 *
 * The default is set to 60 seconds, which is extremely conservative and
 * should not have an impact on I/O testing.
 */
#define CONFIG_NVMEVIRT_IDLE_TIMEOUT 60

/*************************/
#define NVMEV_DRV_NAME "NVMeVirt"
#define NVMEV_VERSION 0x0110
#define NVMEV_DEVICE_ID	NVMEV_VERSION
#define NVMEV_VENDOR_ID 0x0c51
#define NVMEV_SUBSYSTEM_ID	0x370d
#define NVMEV_SUBSYSTEM_VENDOR_ID NVMEV_VENDOR_ID

#define NVMEV_INFO(string, args...) printk(KERN_INFO "%s: " string, NVMEV_DRV_NAME, ##args)
#define NVMEV_ERROR(string, args...) printk(KERN_ERR "%s: " string, NVMEV_DRV_NAME, ##args)
#define NVMEV_ASSERT(x) BUG_ON((!(x)))

#ifdef CONFIG_NVMEV_DEBUG
#define  NVMEV_DEBUG(string, args...) printk(KERN_INFO "%s: " string, NVMEV_DRV_NAME, ##args)
#ifdef CONFIG_NVMEV_DEBUG_VERBOSE
#define  NVMEV_DEBUG_VERBOSE(string, args...) printk(KERN_INFO "%s: " string, NVMEV_DRV_NAME, ##args)
#else
#define  NVMEV_DEBUG_VERBOSE(string, args...)
#endif
#else
#define NVMEV_DEBUG(string, args...)
#define NVMEV_DEBUG_VERBOSE(string, args...)
#endif


#define NR_MAX_IO_QUEUE 72
#define NR_MAX_PARALLEL_IO 16384

#define NVMEV_INTX_IRQ 15

#define PAGE_OFFSET_MASK (PAGE_SIZE - 1)
#define PRP_PFN(x) ((unsigned long)((x) >> PAGE_SHIFT))

#define KB(k) ((k) << 10)
#define MB(m) ((m) << 20)
#define GB(g) ((g) << 30)

#define BYTE_TO_KB(b) ((b) >> 10)
#define BYTE_TO_MB(b) ((b) >> 20)
#define BYTE_TO_GB(b) ((b) >> 30)

#define MS_PER_SEC(s) ((s)*1000)
#define US_PER_SEC(s) (MS_PER_SEC(s) * 1000)
#define NS_PER_SEC(s) (US_PER_SEC(s) * 1000)

#define LBA_TO_BYTE(lba) ((lba) << LBA_BITS)
#define BYTE_TO_LBA(byte) ((byte) >> LBA_BITS)

#define BITMASK32_ALL (0xFFFFFFFF)
#define BITMASK64_ALL (0xFFFFFFFFFFFFFFFF)
#define ASSERT(X)

#include "ssd_config.h"

struct nvmev_sq_stat {
	unsigned int nr_dispatched;
	unsigned int nr_dispatch;
	unsigned int nr_in_flight;
	unsigned int max_nr_in_flight;
	unsigned long long total_io;
};

struct nvmev_submission_queue {
	int qid;
	int cqid;
	int priority;
	bool phys_contig;

	int queue_size;

	struct nvmev_sq_stat stat;

	struct nvme_command __iomem **sq;
	void *mapped;
};

struct nvmev_completion_queue {
	int qid;
	int irq_vector;
	bool irq_enabled;
	bool interrupt_ready;
	bool phys_contig;

	spinlock_t entry_lock;
	struct mutex irq_lock;

	int queue_size;

	int phase;
	int cq_head;
	int cq_tail;

	struct nvme_completion __iomem **cq;
	void *mapped;
};

struct nvmev_admin_queue {
	int phase;

	int sq_depth;
	int cq_depth;

	int cq_head;

	struct nvme_command __iomem **nvme_sq;
	struct nvme_completion __iomem **nvme_cq;
};

#define NR_SQE_PER_PAGE (PAGE_SIZE / sizeof(struct nvme_command))
#define NR_CQE_PER_PAGE (PAGE_SIZE / sizeof(struct nvme_completion))

#define SQ_ENTRY_TO_PAGE_NUM(entry_id) (entry_id / NR_SQE_PER_PAGE)
#define CQ_ENTRY_TO_PAGE_NUM(entry_id) (entry_id / NR_CQE_PER_PAGE)

#define SQ_ENTRY_TO_PAGE_OFFSET(entry_id) (entry_id % NR_SQE_PER_PAGE)
#define CQ_ENTRY_TO_PAGE_OFFSET(entry_id) (entry_id % NR_CQE_PER_PAGE)

struct nvmev_config {
	//Reserved memory address(byte), it is configured by "memmap_start" when execute insmod command
	unsigned long memmap_start;
	//Reserved memory size(byte), it is configured by "memmap_size" when execute insmod command
	unsigned long memmap_size;

	//Reserved storage address(byte), offset by 1MB from memmap_start
	unsigned long storage_start;
	//Reserved storage size(byte), equals (memmap_size - 1MB)
	unsigned long storage_size;

	//cpu number(core id) for dispatcher
	unsigned int cpu_nr_dispatcher;
	//number of io workers
	unsigned int nr_io_workers;
	//cpu numbers(core ids) for io workers
	unsigned int cpu_nr_io_workers[32];

	/* TODO Refactoring storage configurations */
	//Number of I/O units that operate in parallel
	unsigned int nr_io_units;
	//Size of each I/O unit (2^)
	unsigned int io_unit_shift;

	//Read time in nanoseconds
	unsigned int read_delay;
	//Read delay in nanoseconds
	unsigned int read_time;
	//Read trailing in nanoseconds
	unsigned int read_trailing;
	//Write time in nanoseconds
	unsigned int write_delay;
	//Write delay in nanoseconds
	unsigned int write_time;
	//Write trailing in nanoseconds
	unsigned int write_trailing;
};

struct nvmev_io_work {
	int sqid;
	int cqid;

	int sq_entry;
	unsigned int command_id;

	unsigned long long nsecs_start;
	unsigned long long nsecs_target;

	unsigned long long nsecs_enqueue;
	unsigned long long nsecs_copy_start;
	unsigned long long nsecs_copy_done;
	unsigned long long nsecs_cq_filled;

	bool is_copied;
	bool is_completed;

	unsigned int status;
	unsigned int result0;
	unsigned int result1;

	bool is_internal;
	void *write_buffer;
	size_t buffs_to_release;

	unsigned int next, prev;
};

struct nvmev_io_worker {
	struct nvmev_io_work *work_queue;

	unsigned int free_seq; /* free io req head index */
	unsigned int free_seq_end; /* free io req tail index */
	unsigned int io_seq; /* io req head index */
	unsigned int io_seq_end; /* io req tail index */

	unsigned long long latest_nsecs;

	unsigned int id;
	struct task_struct *task_struct;
	char thread_name[32];
};

struct nvmev_dev {
	// virtual bus scaned by the OS
	struct pci_bus *virt_bus;
	// Space for pci_header and all capabilities, 4KB
	void *virtDev;
	//PCI Header, start from 0x00, size 64Byte
	struct pci_header *pcihdr;
	//Power Management Capability, start from 0x40, size 8Byte
	struct pci_pm_cap *pmcap;
	//MSI/MSI-X Capability, start from 0x50, size 12Byte
	struct pci_msix_cap *msixcap;
	//PCI Express Capability, start from 0x60, size 36Byte
	struct pcie_cap *pciecap;
	//PCI Express Extended Capabilities, start from 0x100, take up the remaining space(max 3840Byte)
	struct pci_ext_cap *extcap;

	// virtual device scaned by the OS
	struct pci_dev *pdev;

	struct nvmev_config config;
	// nvmev_dispatcher space pointer
	struct task_struct *nvmev_dispatcher;

	// Storage Area Start Adress
	void *storage_mapped;

	// io_workers space pointer
	struct nvmev_io_worker *io_workers;
	unsigned int io_worker_turn;

	void __iomem *msix_table;

	bool intx_disabled;

	// old register space, it is used to check whether any registers have changed
	struct __nvme_bar *old_bar;
	// new register space, it occupies PAGE_SIZE of space
	struct nvme_ctrl_regs __iomem *bar;

	// old doorbells space, it is used to check whether any doorbells have changed
	u32 *old_dbs;
	/**
	 * new doorbells space, it occupies PAGE_SIZE of space, next to bar
	 * dbs[0]: Submission Queue 0 Tail Doorbell(Admin)
	 * dbs[1]: Completion Queue 0 Head Doorbell(Admin)
	 * dbs[2]: Submission Queue 1 Tail Doorbell
	 * dbs[3]: Completion Queue 1 Head Doorbell
	 * ...
	 * dbs[2*n]: Submission Queue n Tail Doorbell
	 * dbs[2*n+1]: Completion Queue n Head Doorbell
	 */
	u32 __iomem *dbs;

	// namespace config
	struct nvmev_ns *ns;
	// number of namespace
	unsigned int nr_ns;
	// number of submission queue
	unsigned int nr_sq;
	// number of completion queue
	unsigned int nr_cq;

	struct nvmev_admin_queue *admin_q;
	struct nvmev_submission_queue *sqes[NR_MAX_IO_QUEUE + 1];
	struct nvmev_completion_queue *cqes[NR_MAX_IO_QUEUE + 1];

	// Maximum Data Transfer Size,  when converted to bytes, it equals page_size * (2 ^ mdts)
	unsigned int mdts;

	// file root in /proc, default is /proc/nvme
	struct proc_dir_entry *proc_root;
	// read_times file, the path is /proc/nvme/read_times
	struct proc_dir_entry *proc_read_times;
	// write_times file, the path is /proc/nvme/write_times
	struct proc_dir_entry *proc_write_times;
	// io_units file, the path is /proc/nvme/io_units
	struct proc_dir_entry *proc_io_units;
	// stat file, the path is /proc/nvme/stat
	struct proc_dir_entry *proc_stat;
	// debug file, the path is /proc/nvme/debug
	struct proc_dir_entry *proc_debug;

	// io units space start address
	unsigned long long *io_unit_stat;
};

struct nvmev_request {
	struct nvme_command *cmd;
	uint32_t sq_id;
	uint64_t nsecs_start;
};

struct nvmev_result {
	uint32_t status;
	uint64_t nsecs_target;
};

struct nvmev_ns {
	uint32_t id;
	uint32_t csi;
	uint64_t size;
	void *mapped;

	/*conv ftl or zns or kv*/
	uint32_t nr_parts; // partitions
	void *ftls; // ftl instances. one ftl per partition

	/*io command handler*/
	bool (*proc_io_cmd)(struct nvmev_ns *ns, struct nvmev_request *req,
			    struct nvmev_result *ret);

	/*specific CSS io command identifier*/
	bool (*identify_io_cmd)(struct nvmev_ns *ns, struct nvme_command cmd);
	/*specific CSS io command processor*/
	unsigned int (*perform_io_cmd)(struct nvmev_ns *ns, struct nvme_command *cmd,
				       uint32_t *status);
};

// VDEV Init, Final Function
extern struct nvmev_dev *nvmev_vdev;
struct nvmev_dev *VDEV_INIT(void);
void VDEV_FINALIZE(struct nvmev_dev *nvmev_vdev);

// OPS_PCI
bool nvmev_proc_bars(void);
bool NVMEV_PCI_INIT(struct nvmev_dev *dev);
void nvmev_signal_irq(int msi_index);

// OPS ADMIN QUEUE
void nvmev_proc_admin_sq(int new_db, int old_db);
void nvmev_proc_admin_cq(int new_db, int old_db);

// OPS I/O QUEUE
struct buffer;
void schedule_internal_operation(int sqid, unsigned long long nsecs_target,
				struct buffer *write_buffer, size_t buffs_to_release);
void NVMEV_IO_WORKER_INIT(struct nvmev_dev *nvmev_vdev);
void NVMEV_IO_WORKER_FINAL(struct nvmev_dev *nvmev_vdev);
int nvmev_proc_io_sq(int qid, int new_db, int old_db);
void nvmev_proc_io_cq(int qid, int new_db, int old_db);

#endif /* _LIB_NVMEV_H */
