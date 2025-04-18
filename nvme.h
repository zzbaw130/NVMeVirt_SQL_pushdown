// SPDX-License-Identifier: GPL-2.0-only
/*
 * Definitions for the NVM Express interface
 * Copyright (c) 2011-2014, Intel Corporation.
 */

#ifndef _LINUX_NVME_H
#define _LINUX_NVME_H

#include <linux/types.h>

struct nvme_bar {
	__u64 cap; /* Controller Capabilities */
	__u32 vs; /* Version */
	__u32 intms; /* Interrupt Mask Set */
	__u32 intmc; /* Interrupt Mask Clear */
	__u32 cc; /* Controller Configuration */
	__u32 rsvd1; /* Reserved */
	__u32 csts; /* Controller Status */
	__u32 nssr; /* Subsystem Reset */
	__u32 aqa; /* Admin Queue Attributes */
	__u64 asq; /* Admin SQ Base Address */
	__u64 acq; /* Admin CQ Base Address */
	__u32 cmbloc; /* Controller Memory Buffer Location */
	__u32 cmbsz; /* Controller Memory Buffer Size */
};

#define NVME_CAP_MQES(cap) ((cap) & 0xffff)
#define NVME_CAP_TIMEOUT(cap) (((cap) >> 24) & 0xff)
#define NVME_CAP_STRIDE(cap) (((cap) >> 32) & 0xf)
#define NVME_CAP_NSSRC(cap) (((cap) >> 36) & 0x1)
#define NVME_CAP_MPSMIN(cap) (((cap) >> 48) & 0xf)
#define NVME_CAP_MPSMAX(cap) (((cap) >> 52) & 0xf)

#define NVME_CMB_BIR(cmbloc) ((cmbloc) & 0x7)
#define NVME_CMB_OFST(cmbloc) (((cmbloc) >> 12) & 0xfffff)
#define NVME_CMB_SZ(cmbsz) (((cmbsz) >> 12) & 0xfffff)
#define NVME_CMB_SZU(cmbsz) (((cmbsz) >> 8) & 0xf)

#define NVME_CMB_WDS(cmbsz) ((cmbsz) & 0x10)
#define NVME_CMB_RDS(cmbsz) ((cmbsz) & 0x8)
#define NVME_CMB_LISTS(cmbsz) ((cmbsz) & 0x4)
#define NVME_CMB_CQS(cmbsz) ((cmbsz) & 0x2)
#define NVME_CMB_SQS(cmbsz) ((cmbsz) & 0x1)

enum {
	NVME_CC_ENABLE = 1 << 0,
	NVME_CC_CSS_NVM = 0 << 4,
	NVME_CC_MPS_SHIFT = 7,
	NVME_CC_ARB_RR = 0 << 11,
	NVME_CC_ARB_WRRU = 1 << 11,
	NVME_CC_ARB_VS = 7 << 11,
	NVME_CC_SHN_NONE = 0 << 14,
	NVME_CC_SHN_NORMAL = 1 << 14,
	NVME_CC_SHN_ABRUPT = 2 << 14,
	NVME_CC_SHN_MASK = 3 << 14,
	NVME_CC_IOSQES = 6 << 16,
	NVME_CC_IOCQES = 4 << 20,
	NVME_CSTS_RDY = 1 << 0,
	NVME_CSTS_CFS = 1 << 1,
	NVME_CSTS_NSSRO = 1 << 4,
	NVME_CSTS_SHST_NORMAL = 0 << 2,
	NVME_CSTS_SHST_OCCUR = 1 << 2,
	NVME_CSTS_SHST_CMPLT = 2 << 2,
	NVME_CSTS_SHST_MASK = 3 << 2,
};

