// SPDX-License-Identifier: GPL-2.0-only

#ifndef _LIB_NVMEV_HDR_H
#define _LIB_NVMEV_HDR_H

struct __nvme_bar {
	uint64_t cap; /* Controller Capabilities */
	uint32_t vs; /* Version */
	uint32_t intms; /* Interrupt Mask Set */
	uint32_t intmc; /* Interrupt Mask Clear */
	uint32_t cc; /* Controller Configuration */
	uint32_t rsvd1; /* Reserved */
	uint32_t csts; /* Controller Status */
	uint32_t nssr; /* Subsystem Reset */
	uint32_t aqa; /* Admin Queue Attributes */
	uint64_t asq; /* Admin SQ Base Address */
	uint64_t acq; /* Admin CQ Base Address */
	uint32_t cmbloc; /* Controller Memory Buffer Location */
	uint32_t cmbsz; /* Controller Memory Buffer Size */
};

struct pci_header {
	struct {
		// vendor id
		u16 vid;
		// device id
		u16 did;
	} id;
	// Command Register
	struct {
		// I/O Space Enable
		u8 iose : 1;
		// Memory Space Enable
		u8 mse : 1;
		// Bus Master Enable
		u8 bme : 1;
		// Special Cycle Enable
		u8 sce : 1;
		// Memory Write and Invalidate Enable
		u8 mwie : 1;
		// VGA palette snoop
		u8 vga : 1;
		// Parity Error response Enable
		u8 pee : 1;
		u8 fixed : 1;
		// SERR# Enable
		u8 see : 1;
		// Fast Back-to-Back Enable
		u8 fbe : 1;
		// Interrupt Disable
		u8 id : 1;
		u8 rsvd : 5;
	} cmd;
	// Status Register
	struct {
		u8 rsvd1 : 3;
		// Interrupt Status, Read Only
		u8 is : 1;
		// Capabilities List, Read Only
		u8 cl : 1;
		// 66MHz Capability, Read Only
		u8 c66 : 1;
		u8 rsvd2 : 1;
		// Fast Back-to-Back Capable, Read Only
		u8 fbc : 1;
		// master Data Parity Error
		u8 dpd : 1;
		// DEVSEL timing
		u8 devt : 2;
		// Signaled Target Abort
		u8 sta : 1;
		// Received Target Abort
		u8 rta : 1;
		// Received Master Abort
		u8 rma : 1;
		// Signaled System Error
		u8 sse : 1;
		// Detected Parity Error
		u8 dpe : 1;
	} sts;
	// Revision ID
	u8 rid;
	// Class Code
	struct {
		// Interface
		u8 pi;
		// Sub Class Code
		u8 scc;
		// Base Class Code
		u8 bcc;
	} cc;
	// Cache Line Size, not used in pcie
	u8 cls;
	// Latancy Time, not used in pcie
	u8 mlt;
	// Header Type
	struct {
		// 1 means multi-function device, 0 means single-function device
		u8 hl : 7;
		// Type0 has 6 BAR, which usually represent common devices
		// Type1 has 2 BAR, which usually represent pci bridge
		// Type2 has 6 BAR, which usually represent cardbus
		u8 mfd : 1;
	} htype;
	//Build In Self Test
	struct {
		u8 cc : 4;
		u8 rsvd : 2;
		u8 sb : 1;
		u8 bc : 1;
	} bist;

	struct {
		// Resource Type Encoding(0 means Memory Space, 1 means I/O Space)
		u32 rte : 1;
		// Type(00 means 32bit memory space, 10 means 64bit memory space)
		u32 tp : 2;
		// PreFetchable
		u32 pf : 1;
		u32 rsvd : 10;
		// Base Address, high 18bit of storage base address, need to left shift 14bit
		u32 ba : 18;
	} mlbar;

	u32 mulbar;
	u32 idbar;

	u32 bar3;
	u32 bar4;
	u32 bar5;

	// Cardbus CIS Pointer
	u32 ccptr;

