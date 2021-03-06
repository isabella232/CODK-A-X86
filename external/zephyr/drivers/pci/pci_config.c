/* pci_config.c - PCI bus support */

/*
 * Copyright (c) 2009-2010, 2013-2014 Wind River Systems, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1) Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3) Neither the name of Wind River Systems nor the names of its contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
DESCRIPTION

This module implements the PCI config space access functions

 */

#include <nanokernel.h>
#include <arch/cpu.h>

#include <pci/pci_mgr.h>
#include <string.h>

#ifdef __MSIX_DEBUG__
#include <misc/printk.h>
#define _DEBUG_PRINT(fmt, args...) printk(fmt, ##args)
#else
#define _DEBUG_PRINT(fmt, args...) \
	do {                       \
	} while (0)
#endif

/**
 *
 * @brief Write a 32bit data to pci reg in offset
 *
 * @param bus_no      Bus number.
 * @param device_no   Device number
 * @param func_no     Function number
 * @param offset      Offset into the configuration space.
 * @param data        Data written to the offset.
 *
 * @return N/A
 */
void pci_config_out_long(uint32_t bus_no, uint32_t device_no, uint32_t func_no,
						 uint32_t offset, uint32_t data)
{
	union pci_addr_reg pci_addr;

	/* create the PCI address we're going to access */

	pci_addr.field.bus = bus_no;
	pci_addr.field.device = device_no;
	pci_addr.field.func = func_no;
	pci_addr.field.reg = offset / 4;
	pci_addr.field.offset = 0;

	/* write to the PCI controller */

	pci_write(DEFAULT_PCI_CONTROLLER, pci_addr, sizeof(uint32_t), data);
}

/**
 *
 * @brief Write a 16bit data to pci reg in offset
 *
 * @param bus_no      Bus number.
 * @param device_no   Device number.
 * @param func_no     Function number.
 * @param offset      Offset into the configuration space.
 * @param data        Data written to the offset.
 *
 * @return N/A
 */
void pci_config_out_word(uint32_t bus_no, uint32_t device_no, uint32_t func_no,
						 uint32_t offset, uint16_t data)
{
	union pci_addr_reg pci_addr;

	/* create the PCI address we're going to access */

	pci_addr.field.bus = bus_no;
	pci_addr.field.device = device_no;
	pci_addr.field.func = func_no;
	pci_addr.field.reg = offset / 4;
	pci_addr.field.offset = offset & 2;

	/* write to the PCI controller */

	pci_write(DEFAULT_PCI_CONTROLLER, pci_addr, sizeof(uint16_t), data);
}

/**
 *
 * @brief Write a 8bit data to pci reg in offset
 *
 * @param bus_no      Bus number.
 * @param device_no   Device number.
 * @param func_no     Function number.
 * @param offset      Offset into the configuration space.
 * @param data        Data written to the offset.
 *
 * @return N/A
 */
void pci_config_out_byte(uint32_t bus_no, uint32_t device_no, uint32_t func_no,
						 uint32_t offset, uint8_t data)
{
	union pci_addr_reg pci_addr;

	/* create the PCI address we're going to access */

	pci_addr.field.bus = bus_no;
	pci_addr.field.device = device_no;
	pci_addr.field.func = func_no;
	pci_addr.field.reg = offset / 4;
	pci_addr.field.offset = offset % 4;

	/* write to the PCI controller */

	pci_write(DEFAULT_PCI_CONTROLLER, pci_addr, sizeof(uint8_t), data);
}

/**
 *
 * @brief Read a 32bit data from pci reg in offset
 *
 * @param bus_no      Bus number.
 * @param device_no   Device number.
 * @param func_no     Function number.
 * @param offset      Offset into the configuration space.
 * @param data        Data read from the offset.
 *
 * @return N/A
 *
 */
void pci_config_in_long(uint32_t bus_no, uint32_t device_no, uint32_t func_no,
						uint32_t offset, uint32_t *data)
{
	union pci_addr_reg pci_addr;

	/* create the PCI address we're going to access */

	pci_addr.field.bus = bus_no;
	pci_addr.field.device = device_no;
	pci_addr.field.func = func_no;
	pci_addr.field.reg = offset / 4;
	pci_addr.field.offset = 0;

	/* read from the PCI controller */

	pci_read(DEFAULT_PCI_CONTROLLER, pci_addr, sizeof(uint32_t), data);
}