struct nvme_id_power_state {
	__le16 max_power; /* centiwatts */
	__u8 rsvd2;
	__u8 flags;
	__le32 entry_lat; /* microseconds */
	__le32 exit_lat; /* microseconds */
	__u8 read_tput;
	__u8 read_lat;
	__u8 write_tput;
	__u8 write_lat;
	__le16 idle_power;
	__u8 idle_scale;
	__u8 rsvd19;
	__le16 active_power;
	__u8 active_work_scale;
	__u8 rsvd23[9];
};

enum {
	NVME_PS_FLAGS_MAX_POWER_SCALE = 1 << 0,
	NVME_PS_FLAGS_NON_OP_STATE = 1 << 1,
};

struct nvme_id_ctrl {
	__le16 vid;
	__le16 ssvid;
	char sn[20];
	char mn[40];
	char fr[8];
	__u8 rab;
	__u8 ieee[3];
	__u8 mic;
	__u8 mdts;
	__le16 cntlid;
	__le32 ver;
	__le32 rtd3r;
	__le32 rtd3e;
	__le32 oaes;
	__le32 ctratt;
	__le16 rrls;
	__u8 cntrltype;
	__u8 rsvd84[153];
	__le16 oacs;
	__u8 acl;
	__u8 aerl;
	__u8 frmw;
	__u8 lpa;
	__u8 elpe;
	__u8 npss;
	__u8 avscc;
	__u8 apsta;
	__le16 wctemp;
	__le16 cctemp;
	__u8 rsvd270[242];
	__u8 sqes;
	__u8 cqes;
	__u8 rsvd514[2];
	__le32 nn;
	__le16 oncs;
	__le16 fuses;
	__u8 fna;
	__u8 vwc;
	__le16 awun;
	__le16 awupf;
	__u8 nvscc;
	__u8 rsvd531;
	__le16 acwu;
	__u8 rsvd534[2];
	__le32 sgls;
	__u8 rsvd540[1508];
	struct nvme_id_power_state psd[32];
	__u8 vs[1024];
};

enum {
	NVME_CTRL_ONCS_COMPARE = 1 << 0,
	NVME_CTRL_ONCS_WRITE_UNCORRECTABLE = 1 << 1,
	NVME_CTRL_ONCS_DSM = 1 << 2,
	NVME_CTRL_VWC_PRESENT = 1 << 0,
};

struct nvme_lbaf {
	__le16 ms;
	__u8 ds;
	__u8 rp;
};

struct nvme_id_ns {
	__le64 nsze;
	__le64 ncap;
	__le64 nuse;
	__u8 nsfeat;
	__u8 nlbaf;
	__u8 flbas;
	__u8 mc;
	__u8 dpc;
	__u8 dps;
	__u8 nmic;
	__u8 rescap;
	__u8 fpi;
	__u8 rsvd33;
	__le16 nawun;
	__le16 nawupf;
	__le16 nacwu;
	__le16 nabsn;
	__le16 nabo;
	__le16 nabspf;
	__u16 rsvd46;
	__le64 nvmcap[2];
	__u8 rsvd64[40];
	__u8 nguid[16];
	__u8 eui64[8];
	struct nvme_lbaf lbaf[16];
	__u8 rsvd192[192];
	__u8 vs[3712];
};

struct nvme_id_ns_desc {
	__u8 nidt; //namespace id type
	__u8 nidl; //namespace id length
	__u8 rsvd[2];
	__u8 nid[4092];
};

struct nvme_lba_format_extension {
	__u8 zsze[8];
	__u8 zdes[8];
	__u8 rsv[56];
};

