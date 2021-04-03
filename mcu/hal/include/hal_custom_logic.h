/**
 * \file
 *
 * \brief Custom Control Logic functionality declaration.
 *
 * Copyright (c) 2015-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "hpl_custom_logic.h"

#ifndef _HAL_CUSTOM_LOGIC_H_INCLUDED
#define _HAL_CUSTOM_LOGIC_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup doc_driver_hal_custom_logic
 *
 *@{
 */

/**
 *  \brief Initialize the custom logic hardware
 *  \return Initialization operation status
 */
static inline int32_t custom_logic_init(void)
{
	return _custom_logic_init();
}

/**
 *  \brief Disable and reset the custom logic hardware
 */
static inline void custom_logic_deinit(void)
{
	_custom_logic_deinit();
}

/**
 *  \brief Enable the custom logic hardware
 *  \return Initialization operation status
 */
static inline int32_t custom_logic_enable(void)
{
	return _custom_logic_enable();
}

/**
 *  \brief Disable the custom logic hardware
 */
static inline void custom_logic_disable(void)
{
	_custom_logic_disable();
}

	/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* _HAL_CUSTOM_LOGIC_H_INCLUDED */