	struct {
		// SubSystem Vendor ID
		u16 ssvid;
		// SubSystem ID
		u16 ssid;
	} ss;
	//Expansion ROM base address
	u32 erom;
	// Capabilities pointer, must be implement by pcie devices
	u8 cap;
	u8 rsvd[7];
	struct {
		//Interrupt Line
		u8 iline;
		//Interrupt Pin
		u8 ipin;
	} intr;

	// Min Grant Time, the unit is PCI clock cycle
	u8 mgnt;
	// Max latency, the unit is PCI clock cycle
	u8 mlat;
};

struct pci_pm_cap {
	struct {
		// Power Management Capability ID, 0x01
		u8 cid;
		// next Pointer, point to the next capability
		u8 next;
	} pid;
	// PMCR
	struct {
		// VerSion
		u16 vs : 3;
		// PME Clock
		u16 pmec : 1;
		// Reserved
		u16 resv : 1;
		// Device Specific Initialization
		u16 dsi : 1;
		// Auxiliary device Current
		u16 auxc : 3;
		// D1 Support
		u16 d1s : 1;
		// D2 Support
		u16 d2s : 1;
		// PME Support
		u16 psup : 5;
	} pc;
	// PMCSR
	struct {
		// Power State
		u16 ps : 2;
		// Reserved
		u16 rsvd01 : 1;
		// No Soft Reset
		u16 nsfrst : 1;
		// Reserved
		u16 rsvd02 : 4;
		// PME Enable
		u16 pmee : 1;
		// Data Select
		u16 dse : 4;
		// Data Scale
		u16 dsc : 2;
		// PME Status
		u16 pmes : 1;
	} pmcs;
	u8 ext[2];
};

struct pci_msi_cap {
	struct {
	} mid;
	struct {
	} mc;
	struct {
	} ma;
	struct {
	} mua;
	struct {
	} md;
	struct {
	} mmask;
	struct {
	} mpend;
};

struct pci_msix_cap {
	struct {
		u8 cid;
		// next Pointer, point to the next capability
		u8 next;
	} mxid;
	struct {
		u16 ts : 11;
		u16 rsvd : 3;
		u16 fm : 1;
		u16 mxe : 1;
	} mxc;
	struct {
		u32 tbir : 3;
		u32 to : 29;
	} mtab;
	struct {
		u32 pbir : 3;
		u32 pbao : 29;
	} mpba;
};