enum {
	NVME_NS_FEAT_THIN = 1 << 0,
	NVME_NS_FLBAS_LBA_MASK = 0xf,
	NVME_NS_FLBAS_META_EXT = 0x10,
	NVME_LBAF_RP_BEST = 0,
	NVME_LBAF_RP_BETTER = 1,
	NVME_LBAF_RP_GOOD = 2,
	NVME_LBAF_RP_DEGRADED = 3,
	NVME_NS_DPC_PI_LAST = 1 << 4,
	NVME_NS_DPC_PI_FIRST = 1 << 3,
	NVME_NS_DPC_PI_TYPE3 = 1 << 2,
	NVME_NS_DPC_PI_TYPE2 = 1 << 1,
	NVME_NS_DPC_PI_TYPE1 = 1 << 0,
	NVME_NS_DPS_PI_FIRST = 1 << 3,
	NVME_NS_DPS_PI_MASK = 0x7,
	NVME_NS_DPS_PI_TYPE1 = 1,
	NVME_NS_DPS_PI_TYPE2 = 2,
	NVME_NS_DPS_PI_TYPE3 = 3,
};

struct nvme_smart_log {
	__u8 critical_warning;
	__u8 temperature[2];
	__u8 avail_spare;
	__u8 spare_thresh;
	__u8 percent_used;
	__u8 rsvd6[26];
	__u8 data_units_read[16];
	__u8 data_units_written[16];
	__u8 host_reads[16];
	__u8 host_writes[16];
	__u8 ctrl_busy_time[16];
	__u8 power_cycles[16];
	__u8 power_on_hours[16];
	__u8 unsafe_shutdowns[16];
	__u8 media_errors[16];
	__u8 num_err_log_entries[16];
	__le32 warning_temp_time;
	__le32 critical_comp_time;
	__le16 temp_sensor[8];
	__u8 rsvd216[296];
};

enum {
	NVME_CMD_EFFECTS_CSUPP = 1 << 0,
	NVME_CMD_EFFECTS_LBCC = 1 << 1,
	NVME_CMD_EFFECTS_NCC = 1 << 2,
	NVME_CMD_EFFECTS_NIC = 1 << 3,
	NVME_CMD_EFFECTS_CCC = 1 << 4,
	NVME_CMD_EFFECTS_CSE_MASK = 3 << 16,
	NVME_CMD_EFFECTS_UUID_SEL = 1 << 19,
};

struct nvme_effects_log {
	__le32 acs[256];
	__le32 iocs[256];
	__u8 resv[2048];
};

enum {
	NVME_SMART_CRIT_SPARE = 1 << 0,
	NVME_SMART_CRIT_TEMPERATURE = 1 << 1,
	NVME_SMART_CRIT_RELIABILITY = 1 << 2,
	NVME_SMART_CRIT_MEDIA = 1 << 3,
	NVME_SMART_CRIT_VOLATILE_MEMORY = 1 << 4,
};

enum {
	NVME_AER_NOTICE_NS_CHANGED = 0x0002,
};

struct nvme_lba_range_type {
	__u8 type;
	__u8 attributes;
	__u8 rsvd2[14];
	__u64 slba;
	__u64 nlb;
	__u8 guid[16];
	__u8 rsvd48[16];
};

enum {
	NVME_LBART_TYPE_FS = 0x01,
	NVME_LBART_TYPE_RAID = 0x02,
	NVME_LBART_TYPE_CACHE = 0x03,
	NVME_LBART_TYPE_SWAP = 0x04,

	NVME_LBART_ATTRIB_TEMP = 1 << 0,
	NVME_LBART_ATTRIB_HIDE = 1 << 1,
};

struct nvme_reservation_status {
	__le32 gen;
	__u8 rtype;
	__u8 regctl[2];
	__u8 resv5[2];
	__u8 ptpls;
	__u8 resv10[13];
	struct {
		__le16 cntlid;
		__u8 rcsts;
		__u8 resv3[5];
		__le64 hostid;
		__le64 rkey;
	} regctl_ds[];
};

/* I/O commands */

