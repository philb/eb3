/* Auto-generated config file hpl_ccl_config.h */
#ifndef HPL_CCL_CONFIG_H
#define HPL_CCL_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <o> Sequential Control Logic 0
// <0x0=> Sequential logic is disabled
// <0x1=> D flip flop
// <0x2=> JK flip flop
// <0x3=> D latch
// <0x4=> RS latch
// <i> Selects mode for sequential module 0
// <id> ccl_arch_seqsel_0
#ifndef CONF_CCL_SEQSEL_0
#define CONF_CCL_SEQSEL_0 0x0
#endif

// <o> Sequential Control Logic 1
// <0x0=> Sequential logic is disabled
// <0x1=> D flip flop
// <0x2=> JK flip flop
// <0x3=> D latch
// <0x4=> RS latch
// <i> Selects mode for sequential module 1
// <id> ccl_arch_seqsel_1
#ifndef CONF_CCL_SEQSEL_1
#define CONF_CCL_SEQSEL_1 0x0
#endif

// <e> Lookup Table Control 0
// <i> Enable and setup the lookup table module 0
// <id> ccl_arch_lutctrl0
#ifndef CONF_CCL_LUTCTRL_EN_0
#define CONF_CCL_LUTCTRL_EN_0 0
#endif

// <o> Truth Table <0x00-0xFF>
// <i> Define the value of truth logic according to inputs IN[2:0]
// <id> ccl_arch_truth_0
#ifndef CONF_CCL_TRUTH_0
#define CONF_CCL_TRUTH_0 0x0
#endif

// <o> Input Source Selection 0
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel0_0
#ifndef CONF_CCL_INSEL0_0
#define CONF_CCL_INSEL0_0 0x4
#endif

// <o> Input Source Selection 1
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel1_0
#ifndef CONF_CCL_INSEL1_0
#define CONF_CCL_INSEL1_0 0x4
#endif

// <o> Input Source Selection 2
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel2_0
#ifndef CONF_CCL_INSEL2_0
#define CONF_CCL_INSEL2_0 0x4
#endif

// <q> Edge detector enable
// <id> ccl_arch_edgesel_0
#ifndef CONF_CCL_EDGESEL_0
#define CONF_CCL_EDGESEL_0 0
#endif

// <o> Output Filter
// <0x0=> Disabled
// <0x1=> Synchronizer Enabled
// <0x2=> Filter Enabled
// <id> ccl_arch_filtsel_0
#ifndef CONF_CCL_FILTSEL_0
#define CONF_CCL_FILTSEL_0 0x0
#endif

// <h> Event settings 0

// <q> Event output enable
// <id> ccl_arch_luteo_0
#ifndef CONF_CCL_LUTEO_0
#define CONF_CCL_LUTEO_0 0
#endif

// <q> Event input enable
// <id> ccl_arch_lutei_0
#ifndef CONF_CCL_LUTEI_0
#define CONF_CCL_LUTEI_0 0
#endif

// <q> Event input invert
// <id> ccl_arch_invei_0
#ifndef CONF_CCL_INVEI_0
#define CONF_CCL_INVEI_0 0
#endif

// </h>

// </e>

// <hidden> Persistance settings 0

// <s> Expression Persistance
// <id> ccl_e_persistance_0
#define EXPRESSION_PERSISTANCE_0 ""

// <s> Logic Persistance
// <id> ccl_l_persistance_0
#define LOGIC_PERSISTANCE_0 ""

// </hidden>

// <e> Lookup Table Control 1
// <i> Enable and setup the lookup table module 1
// <id> ccl_arch_lutctrl1
#ifndef CONF_CCL_LUTCTRL_EN_1
#define CONF_CCL_LUTCTRL_EN_1 0
#endif

// <o> Truth Table <0x00-0xFF>
// <i> Define the value of truth logic according to inputs IN[2:0]
// <id> ccl_arch_truth_1
#ifndef CONF_CCL_TRUTH_1
#define CONF_CCL_TRUTH_1 0x0
#endif