struct pcie_cap {
	struct {
		// PCI Express Capability ID, 0x10
		u8 cid;
		// next Pointer, point to the next capability
		u8 next;
	} pxid;
	// PCI Express Capability
	struct {
		// Capability Version
		u8 ver : 4;
		// Device/Port Type
		u8 dpt : 4;
		// Slot Implement
		u8 si : 1;
		// Interrupt Message Nuber
		u8 imn : 5;
		// Reserved
		u8 rsvd : 2;
	} pxcap;
	// PCI Express Device Capability
	struct {
		// Max Payload Size Supported
		u32 mps : 3;
		// Phantom Functions Supported
		u32 pfs : 2;
		//Extended Tag Field Supported
		u32 etfs : 1;
		// 
		u32 l0sl : 3;
		u32 l1l : 3;
		u32 rsvd : 3;
		u32 rer : 1;
		u32 rsvd2 : 2;
		u32 csplv : 8;
		u32 cspls : 2;
		u32 flrc : 1;
		u32 rsvd3 : 3;
	} pxdcap;
	// PCI Express Device Control
	struct {
		// Correctable Error Reporting Enable
		u16 cere : 1;
		// Non-Fatal Error Reporting Enable
		u16 nfere : 1;
		// Fatal Error Reporting Enable
		u16 fere : 2;
		// UriHuppcrted Request Reporting Enable
		u16 urre : 1;
		// Enable Relaxed Ordering
		u16 ero : 1;
		// Max Payload Size
		u16 mps : 3;
		// Extended Tag Field Enable
		u16 ete : 1;
		// Phantom Functions Enable
		u16 pfe : 1;
		// Auxiliary Power PM Enable
		u16 appme : 1;
		// Enable No Snoop
		u16 ens : 1;
		// Max Read Request Size
		u16 mrrs : 3;
		u16 iflr : 1;
	} pxdc;
	// PCI Express Device Status
	struct {
		// Correclahle Error Detected
		u16 ced : 1;
		// Non-Fatal Error Deteckd
		u16 nfed : 1;
		// Fatal Error Detected
		u16 fed : 1;
		// Unsupported Request Detected
		u16 urd : 1;
		// AUX Power Detected
		u16 apd : 1;
		// Transactions Pending
		u16 tp : 1;
		// Reserved
		u16 rsvd : 10;
	} pxds;
	// PCI Express Link Capability
	struct {
		// Supported Link Speeds(1, 2, 4 represent 2.5, 5, 8GT/s respectively)
		u32 sls : 4;
		// Maximum Link Width, support x1, x2, x4, x8, x12, x16, x32
		u32 mlw : 6;
		// Active State Power Management Support
		u32 aspms : 2;
		// L0s Exit Latency
		u32 l0sel : 3;
		// L1s Exit Latency
		u32 l1el : 3;
		u32 cpm : 1;
		u32 sderc : 1;
		u32 dllla : 20;
		u32 lbnc : 1;
		u32 aoc : 1;
		u32 rsvd : 1;
		// port number
		u32 pn : 8;
	} pxlcap;
	// PCI Express Link Control
	struct {
		// Active State Power Management Control
		u16 aspmc : 2;
		// Reserved
		u16 rsvd : 1;
		// Read Completion Boundary
		u16 rcb : 1;
		// Reserved
		u16 rsvd2 : 2;
		// Common Clock Configuration
		u16 ccc : 1;
		// Extended Sync
		u16 es : 1;
		u16 ecpm : 1;
		// Hardware Autonomous Width Disable
		u16 hawd : 1;
		// Reserved
		u16 rsvd3 : 6;
	} pxlc;
	// PCI Express Link Status
	struct {
		// Current Link Speeds
		u16 clc : 4;
		// Negolialed Link Width
		u16 nlw : 6;
		// LReserved
		u16 rsvd : 2;
		// Slot Clock Configuration
		u16 scc : 1;
		// Reserved
		u16 rsvd2 : 3;
	} pxls;
	// PCI Express Device Capability 2
	struct {
		u32 ctrs : 4;
		u32 ctds : 1;
		u32 arifs : 1;
		// AlomicOp Routing Supported
		u32 aors : 1;
		// 32-bit AtomicOp Completer Supported
		u32 aocs32 : 1;
		// 64-bit AtomicOp Completer Supported
		u32 aocs : 1;
		// 128-bit AtomicOp Completer Supported
		u32 ccs128 : 1;
		u32 nprpr : 1;
		u32 ltrs : 1;
		// TPH Completer Supported
		u32 tphcs : 2;
		// Reserved
		u32 rsvd : 4;
		u32 obffs : 2;
		// Extended Fmt Field Supported
		u32 effs : 1;
		// End-End TLP Prefix Supported
		u32 eetps : 1;
		// Max End-End TLP Prefixes
		u32 meetp : 2;
		// Reserved
		u32 rsvd2 : 8;
	} pxdcap2;
	// PCI Express Device Control 2
	struct {
		u32 ctv : 4;
		u32 ctd : 1;
		u32 rsvd : 5;
		u32 ltrme : 1;
		u32 rsvd2 : 2;
		u32 obffe : 2;
		u32 rsvd3 : 17;
	} pxdc2;
};

// PCI Express Extended Capabilities
struct pci_ext_cap {
	// Capability ID, if there is no extended Capabilities, ID should be 0xFFFF
	u16 cid;
	// Capbility Version, , if there is no extended Capabilities, version should be 0x00
	u16 cver : 4;
	// Next Capbility offset, if there is no extended Capabilities, version should be 0x000
	u16 next : 12;
};

