/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
/**
 * @defgroup garuda_qos GARUDA_QOS
 * @{
 */
#ifndef _GARUDA_QOS_H_
#define _GARUDA_QOS_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_qos.h"

    sw_error_t garuda_qos_init(a_uint32_t dev_id);

#ifdef IN_QOS
#define GARUDA_QOS_INIT(rv, dev_id) \
    { \
        rv = garuda_qos_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
#define GARUDA_QOS_INIT(rv, dev_id)
#endif

#ifdef HSL_STANDALONG


    HSL_LOCAL sw_error_t
    garuda_qos_sch_mode_set(a_uint32_t dev_id,
                            fal_sch_mode_t mode, const a_uint32_t weight[]);



    HSL_LOCAL sw_error_t
    garuda_qos_sch_mode_get(a_uint32_t dev_id,
                            fal_sch_mode_t * mode, a_uint32_t weight[]);



    HSL_LOCAL sw_error_t
    garuda_qos_queue_tx_buf_status_set(a_uint32_t dev_id,
                                       fal_port_t port_id, a_bool_t enable);



    HSL_LOCAL sw_error_t
    garuda_qos_queue_tx_buf_status_get(a_uint32_t dev_id,
                                       fal_port_t port_id,
                                       a_bool_t * enable);


    HSL_LOCAL sw_error_t
    garuda_qos_port_tx_buf_status_set(a_uint32_t dev_id, fal_port_t port_id,
                                      a_bool_t enable);



    HSL_LOCAL sw_error_t
    garuda_qos_port_tx_buf_status_get(a_uint32_t dev_id, fal_port_t port_id,
                                      a_bool_t * enable);



    HSL_LOCAL sw_error_t
    garuda_qos_queue_tx_buf_nr_set(a_uint32_t dev_id, fal_port_t port_id,
                                   fal_queue_t queue_id,
                                   a_uint32_t * number);



    HSL_LOCAL sw_error_t
    garuda_qos_queue_tx_buf_nr_get(a_uint32_t dev_id, fal_port_t port_id,
                                   fal_queue_t queue_id,
                                   a_uint32_t * number);



    HSL_LOCAL sw_error_t
    garuda_qos_port_tx_buf_nr_set(a_uint32_t dev_id, fal_port_t port_id,
                                  a_uint32_t * number);



    HSL_LOCAL sw_error_t
    garuda_qos_port_tx_buf_nr_get(a_uint32_t dev_id, fal_port_t port_id,
                                  a_uint32_t * number);



    HSL_LOCAL sw_error_t
    garuda_cosmap_up_queue_set(a_uint32_t dev_id, a_uint32_t up,
                               fal_queue_t queue);


    HSL_LOCAL sw_error_t
    garuda_cosmap_up_queue_get(a_uint32_t dev_id, a_uint32_t up,
                               fal_queue_t * queue);



    HSL_LOCAL sw_error_t
    garuda_cosmap_dscp_queue_set(a_uint32_t dev_id, a_uint32_t dscp,
                                 fal_queue_t queue);



    HSL_LOCAL sw_error_t
    garuda_cosmap_dscp_queue_get(a_uint32_t dev_id, a_uint32_t dscp,
                                 fal_queue_t * queue);



    HSL_LOCAL sw_error_t
    garuda_qos_port_mode_set(a_uint32_t dev_id, fal_port_t port_id,
                             fal_qos_mode_t mode, a_bool_t enable);



    HSL_LOCAL sw_error_t
    garuda_qos_port_mode_get(a_uint32_t dev_id, fal_port_t port_id,
                             fal_qos_mode_t mode, a_bool_t * enable);



    HSL_LOCAL sw_error_t
    garuda_qos_port_mode_pri_set(a_uint32_t dev_id, fal_port_t port_id,
                                 fal_qos_mode_t mode, a_uint32_t pri);


    HSL_LOCAL sw_error_t
    garuda_qos_port_mode_pri_get(a_uint32_t dev_id, fal_port_t port_id,
                                 fal_qos_mode_t mode, a_uint32_t * pri);



    HSL_LOCAL sw_error_t
    garuda_qos_port_default_up_set(a_uint32_t dev_id, fal_port_t port_id,
                                   a_uint32_t up);



    HSL_LOCAL sw_error_t
    garuda_qos_port_default_up_get(a_uint32_t dev_id, fal_port_t port_id,
                                   a_uint32_t * up);
#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* _GARUDA_QOS_H_ */
/**
 * @}
 */