// <o> Input Source Selection 0
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel0_1
#ifndef CONF_CCL_INSEL0_1
#define CONF_CCL_INSEL0_1 0x4
#endif

// <o> Input Source Selection 1
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel1_1
#ifndef CONF_CCL_INSEL1_1
#define CONF_CCL_INSEL1_1 0x4
#endif

// <o> Input Source Selection 2
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel2_1
#ifndef CONF_CCL_INSEL2_1
#define CONF_CCL_INSEL2_1 0x4
#endif

// <q> Edge detector enable
// <id> ccl_arch_edgesel_1
#ifndef CONF_CCL_EDGESEL_1
#define CONF_CCL_EDGESEL_1 0
#endif

// <o> Output Filter
// <0x0=> Disabled
// <0x1=> Synchronizer Enabled
// <0x2=> Filter Enabled
// <id> ccl_arch_filtsel_1
#ifndef CONF_CCL_FILTSEL_1
#define CONF_CCL_FILTSEL_1 0x0
#endif

// <h> Event settings 1

// <q> Event output enable
// <id> ccl_arch_luteo_1
#ifndef CONF_CCL_LUTEO_1
#define CONF_CCL_LUTEO_1 0
#endif

// <q> Event input enable
// <id> ccl_arch_lutei_1
#ifndef CONF_CCL_LUTEI_1
#define CONF_CCL_LUTEI_1 0
#endif

// <q> Event input invert
// <id> ccl_arch_invei_1
#ifndef CONF_CCL_INVEI_1
#define CONF_CCL_INVEI_1 0
#endif

// </h>

// </e>

// <hidden> Persistance settings 1

// <s> Expression Persistance
// <id> ccl_e_persistance_1
#define EXPRESSION_PERSISTANCE_1 ""

// <s> Logic Persistance
// <id> ccl_l_persistance_1
#define LOGIC_PERSISTANCE_1 ""

// </hidden>

// <e> Lookup Table Control 2
// <i> Enable and setup the lookup table module 2
// <id> ccl_arch_lutctrl2
#ifndef CONF_CCL_LUTCTRL_EN_2
#define CONF_CCL_LUTCTRL_EN_2 0
#endif

// <o> Truth Table <0x00-0xFF>
// <i> Define the value of truth logic according to inputs IN[2:0]
// <id> ccl_arch_truth_2
#ifndef CONF_CCL_TRUTH_2
#define CONF_CCL_TRUTH_2 0x0
#endif

// <o> Input Source Selection 0
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel0_2
#ifndef CONF_CCL_INSEL0_2
#define CONF_CCL_INSEL0_2 0x4
#endif

// <o> Input Source Selection 1
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel1_2
#ifndef CONF_CCL_INSEL1_2
#define CONF_CCL_INSEL1_2 0x4
#endif

// <o> Input Source Selection 2
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel2_2
#ifndef CONF_CCL_INSEL2_2
#define CONF_CCL_INSEL2_2 0x4
#endif

// <q> Edge detector enable
// <id> ccl_arch_edgesel_2
#ifndef CONF_CCL_EDGESEL_2
#define CONF_CCL_EDGESEL_2 0
#endif

// <o> Output Filter
// <0x0=> Disabled
// <0x1=> Synchronizer Enabled
// <0x2=> Filter Enabled
// <id> ccl_arch_filtsel_2
#ifndef CONF_CCL_FILTSEL_2
#define CONF_CCL_FILTSEL_2 0x0
#endif

// <h> Event settings 2

// <q> Event output enable
// <id> ccl_arch_luteo_2
#ifndef CONF_CCL_LUTEO_2
#define CONF_CCL_LUTEO_2 0
#endif

// <q> Event input enable
// <id> ccl_arch_lutei_2
#ifndef CONF_CCL_LUTEI_2
#define CONF_CCL_LUTEI_2 0
#endif