/**
 *
 * @brief Read in a 16bit data from a pci reg in offset
 *
 * @param bus_no      Bus number.
 * @param device_no   Device number.
 * @param func_no     Function number.
 * @param offset      Offset into the configuration space.
 * @param data        Data read from the offset.
 *
 * @return N/A
 *
 */

void pci_config_in_word(uint32_t bus_no, uint32_t device_no, uint32_t func_no,
						uint32_t offset, uint16_t *data)
{
	union pci_addr_reg pci_addr;
	uint32_t pci_data;

	/* create the PCI address we're going to access */

	pci_addr.field.bus = bus_no;
	pci_addr.field.device = device_no;
	pci_addr.field.func = func_no;
	pci_addr.field.reg = offset / 4;
	pci_addr.field.offset = offset & 2;

	/* read from the PCI controller */

	pci_read(DEFAULT_PCI_CONTROLLER, pci_addr, sizeof(uint16_t), &pci_data);

	/* return the data */

	*data = (uint16_t)pci_data;
}

/**
 *
 * @brief Read in a 8bit data from a pci reg in offset
 *
 * @param bus_no      Bus number.
 * @param device_no   Device number.
 * @param func_no     Function number.
 * @param offset      Offset into the configuration space.
 * @param data        Data read from the offset.
 *
 * @return N/A
 *
 */

void pci_config_in_byte(uint32_t bus_no, uint32_t device_no, uint32_t func_no,
						uint32_t offset, uint8_t *data)
{
	union pci_addr_reg pci_addr;
	uint32_t pci_data;

	/* create the PCI address we're going to access */

	pci_addr.field.bus = bus_no;
	pci_addr.field.device = device_no;
	pci_addr.field.func = func_no;
	pci_addr.field.reg = offset / 4;
	pci_addr.field.offset = offset % 4;

	/* read from the PCI controller */

	pci_read(DEFAULT_PCI_CONTROLLER, pci_addr, sizeof(uint8_t), &pci_data);

	/* return the data */

	*data = (uint8_t)pci_data;
}

/**
 *
 * @brief Find extended capability in ECP linked list
 *
 * This routine searches for an extended capability in the linked list of
 * capabilities in config space. If found, the offset of the first byte
 * of the capability of interest in config space is returned via pOffset.
 *
 * @param ext_cap_find_id   Extended capabilities ID to search for.
 * @param bus               PCI bus number.
 * @param device            PCI device number.
 * @param function          PCI function number.
 * @param p_offset          Returned config space offset.
 *
 * @return 0 if Extended Capability found, -1 otherwise
 *
 */

int pci_config_ext_cap_ptr_find(uint8_t ext_cap_find_id, uint32_t bus,
								uint32_t device, uint32_t function,
								uint8_t *p_offset)
{
	uint16_t tmp_stat;
	uint8_t tmp_offset;
	uint8_t cap_offset = 0x00;
	uint8_t cap_id = 0x00;

	/* Check to see if the device has any extended capabilities */

	pci_config_in_word(bus, device, function, PCI_CFG_STATUS, &tmp_stat);

	if ((tmp_stat & PCI_STATUS_NEW_CAP) == 0) {
		return -1;
	}

	/* Get the initial ECP offset and make longword aligned */

	pci_config_in_byte(bus, device, function, PCI_CFG_CAP_PTR, &cap_offset);
	cap_offset &= ~0x02;

	/* Bounds check the ECP offset */

	if (cap_offset < 0x40) {
		return -1;
	}

	/* Look for the specified Extended Cap item in the linked list */

	while (cap_offset != 0x00) {

		/* Get the Capability ID and check */

		pci_config_in_byte(bus, device, function, (int)cap_offset, &cap_id);
		if (cap_id == ext_cap_find_id) {
			*p_offset = cap_offset;
			return 0;
		}

		/* Get the offset to the next New Capabilities item */

		tmp_offset = cap_offset + (uint8_t)0x01;
		pci_config_in_byte(bus, device, function, (int)tmp_offset, &cap_offset);
	}

	return -1;
}

