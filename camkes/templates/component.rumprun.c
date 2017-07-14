/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the BSD 2-Clause license. Note that NO WARRANTY is provided.
 * See "LICENSE_BSD2.txt" for details.
 *
 * @TAG(DATA61_BSD)
 */

#include <stdio.h>
#include <camkes.h>
#include <simple/simple.h>
#include <rumprun/custom_simple.h>
#include <sel4/sel4_arch/constants.h>

/* TODO: This function isn't prototyped anywhere */
void camkes_make_simple(simple_t *simple);

static simple_t camkes_simple;
static custom_simple_t custom_simple;
static char cmdline[200];

/*- set rump_config = configuration[me.name].get('rump_config') -*/
/*- set eth_irq_config = rump_config.get('eth_irq_interface') -*/
/*- set eth_irq_number = rump_config.get('eth_irq_number') -*/
/*- if eth_irq_config != "" -*/
void /*? eth_irq_config ?*/_handle(void) {
    rump_irq_handle(BIT(/*? eth_irq_number ?*/));
}
/*- endif -*/

int run(void) {

    if (rump_config.rump_net == 1) {
        sprintf(cmdline, "{,,\"net\" :  {,,\"if\":\"wm0\",, \"type\":\"inet\",,\"method\":\"dhcp\",,},,\"cmdline\": \"%s\",,},,", rump_config.cmdline);
    } else {
        sprintf(cmdline, "{,,\"cmdline\": \"%s\",,},,", rump_config.cmdline);
    }
    camkes_make_simple(&camkes_simple);

    custom_simple.cmdline = cmdline;
    custom_simple.priority = rump_priority;
    custom_simple.rumprun_memory_size = /*? rump_config.get('rumprun_memory_size') ?*/;
    custom_simple.simple = &camkes_simple;
    custom_simple.camkes = true;
    custom_simple.timer_config.timer = TIMER_INTERFACE;
/*- set timer_config_interface = rump_config.get('timer_interface') -*/
    custom_simple.timer_config.timer_cap = /*? timer_config_interface ?*/_notification();
    custom_simple.timer_config.oneshot_relative = /*? timer_config_interface ?*/_oneshot_relative;
    custom_simple.timer_config.tsc_freq = /*? timer_config_interface ?*/_tsc_frequency();

/*- set serial_config_interface = rump_config.get('serial_interface') -*/
    custom_simple.serial_config.serial = SERIAL_INTERFACE;
    custom_simple.serial_config.putchar = /*? serial_config_interface ?*/_putchar;

/*- set pci_config_interface = rump_config.get('pci_config_interface') -*/
/*- if pci_config_interface != "" -*/
    /* Set the PCI configuration interface if one is present */
    custom_simple.pci_config_config.pci_config = PCI_CONFIG_INTERFACE;
    custom_simple.pci_config_config.pci_config_read32 = /*? pci_config_interface ?*/_read32;
    custom_simple.pci_config_config.pci_config_write32 = /*? pci_config_interface ?*/_write32;
    custom_simple.ethernet_intr_config.eth_irq_acknowledge = /*? eth_irq_config ?*/_acknowledge;
/*- endif -*/

    /* Start rumprun instance */
    init_rumprun(&custom_simple);

    return 0;
 }