// PCI Express Extended Capabilities: AER(All Error) Capability
struct pci_ext_cap_aer {
	struct pci_ext_cap id;
	struct {
		u32 rsvd : 4;
		u32 dlpes : 1;
		u32 rsvd2 : 7;
		u32 pts : 1;
		u32 fcpes : 1;
		u32 cts : 1;
		u32 cas : 1;
		u32 ucs : 1;
		u32 ros : 1;
		u32 mts : 1;
		u32 ecrces : 1;
		u32 ures : 1;
		u32 acsvs : 1;
		u32 uies : 1;
		u32 mcbts : 1;
		u32 aoebs : 1;
		u32 tpbes : 1;
		u32 rsvd3 : 6;
	} aeruces;
	struct {
		u32 rsvd : 4;
		u32 dlpem : 1;
		u32 rsvd2 : 7;
		u32 ptm : 1;
		u32 fcpem : 1;
		u32 ctm : 1;
		u32 cam : 1;
		u32 ucm : 1;
		u32 rom : 1;
		u32 mtm : 1;
		u32 ecrcem : 1;
		u32 urem : 1;
		u32 acsvm : 1;
		u32 uiem : 1;
		u32 mcbtm : 1;
		u32 aoebm : 1;
		u32 tpbem : 1;
		u32 rsvd3 : 6;
	} aerucem;
	struct {
		u32 rsvd : 4;
		u32 dlpesev : 1;
		u32 rsvd2 : 7;
		u32 ptsev : 1;
		u32 fcpesev : 1;
		u32 ctsev : 1;
		u32 casev : 1;
		u32 ucsev : 1;
		u32 rosev : 1;
		u32 mtsev : 1;
		u32 ecrcesev : 1;
		u32 uresev : 1;
		u32 acsvsev : 1;
		u32 uiesev : 1;
		u32 mcbtsev : 1;
		u32 aoebsev : 1;
		u32 tpbesev : 1;
		u32 rsvd3 : 6;
	} aerucesev;
	struct {
		u32 res : 1;
		u32 rsvd : 5;
		u32 bts : 1;
		u32 bds : 1;
		u32 rrs : 1;
		u32 rsvd2 : 3;
		u32 rts : 1;
		u32 anfes : 1;
		u32 cies : 1;
		u32 hlos : 1;
		u32 rsvd3 : 16;
	} aerces;
	struct {
		u32 rem : 1;
		u32 rsvd : 5;
		u32 btm : 1;
		u32 bdm : 1;
		u32 rrm : 1;
		u32 rsvd2 : 3;
		u32 rtm : 1;
		u32 anfem : 1;
		u32 ciem : 1;
		u32 hlom : 1;
		u32 rsvd3 : 16;
	} aercem;
	struct {
		u32 fep : 5;
		u32 egc : 1;
		u32 ege : 1;
		u32 ecc : 1;
		u32 ece : 1;
		u32 mhrc : 1;
		u32 mhre : 1;
		u32 tplp : 1;
		u32 rsvd : 20;
	} aercc;
	struct {
		u8 hb3;
		u8 hb2;
		u8 hb1;
		u8 hb0;
		u8 hb7;
		u8 hb6;
		u8 hb5;
		u8 hb4;
		u8 hb11;
		u8 hb10;
		u8 hb9;
		u8 hb8;
		u8 hb15;
		u8 hb14;
		u8 hb13;
		u8 hb12;
	} aerhl;
	struct {
		u8 tpl1b3;
		u8 tpl1b2;
		u8 tpl1b1;
		u8 tpl1b0;
		u8 tpl2b3;
		u8 tpl2b2;
		u8 tpl2b1;
		u8 tpl2b0;
		u8 tpl3b3;
		u8 tpl3b2;
		u8 tpl3b1;
		u8 tpl3b0;
		u8 tpl4b3;
		u8 tpl4b2;
		u8 tpl4b1;
		u8 tpl4b0;
	} aertlp;
};

// PCI Express Extended Capabilities: Device Serial Number Capabilit
struct pci_ext_cap_dsn {
	struct pci_ext_cap id;
	u64 serial;
};

