/*
 * Copyright (c) 2012 Qualcomm Atheros, Inc.
 * All rights reserved.
 * Qualcomm Atheros Confidential and Proprietary.
 *
 */
/**
 * @defgroup shiva_mirror SHIVA_MIRROR
 * @{
 */
#include "sw.h"
#include "hsl.h"
#include "hsl_dev.h"
#include "hsl_port_prop.h"
#include "shiva_mirror.h"
#include "shiva_reg.h"

static sw_error_t
_shiva_mirr_analysis_port_set(a_uint32_t dev_id, fal_port_t port_id)
{
    sw_error_t rv;
    a_uint32_t val;

    HSL_DEV_ID_CHECK(dev_id);

    if (A_TRUE != hsl_port_prop_check(dev_id, port_id, HSL_PP_INCL_CPU))
    {
        return SW_BAD_PARAM;
    }

    val = port_id;
    HSL_REG_FIELD_SET(rv, dev_id, CPU_PORT, 0, MIRROR_PORT_NUM,
                      (a_uint8_t *) (&val), sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_shiva_mirr_analysis_port_get(a_uint32_t dev_id, fal_port_t * port_id)
{
    sw_error_t rv;
    a_uint32_t val;

    HSL_DEV_ID_CHECK(dev_id);

    HSL_REG_FIELD_GET(rv, dev_id, CPU_PORT, 0, MIRROR_PORT_NUM,
                      (a_uint8_t *) (&val), sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    *port_id = val;
    return SW_OK;
}

static sw_error_t
_shiva_mirr_port_in_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable)
{
    sw_error_t rv;
    a_uint32_t val;

    HSL_DEV_ID_CHECK(dev_id);

    if (A_TRUE != hsl_port_prop_check(dev_id, port_id, HSL_PP_INCL_CPU))
    {
        return SW_BAD_PARAM;
    }

    if (A_TRUE == enable)
    {
        val = 1;
    }
    else if (A_FALSE == enable)
    {
        val = 0;
    }
    else
    {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_CTL, port_id, ING_MIRROR_EN,
                      (a_uint8_t *) (&val), sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_shiva_mirr_port_in_get(a_uint32_t dev_id, fal_port_t port_id,
                        a_bool_t * enable)
{
    sw_error_t rv;
    a_uint32_t val;

    HSL_DEV_ID_CHECK(dev_id);

    if (A_TRUE != hsl_port_prop_check(dev_id, port_id, HSL_PP_INCL_CPU))
    {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_GET(rv, dev_id, PORT_CTL, port_id, ING_MIRROR_EN,
                      (a_uint8_t *) (&val), sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    if (1 == val)
    {
        *enable = A_TRUE;
    }
    else
    {
        *enable = A_FALSE;
    }

    return SW_OK;
}

static sw_error_t
_shiva_mirr_port_eg_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable)
{
    sw_error_t rv;
    a_uint32_t val;

    HSL_DEV_ID_CHECK(dev_id);

    if (A_TRUE != hsl_port_prop_check(dev_id, port_id, HSL_PP_INCL_CPU))
    {
        return SW_BAD_PARAM;
    }

    if (A_TRUE == enable)
    {
        val = 1;
    }
    else if (A_FALSE == enable)
    {
        val = 0;
    }
    else
    {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_SET(rv, dev_id, PORT_CTL, port_id, EG_MIRROR_EN,
                      (a_uint8_t *) (&val), sizeof (a_uint32_t));
    return rv;
}

static sw_error_t
_shiva_mirr_port_eg_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable)
{
    sw_error_t rv;
    a_uint32_t val;

    HSL_DEV_ID_CHECK(dev_id);

    if (A_TRUE != hsl_port_prop_check(dev_id, port_id, HSL_PP_INCL_CPU))
    {
        return SW_BAD_PARAM;
    }

    HSL_REG_FIELD_GET(rv, dev_id, PORT_CTL, port_id, EG_MIRROR_EN,
                      (a_uint8_t *) (&val), sizeof (a_uint32_t));
    SW_RTN_ON_ERROR(rv);

    if (1 == val)
    {
        *enable = A_TRUE;
    }
    else
    {
        *enable = A_FALSE;
    }

    return SW_OK;
}

/**
 *   @details  Comments:
 *   The analysis port works for both ingress and egress mirror.
 * @brief Set mirror analyzer port on particular a device.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
shiva_mirr_analysis_port_set(a_uint32_t dev_id, fal_port_t port_id)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _shiva_mirr_analysis_port_set(dev_id, port_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get mirror analysis port on particular a device.
 * @param[in] dev_id device id
 * @param[out] port_id port id
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
shiva_mirr_analysis_port_get(a_uint32_t dev_id, fal_port_t * port_id)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _shiva_mirr_analysis_port_get(dev_id, port_id);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set ingress mirror status on particular a port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
shiva_mirr_port_in_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _shiva_mirr_port_in_set(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get ingress mirror status on particular a port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
shiva_mirr_port_in_get(a_uint32_t dev_id, fal_port_t port_id,
                       a_bool_t * enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _shiva_mirr_port_in_get(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Set egress mirror status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[in] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
shiva_mirr_port_eg_set(a_uint32_t dev_id, fal_port_t port_id, a_bool_t enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _shiva_mirr_port_eg_set(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

/**
 * @brief Get egress mirror status on a particular port.
 * @param[in] dev_id device id
 * @param[in] port_id port id
 * @param[out] enable A_TRUE or A_FALSE
 * @return SW_OK or error code
 */
HSL_LOCAL sw_error_t
shiva_mirr_port_eg_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable)
{
    sw_error_t rv;

    HSL_API_LOCK;
    rv = _shiva_mirr_port_eg_get(dev_id, port_id, enable);
    HSL_API_UNLOCK;
    return rv;
}

sw_error_t
shiva_mirr_init(a_uint32_t dev_id)
{
    HSL_DEV_ID_CHECK(dev_id);

#ifndef HSL_STANDALONG
    {
        hsl_api_t *p_api;

        SW_RTN_ON_NULL(p_api = hsl_api_ptr_get(dev_id));

        p_api->mirr_analysis_port_set = shiva_mirr_analysis_port_set;
        p_api->mirr_analysis_port_get = shiva_mirr_analysis_port_get;
        p_api->mirr_port_in_set = shiva_mirr_port_in_set;
        p_api->mirr_port_in_get = shiva_mirr_port_in_get;
        p_api->mirr_port_eg_set = shiva_mirr_port_eg_set;
        p_api->mirr_port_eg_get = shiva_mirr_port_eg_get;
    }
#endif

    return SW_OK;
}

/**
 * @}
 */