#define NVME_OPCODES(op)                                                                          \
	op(nvme_cmd_flush, 0x00) op(nvme_cmd_write, 0x01) op(nvme_cmd_read, 0x02) op(             \
		nvme_cmd_write_uncor, 0x04) op(nvme_cmd_compare, 0x05) op(nvme_cmd_write_zeroes,  \
									  0x08)                   \
		op(nvme_cmd_dsm, 0x09) op(nvme_cmd_verify, 0x0c) op(nvme_cmd_resv_register, 0x0d) \
			op(nvme_cmd_resv_report, 0x0e) op(nvme_cmd_resv_acquire, 0x11) op(        \
				nvme_cmd_resv_release, 0x15) op(nvme_cmd_zone_mgmt_send, 0x79)    \
				op(nvme_cmd_zone_mgmt_recv, 0x7a) op(nvme_cmd_zone_append, 0x7d)  \
					op(nvme_cmd_kv_store, 0x81) op(nvme_cmd_kv_append, 0x83)  \
						op(nvme_cmd_kv_retrieve,                          \
						   0x90) op(nvme_cmd_kv_delete,                   \
							    0xA1) op(nvme_cmd_kv_iter_req, 0xB1)  \
							op(nvme_cmd_kv_iter_read,                 \
							   0xB2) op(nvme_cmd_kv_exist, 0xB3)      \
								op(nvme_cmd_kv_batch, 0x85)       \
									op(nvme_cmd_filter, 0x66)

#define ENUM_NVME_OP(name, value) name = value,
#define STRING_NVME_OP(name, value) [name] = #name,

enum nvme_opcode { NVME_OPCODES(ENUM_NVME_OP) };

static const char *const __nvme_opcode_strings[] = { NVME_OPCODES(STRING_NVME_OP) };

#define nvme_opcode_string(opcode) \
	(__nvme_opcode_strings[opcode] ? __nvme_opcode_strings[opcode] : "unknown")

struct nvme_common_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__le32 cdw2[2];
	__le64 metadata;
	__le64 prp1;
	__le64 prp2;
	__le32 cdw10[6];
};

struct nvme_filter_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2;
	__le64 metadata;
	__le64 prp1;
	__le64 prp2;
	__le64 slba;
	__le16 length;
	__le16 filter_factor;
	__le32 filter_index;
	__le32 filter_op;
	__le32 filter_const;
};

struct nvme_rw_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2;
	__le64 metadata;
	__le64 prp1;
	__le64 prp2;
	__le64 slba;
	__le16 length;
	__le16 control;
	__le32 dsmgmt;
	__le32 reftag;
	__le16 apptag;
	__le16 appmask;
};

struct nvme_get_log_page_command {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2[2];
	__le64 prp1;
	__le64 prp2;
	__u8 lid;
	__u8 lsp; /* upper 4 bits reserved */
	__le16 numdl;
	__le16 numdu;
	__u16 rsvd11;
	union {
		struct {
			__le32 lpol;
			__le32 lpou;
		};
		__le64 lpo;
	};
	__u8 rsvd14[3];
	__u8 csi;
	__u32 rsvd15;
};

enum {
	NVME_RW_LR = 1 << 15,
	NVME_RW_FUA = 1 << 14,
	NVME_RW_DSM_FREQ_UNSPEC = 0,
	NVME_RW_DSM_FREQ_TYPICAL = 1,
	NVME_RW_DSM_FREQ_RARE = 2,
	NVME_RW_DSM_FREQ_READS = 3,
	NVME_RW_DSM_FREQ_WRITES = 4,
	NVME_RW_DSM_FREQ_RW = 5,
	NVME_RW_DSM_FREQ_ONCE = 6,
	NVME_RW_DSM_FREQ_PREFETCH = 7,
	NVME_RW_DSM_FREQ_TEMP = 8,
	NVME_RW_DSM_LATENCY_NONE = 0 << 4,
	NVME_RW_DSM_LATENCY_IDLE = 1 << 4,
	NVME_RW_DSM_LATENCY_NORM = 2 << 4,
	NVME_RW_DSM_LATENCY_LOW = 3 << 4,
	NVME_RW_DSM_SEQ_REQ = 1 << 6,
	NVME_RW_DSM_COMPRESSED = 1 << 7,
	NVME_RW_PRINFO_PRCHK_REF = 1 << 10,
	NVME_RW_PRINFO_PRCHK_APP = 1 << 11,
	NVME_RW_PRINFO_PRCHK_GUARD = 1 << 12,
	NVME_RW_PRINFO_PRACT = 1 << 13,
};

