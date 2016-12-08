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

#include "usb_api_spi.h"
#include "usb_queue.h"
#include "greatfet_pins.h"
#include "sctimer_regs.h"

#include <stddef.h>
#include <greatfet_core.h>

#include <libopencm3/lpc43xx/rgu.h>


usb_request_status_t usb_vendor_request_pwm_start(
		usb_endpoint_t* const endpoint, const usb_transfer_stage_t stage) {
	if (stage == USB_TRANSFER_STAGE_SETUP) {

		RESET_CTRL1 = RESET_CTRL1_SCT_RST;

		// Delay to allow reset sigal to be processed
		// The reset generator uses a 12MHz clock (IRC)
		// The difference between this and the core clock (CCLK)
		// determines this value (CCLK/IRC).
		// The value used here is a bit shorter than would be required, since
		// there are additional commands that fill the time
		delay(8);

		scu_pinmux(P4_2, SCU_CONF_EPUN_DIS_PULLUP | SCU_CONF_EHS_FAST | SCU_CONF_FUNCTION1);

		SCT_REG(SCT_CONFIG) |= BIT0;
		SCT_REG16(SCT_CTRL_L) = BIT2;

		// Prescaler
		SCT_REG16(SCT_CTRL_L) |= 255 << 5;

		// Maximum counter value
		if (endpoint->setup.value) {
			SCT_REG(SCT_MATCH0_L) = endpoint->setup.value;
			SCT_REG(SCT_MATCHREL0_L) = endpoint->setup.value;
		} else {
			SCT_REG(SCT_MATCH0_L) = 16384;
			SCT_REG(SCT_MATCHREL0_L) = 16384;
		}

		// When to turn off output
		if (endpoint->setup.index) {
			SCT_REG(SCT_MATCH1_L) = endpoint->setup.index;
			SCT_REG(SCT_MATCHREL1_L) = endpoint->setup.index;
		} else {
			SCT_REG(SCT_MATCH1_L) = 1024;
			SCT_REG(SCT_MATCHREL1_L) = 1024;
		}

		// Event 0 triggered by MATCH0. Event 1 triggered by MATCH1.
		SCT_REG(SCT_EVCTRL0) = BIT12 | 0x0;
		SCT_REG(SCT_EVCTRL1) = BIT12 | 0x1;

		// Event 0 sets output. Event 1 clears the output.
		SCT_REG(SCT_OUTPUTSET0) = BIT0;
		SCT_REG(SCT_OUTPUTCL0) = BIT1;

		// SCT_REG(SCT_RES) = 0x3;

		// Events 0 and 1 can occur in state 0
		SCT_REG(SCT_EVSTATEMSK0) = BIT0;
		SCT_REG(SCT_EVSTATEMSK1) = BIT0;

		// Event 0 resets the counter to 0
		SCT_REG(SCT_LIMIT) = 0x01;

		// Begin starting
		SCT_REG(SCT_STATE) = 0;

		SCT_REG16(SCT_CTRL)  &= ~(BIT2 | BIT1);

		usb_transfer_schedule_ack(endpoint->in);
	}
	return USB_REQUEST_STATUS_OK;
}

usb_request_status_t usb_vendor_request_pwm_stop(
		usb_endpoint_t* const endpoint, const usb_transfer_stage_t stage) {
	if (stage == USB_TRANSFER_STAGE_SETUP) {
		// Stop timer
		SCT_REG16(SCT_CTRL_L) |= BIT2;

		usb_transfer_schedule_ack(endpoint->in);
	}
	return USB_REQUEST_STATUS_OK;
}

/* wValue = Freq; wIndex = Duty Cycle */
usb_request_status_t usb_vendor_request_pwm_set(
	usb_endpoint_t* const endpoint, const usb_transfer_stage_t stage) {
	if (stage == USB_TRANSFER_STAGE_SETUP) {

		SCT_REG16(SCT_CTRL_L) |= BIT2 | BIT3;

		if (endpoint->setup.value) {
			SCT_REG(SCT_MATCH0_L) = endpoint->setup.value;
			SCT_REG(SCT_MATCHREL0_L) = endpoint->setup.value;
		}

		if (endpoint->setup.index) {
			SCT_REG(SCT_MATCH1_L) = endpoint->setup.index;
			SCT_REG(SCT_MATCHREL1_L) = endpoint->setup.index;
		}

		SCT_REG(SCT_STATE) = 0;

		SCT_REG16(SCT_CTRL) &= ~(BIT2 | BIT1);

		usb_transfer_schedule_ack(endpoint->in);
	}
	return USB_REQUEST_STATUS_OK;
}
