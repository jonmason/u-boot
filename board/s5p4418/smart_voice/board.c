/*
 * (C) Copyright 2016 Nexell
 * Youngbok, Park <ybpark@nexell.co.kr>
 *
 * SPDX-License-Identifier:      GPL-2.0+
 */

#include <config.h>
#include <common.h>
#ifdef CONFIG_PWM_NX
#include <pwm.h>
#endif
#include <asm/io.h>

#ifdef CONFIG_DM_PMIC_NXE2000
#include <dm.h>
#include <dm/uclass-internal.h>
#include <power/pmic.h>
#include <power/nxe2000.h>
#endif

#include <asm/arch/nexell.h>
#include <asm/arch/nx_gpio.h>

DECLARE_GLOBAL_DATA_PTR;

/*------------------------------------------------------------------------------
 * intialize nexell soc and board status.
 */

/* call from u-boot */
int board_early_init_f(void)
{
	return 0;
}

#ifdef CONFIG_DM_PMIC_NXE2000
void pmic_init(void)
{
	static struct udevice *dev;
	int ret = -ENODEV;
	uint8_t bit_mask = 0;

	ret = pmic_get("nxe2000_gpio@32", &dev);
	if (ret)
		printf("Can't get PMIC: %s!\n", "nxe2000_gpio@32");

	bit_mask = pmic_reg_read(dev, NXE2000_REG_PWRONTIMSET);
	bit_mask &= ~(0x1 << NXE2000_POS_PWRONTIMSET_OFF_JUDGE_PWRON);
	bit_mask |= (0x0 << NXE2000_POS_PWRONTIMSET_OFF_JUDGE_PWRON);
	ret = pmic_write(dev, NXE2000_REG_PWRONTIMSET, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC REG: %d!\n", NXE2000_REG_PWRONTIMSET);

	bit_mask = 0x00;
	ret = pmic_reg_write(dev, (u32)NXE2000_REG_BANKSEL, (u32)bit_mask);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_BANKSEL);

	bit_mask = ((0 << NXE2000_POS_DCxCTL2_DCxOSC) |
			(0 << NXE2000_POS_DCxCTL2_DCxSR) |
			(3 << NXE2000_POS_DCxCTL2_DCxLIM) |
			(0 << NXE2000_POS_DCxCTL2_DCxLIMSDEN));
	ret = pmic_write(dev, NXE2000_REG_DC1CTL2, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_DC1CTL2);

	bit_mask = ((0 << NXE2000_POS_DCxCTL2_DCxOSC) |
			(0 << NXE2000_POS_DCxCTL2_DCxSR) |
			(3 << NXE2000_POS_DCxCTL2_DCxLIM) |
			(0 << NXE2000_POS_DCxCTL2_DCxLIMSDEN));
	ret = pmic_write(dev, NXE2000_REG_DC2CTL2, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_DC2CTL2);

	bit_mask = ((0 << NXE2000_POS_DCxCTL2_DCxOSC) |
			(0 << NXE2000_POS_DCxCTL2_DCxSR) |
			(1 << NXE2000_POS_DCxCTL2_DCxLIM) |
			(1 << NXE2000_POS_DCxCTL2_DCxLIMSDEN));
	ret = pmic_write(dev, NXE2000_REG_DC3CTL2, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_DC3CTL2);

	bit_mask = ((0 << NXE2000_POS_DCxCTL2_DCxOSC) |
			(0 << NXE2000_POS_DCxCTL2_DCxSR) |
			(1 << NXE2000_POS_DCxCTL2_DCxLIM) |
			(1 << NXE2000_POS_DCxCTL2_DCxLIMSDEN));
	ret = pmic_write(dev, NXE2000_REG_DC4CTL2, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_DC4CTL2);

	bit_mask = ((0 << NXE2000_POS_DCxCTL2_DCxOSC) |
			(0 << NXE2000_POS_DCxCTL2_DCxSR) |
			(1 << NXE2000_POS_DCxCTL2_DCxLIM) |
			(1 << NXE2000_POS_DCxCTL2_DCxLIMSDEN));
	ret = pmic_write(dev, NXE2000_REG_DC5CTL2, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_DC5CTL2);

	bit_mask = (1 << NXE2000_POS_CHGCTL1_SUSPEND);
	ret = pmic_write(dev, NXE2000_REG_CHGCTL1, &bit_mask, 1);
	if (ret)
		printf("Can't write PMIC register: %d!\n", NXE2000_REG_CHGCTL1);
}
#endif

void board_gpio_init(void)
{
	nx_gpio_initialize();
	nx_gpio_set_base_address(0, (void *)PHY_BASEADDR_GPIOA);
	nx_gpio_set_base_address(1, (void *)PHY_BASEADDR_GPIOB);
	nx_gpio_set_base_address(2, (void *)PHY_BASEADDR_GPIOC);
	nx_gpio_set_base_address(3, (void *)PHY_BASEADDR_GPIOD);
	nx_gpio_set_base_address(4, (void *)PHY_BASEADDR_GPIOE);
}

int mmc_get_env_dev(void)
{
	return 0;
}

int board_init(void)
{
	board_gpio_init();
	return 0;
}

/* u-boot dram initialize  */
int dram_init(void)
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

/* u-boot dram board specific */
void dram_init_banksize(void)
{
	/* set global data memory */
	gd->bd->bi_arch_number = machine_arch_type;
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x00000100;

	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size  = CONFIG_SYS_SDRAM_SIZE;
}

int board_late_init(void)
{
#ifdef CONFIG_DM_PMIC_NXE2000
	pmic_init();
#endif
	run_command("ext4load mmc 0 0x49000000 nxc100_ver_01.bin", 0);
	run_command("nxc100 0x49000000", 0);
	return 0;
}