struct nvme_dsm_cmd {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2[2];
	__le64 prp1;
	__le64 prp2;
	__le32 nr;
	__le32 attributes;
	__u32 rsvd12[4];
};

enum {
	NVME_DSMGMT_IDR = 1 << 0,
	NVME_DSMGMT_IDW = 1 << 1,
	NVME_DSMGMT_AD = 1 << 2,
};

struct nvme_dsm_range {
	__le32 cattr;
	__le32 nlb;
	__le64 slba;
};

/* Admin commands */

enum nvme_admin_opcode {
	nvme_admin_delete_sq = 0x00,
	nvme_admin_create_sq = 0x01,
	nvme_admin_get_log_page = 0x02,
	nvme_admin_delete_cq = 0x04,
	nvme_admin_create_cq = 0x05,
	nvme_admin_identify = 0x06,
	nvme_admin_abort_cmd = 0x08,
	nvme_admin_set_features = 0x09,
	nvme_admin_get_features = 0x0a,
	nvme_admin_async_event = 0x0c,
	nvme_admin_ns_mgmt = 0x0d,
	nvme_admin_activate_fw = 0x10,
	nvme_admin_download_fw = 0x11,
	nvme_admin_dev_self_test = 0x14,
	nvme_admin_ns_attach = 0x15,
	nvme_admin_keep_alive = 0x18,
	nvme_admin_directive_send = 0x19,
	nvme_admin_directive_recv = 0x1a,
	nvme_admin_virtual_mgmt = 0x1c,
	nvme_admin_nvme_mi_send = 0x1d,
	nvme_admin_nvme_mi_recv = 0x1e,
	nvme_admin_dbbuf = 0x7C,
	nvme_admin_format_nvm = 0x80,
	nvme_admin_security_send = 0x81,
	nvme_admin_security_recv = 0x82,
	nvme_admin_sanitize_nvm = 0x84,
	nvme_admin_get_lba_status = 0x86,
	nvme_admin_vendor_start = 0xC0,
};

enum {
	NVME_QUEUE_PHYS_CONTIG = (1 << 0),
	NVME_CQ_IRQ_ENABLED = (1 << 1),
	NVME_SQ_PRIO_URGENT = (0 << 1),
	NVME_SQ_PRIO_HIGH = (1 << 1),
	NVME_SQ_PRIO_MEDIUM = (2 << 1),
	NVME_SQ_PRIO_LOW = (3 << 1),
	NVME_FEAT_ARBITRATION = 0x01,
	NVME_FEAT_POWER_MGMT = 0x02,
	NVME_FEAT_LBA_RANGE = 0x03,
	NVME_FEAT_TEMP_THRESH = 0x04,
	NVME_FEAT_ERR_RECOVERY = 0x05,
	NVME_FEAT_VOLATILE_WC = 0x06,
	NVME_FEAT_NUM_QUEUES = 0x07,
	NVME_FEAT_IRQ_COALESCE = 0x08,
	NVME_FEAT_IRQ_CONFIG = 0x09,
	NVME_FEAT_WRITE_ATOMIC = 0x0a,
	NVME_FEAT_ASYNC_EVENT = 0x0b,
	NVME_FEAT_AUTO_PST = 0x0c,
	NVME_FEAT_SW_PROGRESS = 0x80,
	NVME_FEAT_HOST_ID = 0x81,
	NVME_FEAT_RESV_MASK = 0x82,
	NVME_FEAT_RESV_PERSIST = 0x83,
	NVME_LOG_ERROR = 0x01,
	NVME_LOG_SMART = 0x02,
	NVME_LOG_FW_SLOT = 0x03,
	NVME_LOG_CHANGED_NS = 0x04,
	NVME_LOG_CMD_EFFECTS = 0x05,
	NVME_LOG_DEVICE_SELF_TEST = 0x06,
	NVME_LOG_TELEMETRY_HOST = 0x07,
	NVME_LOG_TELEMETRY_CTRL = 0x08,
	NVME_LOG_ENDURANCE_GROUP = 0x09,
	NVME_LOG_ANA = 0x0c,
	NVME_LOG_DISC = 0x70,
	NVME_LOG_RESERVATION = 0x80,
	NVME_FWACT_REPL = (0 << 3),
	NVME_FWACT_REPL_ACTV = (1 << 3),
	NVME_FWACT_ACTV = (2 << 3),
};

