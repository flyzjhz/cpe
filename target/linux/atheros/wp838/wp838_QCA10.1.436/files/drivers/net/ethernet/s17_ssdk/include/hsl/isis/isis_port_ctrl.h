/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
#ifndef _ISIS_PORT_CTRL_H_
#define _ISIS_PORT_CTRL_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_port_ctrl.h"

    sw_error_t isis_port_ctrl_init(a_uint32_t dev_id);

#ifdef IN_PORTCONTROL
#define ISIS_PORT_CTRL_INIT(rv, dev_id) \
    { \
        rv = isis_port_ctrl_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
#define ISIS_PORT_CTRL_INIT(rv, dev_id)
#endif

#ifdef HSL_STANDALONG


    HSL_LOCAL sw_error_t
    isis_port_duplex_set(a_uint32_t dev_id, fal_port_t port_id,
                         fal_port_duplex_t duplex);


    HSL_LOCAL sw_error_t
    isis_port_duplex_get(a_uint32_t dev_id, fal_port_t port_id,
                         fal_port_duplex_t * pduplex);


    HSL_LOCAL sw_error_t
    isis_port_speed_set(a_uint32_t dev_id, fal_port_t port_id,
                        fal_port_speed_t speed);


    HSL_LOCAL sw_error_t
    isis_port_speed_get(a_uint32_t dev_id, fal_port_t port_id,
                        fal_port_speed_t * pspeed);


    HSL_LOCAL sw_error_t
    isis_port_autoneg_status_get(a_uint32_t dev_id, fal_port_t port_id,
                                 a_bool_t * status);


    HSL_LOCAL sw_error_t
    isis_port_autoneg_enable(a_uint32_t dev_id, fal_port_t port_id);


    HSL_LOCAL sw_error_t
    isis_port_autoneg_restart(a_uint32_t dev_id, fal_port_t port_id);


    HSL_LOCAL sw_error_t
    isis_port_autoneg_adv_set(a_uint32_t dev_id, fal_port_t port_id,
                              a_uint32_t autoadv);


    HSL_LOCAL sw_error_t
    isis_port_autoneg_adv_get(a_uint32_t dev_id, fal_port_t port_id,
                              a_uint32_t * autoadv);


    HSL_LOCAL sw_error_t
    isis_port_flowctrl_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_flowctrl_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_flowctrl_forcemode_set(a_uint32_t dev_id,
                                     fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_flowctrl_forcemode_get(a_uint32_t dev_id,
                                     fal_port_t port_id,
                                     a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_powersave_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_powersave_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t *enable);


    HSL_LOCAL sw_error_t
    isis_port_hibernate_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_hibernate_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t *enable);


    HSL_LOCAL sw_error_t
    isis_port_cdt(a_uint32_t dev_id, fal_port_t port_id, a_uint32_t mdi_pair,
                  fal_cable_status_t *cable_status, a_uint32_t *cable_len);


    HSL_LOCAL sw_error_t
    isis_port_rxhdr_mode_set(a_uint32_t dev_id, fal_port_t port_id,
                             fal_port_header_mode_t mode);


    HSL_LOCAL sw_error_t
    isis_port_rxhdr_mode_get(a_uint32_t dev_id, fal_port_t port_id,
                             fal_port_header_mode_t * mode);


    HSL_LOCAL sw_error_t
    isis_port_txhdr_mode_set(a_uint32_t dev_id, fal_port_t port_id,
                             fal_port_header_mode_t mode);


    HSL_LOCAL sw_error_t
    isis_port_txhdr_mode_get(a_uint32_t dev_id, fal_port_t port_id,
                             fal_port_header_mode_t * mode);


    HSL_LOCAL sw_error_t
    isis_header_type_set(a_uint32_t dev_id, a_bool_t enable, a_uint32_t type);


    HSL_LOCAL sw_error_t
    isis_header_type_get(a_uint32_t dev_id, a_bool_t * enable, a_uint32_t * type);


    HSL_LOCAL sw_error_t
    isis_port_txmac_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_txmac_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_rxmac_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_rxmac_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_txfc_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_txfc_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_rxfc_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_rxfc_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_bp_status_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_bp_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_link_forcemode_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_link_forcemode_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


    HSL_LOCAL sw_error_t
    isis_port_link_status_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * status);

    HSL_LOCAL sw_error_t
    isis_port_mac_loopback_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable);


    HSL_LOCAL sw_error_t
    isis_port_mac_loopback_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);

#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* _ISIS_PORT_CTRL_H_ */