struct nvme_ctrl_regs {
	union {
		struct {
			u16 mqes;
			u16 cqr : 1;
			u16 ams : 2;
			u16 rsvd : 5;
			u16 to : 8;
			u16 dstrd : 4;
			u16 nssrs : 1;
			u16 css : 8;
			u16 rsvd2 : 3;
			u16 mpsmin : 4;
			u16 mpsmax : 4;
			u16 rsvd3 : 8;
		} cap;
		u64 u_cap;
	};
	//uint64_t			cap;	/* Controller Capabilities */
	union {
		struct {
			u8 rsvd;
			u8 mnr;
			u16 mjr;
		} vs;
		u32 u_vs;
	};
	//uint32_t			vs;	/* Version */
	u32 intms; /* Interrupt Mask Set */
	u32 intmc; /* Interrupt Mask Clear */
	union {
		struct {
			u16 en : 1;
			u16 rsvd : 3;
			u16 css : 3;
			u16 mps : 4;
			u16 ams : 3;
			u16 shn : 2;
			u16 iosqes : 4;
			u16 iocqes : 4;
			u16 rsvd2 : 8;
		} cc;
		u32 u_cc;
	};
	//uint32_t			cc;	/* Controller Configuration */
	u32 rsvd1; /* Reserved */
	union {
		struct {
			u32 rdy : 1;
			u32 cfs : 1;
			u32 shst : 2;
			u32 nssro : 1;
			u32 pp : 1;
			u32 rsvd : 26;
		} csts;
		u32 u_csts;
	};
	//uint32_t			csts;	/* Controller Status */
	u32 nssr; /* Subsystem Reset */
	union {
		struct {
			u32 asqs : 12;
			u32 rsvd1 : 4;
			u32 acqs : 12;
			u32 rsvd2 : 4;
		} aqa;
		u32 u_aqa;
	};
	//uint32_t			aqa;	/* Admin Queue Attributes */
	union {
		struct {
			u64 rsvd : 12;
			u64 asqb : 52;
		} asq;
		u64 u_asq;
	};
	//uint64_t			asq;	/* Admin SQ Base Address */
	union {
		struct {
			u64 rsvd : 12;
			u64 acqb : 52;
		} acq;
		u64 u_acq;
	};
	//uint64_t			acq;	/* Admin CQ Base Address */
	union {
		struct {
			u32 bir : 3;
			u32 rsvd : 9;
			u32 ofst : 20;
		} cmbloc;
		u32 u_cmbloc;
	};
	//uint32_t			cmbloc; /* Controller Memory Buffer Location */
	union {
		struct {
			u32 sqs : 1;
			u32 cqs : 1;
			u32 lists : 1;
			u32 rds : 1;
			u32 wds : 1;
			u32 rsvd : 3;
			u32 szu : 4;
			u32 sz : 20;
		} cmbsz;
		u32 u_cmbsz;
	};
	//uint32_t			cmbsz;  /* Controller Memory Buffer Size */
};


#define NVMEV_PCI_DOMAIN_NUM 0x0001
#define NVMEV_PCI_BUS_NUM 0x10

//[PCI_HEADER][PM_CAP][MSIX_CAP][PCIE_CAP] | [AER_CAP][EXT_CAP]
#define SZ_PCI_HDR sizeof(struct pci_header) // 0
#define SZ_PCI_PM_CAP sizeof(struct pci_pm_cap) // 0x40
#define SZ_PCI_MSIX_CAP sizeof(struct pci_msix_cap) // 0x50
#define SZ_PCIE_CAP sizeof(struct pcie_cap) // 0x60

#define OFFS_PCI_HDR 0x0
#define OFFS_PCI_PM_CAP 0x40
#define OFFS_PCI_MSIX_CAP 0x50
#define OFFS_PCIE_CAP 0x60

#define SZ_HEADER (OFFS_PCIE_CAP + SZ_PCIE_CAP)

//#define PCI_CFG_SPACE_SIZE 0x100
#define PCI_EXT_CAP_START 0x50

#define OFFS_PCI_EXT_CAP (PCI_CFG_SPACE_SIZE)


enum {
	CAP_CSS_BIT_NVM = (1 << 0),
	CAP_CSS_BIT_SPECIFIC = (1 << 6),
	CAP_CSS_BIT_NOT_SUPPORTED = (1 << 7),
};

#endif /* _LIB_NVMEV_HDR_H */