struct nvme_identify {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2[2];
	__le64 prp1;
	__le64 prp2;
	__le32 cns;
	__u32 rsvd11[5];
};

struct nvme_features {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2[2];
	__le64 prp1;
	__le64 prp2;
	__le32 fid;
	__le32 dword11;
	__u32 rsvd12[4];
};

struct nvme_create_cq {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__u32 rsvd1[5];
	__le64 prp1;
	__u64 rsvd8;
	__le16 cqid;
	__le16 qsize;
	__le16 cq_flags;
	__le16 irq_vector;
	__u32 rsvd12[4];
};

struct nvme_create_sq {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__u32 rsvd1[5];
	__le64 prp1;
	__u64 rsvd8;
	__le16 sqid;
	__le16 qsize;
	__le16 sq_flags;
	__le16 cqid;
	__u32 rsvd12[4];
};

struct nvme_delete_queue {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__u32 rsvd1[9];
	__le16 qid;
	__u16 rsvd10;
	__u32 rsvd11[5];
};

struct nvme_abort_cmd {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__u32 rsvd1[9];
	__le16 sqid;
	__u16 cid;
	__u32 rsvd11[5];
};

struct nvme_download_firmware {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__u32 rsvd1[5];
	__le64 prp1;
	__le64 prp2;
	__le32 numd;
	__le32 offset;
	__u32 rsvd12[4];
};

struct nvme_format_cmd {
	__u8 opcode;
	__u8 flags;
	__u16 command_id;
	__le32 nsid;
	__u64 rsvd2[4];
	__le32 cdw10;
	__u32 rsvd11[5];
};

struct nvme_command {
	union {
		struct nvme_common_command common;
		struct nvme_rw_command rw;
		struct nvme_filter_command filter;
		struct nvme_get_log_page_command get_log_page;
		struct nvme_identify identify;
		struct nvme_features features;
		struct nvme_create_cq create_cq;
		struct nvme_create_sq create_sq;
		struct nvme_delete_queue delete_queue;
		struct nvme_download_firmware dlfw;
		struct nvme_format_cmd format;
		struct nvme_dsm_cmd dsm;
		struct nvme_abort_cmd abort;
	};
};

enum {
	NVME_SCT_GENERIC_CMD_STATUS = 0,
	NVME_SCT_CMD_SPECIFIC_STATUS,
	NVME_SCT_MEDIA_INTEGRITY_ERRORS,
	NVME_SCT_PATH_RELATED_STATUS
};