// <q> Event input invert
// <id> ccl_arch_invei_2
#ifndef CONF_CCL_INVEI_2
#define CONF_CCL_INVEI_2 0
#endif

// </h>

// </e>

// <hidden> Persistance settings 2

// <s> Expression Persistance
// <id> ccl_e_persistance_2
#define EXPRESSION_PERSISTANCE_2 ""

// <s> Logic Persistance
// <id> ccl_l_persistance_2
#define LOGIC_PERSISTANCE_2 ""

// </hidden>

// <e> Lookup Table Control 3
// <i> Enable and setup the lookup table module 3
// <id> ccl_arch_lutctrl3
#ifndef CONF_CCL_LUTCTRL_EN_3
#define CONF_CCL_LUTCTRL_EN_3 0
#endif

// <o> Truth Table <0x00-0xFF>
// <i> Define the value of truth logic according to inputs IN[2:0]
// <id> ccl_arch_truth_3
#ifndef CONF_CCL_TRUTH_3
#define CONF_CCL_TRUTH_3 0x0
#endif

// <o> Input Source Selection 0
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel0_3
#ifndef CONF_CCL_INSEL0_3
#define CONF_CCL_INSEL0_3 0x4
#endif

// <o> Input Source Selection 1
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel1_3
#ifndef CONF_CCL_INSEL1_3
#define CONF_CCL_INSEL1_3 0x4
#endif

// <o> Input Source Selection 2
// <0x0=> Masked input
// <0x1=> Feedback input source
// <0x2=> Linked LookUpTable input source
// <0x3=> Event input source
// <0x4=> IO pin input source
// <0x5=> AC input source
// <0x6=> TC input source
// <0x7=> Alternative TC input source
// <0x8=> TCC input source
// <0x9=> SERCOM input source
// <id> ccl_arch_insel2_3
#ifndef CONF_CCL_INSEL2_3
#define CONF_CCL_INSEL2_3 0x4
#endif

// <q> Edge detector enable
// <id> ccl_arch_edgesel_3
#ifndef CONF_CCL_EDGESEL_3
#define CONF_CCL_EDGESEL_3 0
#endif

// <o> Output Filter
// <0x0=> Disabled
// <0x1=> Synchronizer Enabled
// <0x2=> Filter Enabled
// <id> ccl_arch_filtsel_3
#ifndef CONF_CCL_FILTSEL_3
#define CONF_CCL_FILTSEL_3 0x0
#endif

// <h> Event settings 3

// <q> Event output enable
// <id> ccl_arch_luteo_3
#ifndef CONF_CCL_LUTEO_3
#define CONF_CCL_LUTEO_3 0
#endif

// <q> Event input enable
// <id> ccl_arch_lutei_3
#ifndef CONF_CCL_LUTEI_3
#define CONF_CCL_LUTEI_3 0
#endif

// <q> Event input invert
// <id> ccl_arch_invei_3
#ifndef CONF_CCL_INVEI_3
#define CONF_CCL_INVEI_3 0
#endif

// </h>

// </e>

// <hidden> Persistance settings 3

// <s> Expression Persistance
// <id> ccl_e_persistance_3
#define EXPRESSION_PERSISTANCE_3 ""

// <s> Logic Persistance
// <id> ccl_l_persistance_3
#define LOGIC_PERSISTANCE_3 ""

// </hidden>

// <e> Advanced configurations
// <id> ccl_arch_advanced_settings
#ifndef CONF_CCL_ADVANCED
#define CONF_CCL_ADVANCED 0
#endif

// <q> Run in Standby
// <id> ccl_arch_runstdby
#ifndef CONF_CCL_RUNSTDBY
#define CONF_CCL_RUNSTDBY 0
#endif

// </e>

// <<< end of configuration section >>>

#endif // HPL_CCL_CONFIG_H
