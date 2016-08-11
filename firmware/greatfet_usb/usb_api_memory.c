/*
 * Copyright 2016 Schuyler St. Leger
 *
 * This file is part of GreatFET.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include "usb_api_memory.h"
#include "usb_queue.h"

#include <stddef.h>
#include <greatfet_core.h>

/* Write to memory */
// TODO: Does not work
// board crashes when run and requires hard reset?
usb_request_status_t usb_vendor_request_write_memory(
	usb_endpoint_t* const endpoint, const usb_transfer_stage_t stage)
{
	uint32_t addr;

	if (stage == USB_TRANSFER_STAGE_SETUP) {
		addr = (endpoint->setup.value << 16) | endpoint->setup.index;

		usb_transfer_schedule_block(endpoint->out, addr,
									endpoint->setup.length, NULL, NULL);
	} else if (stage == USB_TRANSFER_STAGE_DATA) {
		usb_transfer_schedule_ack(endpoint->in);
	}
	return USB_REQUEST_STATUS_OK;
}

/* Read from memory */
// TODO: Test better, believed to work
usb_request_status_t usb_vendor_request_read_memory(
	usb_endpoint_t* const endpoint, const usb_transfer_stage_t stage)
{
	uint32_t addr;
	uint16_t len;

	if (stage == USB_TRANSFER_STAGE_SETUP) {
		len = endpoint->setup.length;
		addr = (endpoint->setup.value << 16) | endpoint->setup.index;

		usb_transfer_schedule_block(endpoint->in, addr, len, NULL, NULL);
		return USB_REQUEST_STATUS_OK;
	} else if (stage == USB_TRANSFER_STAGE_DATA) {
		usb_transfer_schedule_ack(endpoint->out);
		return USB_REQUEST_STATUS_OK;
	} else {
		return USB_REQUEST_STATUS_OK;
	}
}