enum {
	NVME_SC_SUCCESS = 0x0,
	NVME_SC_INVALID_OPCODE = 0x1,
	NVME_SC_INVALID_FIELD = 0x2,
	NVME_SC_CMDID_CONFLICT = 0x3,
	NVME_SC_DATA_XFER_ERROR = 0x4,
	NVME_SC_POWER_LOSS = 0x5,
	NVME_SC_INTERNAL = 0x6,
	NVME_SC_ABORT_REQ = 0x7,
	NVME_SC_ABORT_QUEUE = 0x8,
	NVME_SC_FUSED_FAIL = 0x9,
	NVME_SC_FUSED_MISSING = 0xa,
	NVME_SC_INVALID_NS = 0xb,
	NVME_SC_CMD_SEQ_ERROR = 0xc,
	NVME_SC_SGL_INVALID_LAST = 0xd,
	NVME_SC_SGL_INVALID_COUNT = 0xe,
	NVME_SC_SGL_INVALID_DATA = 0xf,
	NVME_SC_SGL_INVALID_METADATA = 0x10,
	NVME_SC_SGL_INVALID_TYPE = 0x11,
	NVME_SC_LBA_RANGE = 0x80,
	NVME_SC_CAP_EXCEEDED = 0x81,
	NVME_SC_NS_NOT_READY = 0x82,
	NVME_SC_RESERVATION_CONFLICT = 0x83,
	NVME_SC_CQ_INVALID = 0x100,
	NVME_SC_QID_INVALID = 0x101,
	NVME_SC_QUEUE_SIZE = 0x102,
	NVME_SC_ABORT_LIMIT = 0x103,
	NVME_SC_ABORT_MISSING = 0x104,
	NVME_SC_ASYNC_LIMIT = 0x105,
	NVME_SC_FIRMWARE_SLOT = 0x106,
	NVME_SC_FIRMWARE_IMAGE = 0x107,
	NVME_SC_INVALID_VECTOR = 0x108,
	NVME_SC_INVALID_LOG_PAGE = 0x109,
	NVME_SC_INVALID_FORMAT = 0x10a,
	NVME_SC_FIRMWARE_NEEDS_RESET = 0x10b,
	NVME_SC_INVALID_QUEUE = 0x10c,
	NVME_SC_FEATURE_NOT_SAVEABLE = 0x10d,
	NVME_SC_FEATURE_NOT_CHANGEABLE = 0x10e,
	NVME_SC_FEATURE_NOT_PER_NS = 0x10f,
	NVME_SC_FW_NEEDS_RESET_SUBSYS = 0x110,
	NVME_SC_BAD_ATTRIBUTES = 0x180,
	NVME_SC_INVALID_PI = 0x181,
	NVME_SC_READ_ONLY = 0x182,
	NVME_SC_WRITE_FAULT = 0x280,
	NVME_SC_READ_ERROR = 0x281,
	NVME_SC_GUARD_CHECK = 0x282,
	NVME_SC_APPTAG_CHECK = 0x283,
	NVME_SC_REFTAG_CHECK = 0x284,
	NVME_SC_COMPARE_FAILED = 0x285,
	NVME_SC_ACCESS_DENIED = 0x286,
	NVME_SC_DNR = 0x4000,
};

struct nvme_completion {
	__le32 result0; /* Used by admin commands to return data */
	__le32 result1;
	__le16 sq_head; /* how much of this queue may be reclaimed */
	__le16 sq_id; /* submission queue that generated this entry */
	__u16 command_id; /* of the command which completed */
	__le16 status; /* did the command fail, and if so, why? */
};

enum {
	NVME_NIDT_EUI = 0x1, /*IEEE Extended Unique Identifier*/
	NVME_NIDT_GUI = 0x2, /*Namespace Globally Unique Identifier*/
	NVME_NIDT_UUID = 0x3, /*Namespace UUID*/
	NVME_NIDT_CSI = 0x4, /*Command Set Identifier*/
};

enum {
	NVME_CSI_NVM = 0x0, /*NVM Command Set*/
	NVME_CSI_KV = 0x1, /*Key Value Command Set*/
	NVME_CSI_ZNS = 0x2, /*Zoned Namespace Command Set*/
};
#define NVME_VS(major, minor) (((major) << 16) | ((minor) << 8))

#endif /* _LINUX_NVME_H */
