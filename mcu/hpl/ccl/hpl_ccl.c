/**
 * \file
 *
 * \brief Custom Control Logic functionality implementation.
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

#include <hpl_custom_logic.h>
#include <hpl_ccl_config.h>
#include <utils_repeat_macro.h>

/* Configuration value for CCL LUTCTRL reg */
#define _CCL_LUTCTRL(unused, n)                                                                                        \
	((CONF_CCL_LUTCTRL_EN_##n << 1) | ((uint32_t)CONF_CCL_TRUTH_##n << 24) | (CONF_CCL_INSEL0_##n << 8)                \
	 | (CONF_CCL_INSEL1_##n << 12) | (CONF_CCL_INSEL2_##n << 16) | (CONF_CCL_EDGESEL_##n << 7)                         \
	 | (CONF_CCL_FILTSEL_##n << 4) | (CONF_CCL_LUTEO_##n << 22) | (CONF_CCL_LUTEI_##n << 21)                           \
	 | (CONF_CCL_INVEI_##n << 20)),

/* Configuration value for CCL SEQCTRL reg */
#define _CCL_SEQCTRL(unused, n) ((CONF_CCL_SEQSEL_##n)),

/* Configuration value for CCL CTRL reg */
#define _CCL_CTRL() (CONF_CCL_RUNSTDBY << CCL_CTRL_RUNSTDBY_Pos)

#define REG_SEQCTRL_NUM 2
#define REG_LUTCTRL_NUM 4

struct _ccl_regs {
	uint32_t lutctrl[REG_LUTCTRL_NUM];
	uint8_t  seqctrl[REG_SEQCTRL_NUM];
	uint8_t  ctrl;
};

const struct _ccl_regs _ccl_reg_cfgs = {{REPEAT_MACRO(_CCL_LUTCTRL, unused, REG_LUTCTRL_NUM)},
                                        {REPEAT_MACRO(_CCL_SEQCTRL, unused, REG_SEQCTRL_NUM)},
                                        _CCL_CTRL()};

int32_t _custom_logic_init(void)
{
	uint8_t i;

	hri_ccl_write_CTRL_reg(CCL, 0);
	hri_ccl_write_CTRL_reg(CCL, CCL_CTRL_SWRST);

	for (i = 0; i < REG_SEQCTRL_NUM; i++) {
		hri_ccl_write_SEQCTRL_reg(CCL, i, _ccl_reg_cfgs.seqctrl[i]);
	}
	for (i = 0; i < REG_LUTCTRL_NUM; i++) {
		hri_ccl_write_LUTCTRL_reg(CCL, i, _ccl_reg_cfgs.lutctrl[i]);
	}

	hri_ccl_write_CTRL_reg(CCL, _ccl_reg_cfgs.ctrl);
	return ERR_NONE;
}

void _custom_logic_deinit(void)
{
	hri_ccl_clear_CTRL_ENABLE_bit(CCL);
	hri_ccl_write_CTRL_reg(CCL, CCL_CTRL_SWRST);
}

int32_t _custom_logic_enable(void)
{
	hri_ccl_set_CTRL_ENABLE_bit(CCL);
	return ERR_NONE;
}

void _custom_logic_disable(void)
{
	hri_ccl_clear_CTRL_ENABLE_bit(CCL);
}
