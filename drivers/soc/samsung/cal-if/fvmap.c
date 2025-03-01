#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/io.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <soc/samsung/cal-if.h>

#include "fvmap.h"
#include "cmucal.h"
#include "vclk.h"
#include "ra.h"

#define FVMAP_SIZE		(SZ_8K)
#define STEP_UV			(6250)

void __iomem *fvmap_base;
void __iomem *sram_fvmap_base;

static int init_margin_table[MAX_MARGIN_ID] = {0};
static int volt_offset_percent = 0;

static int __init get_all_volt(char *str)
{
	int i, volt;

	get_option(&str, &volt);

	for (i = MARGIN_MIF; i <= MARGIN_G3D; i++) {
		if (init_margin_table[i] == 0)
			init_margin_table[i] = volt;
	}

	return 0;
}
early_param("all", get_all_volt);

static int __init get_mif_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_MIF] = volt;

	return 0;
}
early_param("mif", get_mif_volt);

static int __init get_int_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_INT] = volt;

	return 0;
}
early_param("int", get_int_volt);

static int __init get_big_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_BIG] = volt;

	return 0;
}
early_param("big", get_big_volt);

static int __init get_lit_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_LIT] = volt;

	return 0;
}
early_param("lit", get_lit_volt);

static int __init get_g3d_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_G3D] = volt;

	return 0;
}
early_param("g3d", get_g3d_volt);

static int __init get_fsys_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_FSYS] = volt;

	return 0;
}
early_param("fsys", get_fsys_volt);

static int __init get_intcam_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_INTCAM] = volt;

	return 0;
}
early_param("intcam", get_intcam_volt);

static int __init get_cam_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_CAM] = volt;

	return 0;
}
early_param("cam", get_cam_volt);

static int __init get_disp_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_DISP] = volt;

	return 0;
}
early_param("disp", get_disp_volt);

static int __init get_g3dm_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_G3DM] = volt;

	return 0;
}
early_param("g3dm", get_g3dm_volt);

static int __init get_aud_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_AUD] = volt;

	return 0;
}
early_param("aud", get_aud_volt);

static int __init get_cp_volt(char *str)
{
	int volt;

	get_option(&str, &volt);
	init_margin_table[MARGIN_CP] = volt;

	return 0;
}
early_param("cp", get_cp_volt);

static int __init get_percent_margin_volt(char *str)
{
	int percent;

	get_option(&str, &percent);
	volt_offset_percent = percent;

	return 0;
}
early_param("volt_offset_percent", get_percent_margin_volt);

int fvmap_set_raw_voltage_table(unsigned int id, int uV)
{
	struct fvmap_header *fvmap_header;
	struct rate_volt_header *fv_table;
	int num_of_lv;
	int idx, i;

	idx = GET_IDX(id);

	fvmap_header = sram_fvmap_base;
	fv_table = sram_fvmap_base + fvmap_header[idx].o_ratevolt;
	num_of_lv = fvmap_header[idx].num_of_lv;

	for (i = 0; i < num_of_lv; i++)
		fv_table->table[i].volt += uV;

	return 0;
}

int fvmap_get_voltage_table(unsigned int id, unsigned int *table)
{
	struct fvmap_header *fvmap_header = fvmap_base;
	struct rate_volt_header *fv_table;
	int idx, i;
	int num_of_lv;

	if (!IS_ACPM_VCLK(id))
		return 0;

	idx = GET_IDX(id);

	fvmap_header = fvmap_base;
	fv_table = fvmap_base + fvmap_header[idx].o_ratevolt;
	num_of_lv = fvmap_header[idx].num_of_lv;
	for (i = 0; i < num_of_lv; i++)
	{	if(num_of_lv==18){
			//	for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
				/*Undervolt little cores*/
				fv_table->table[0].volt=950000; 	//freq=2002 1168750
				fv_table->table[1].volt=912500; 	//freq=1898 1131250
				
				fv_table->table[2].volt=881250; 	//freq=1794 1043750
				fv_table->table[3].volt=850000; 	//freq=1690 1031250
				fv_table->table[4].volt=825000; 	//freq=1586 1031250
				fv_table->table[5].volt=806250; 	//freq=1482 987500
				fv_table->table[6].volt=775000; 	//freq=1352 931250
				fv_table->table[7].volt=750000; 	//freq=1248 893750
				fv_table->table[8].volt=731250; 	//freq=1144 856250
				fv_table->table[9].volt=706250; 	//freq=1014 806250
				fv_table->table[10].volt=662500; 	//freq=902  768750
				
				fv_table->table[11].volt=643750; 	//freq=839  750000
				fv_table->table[12].volt=618750; 	//freq=757  731250
				fv_table->table[13].volt=606250; 	//freq=676  706250
				fv_table->table[14].volt=568750; 	//freq=546  668750
				fv_table->table[15].volt=550000;	//freq=449  637500
				fv_table->table[16].volt=531250;	//freq=343  612500
				fv_table->table[17].volt=500000; 	//freq=208  600000
		}
		if(num_of_lv==17){
				//for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
				/*Undervolt the BIG cores*/
				fv_table->table[0].volt=1062500;	// freq=2496 1256250
				
				fv_table->table[1].volt=1050000;	// freq=2392 1256250
				fv_table->table[2].volt=1037500;	// freq=2288 1187500
				fv_table->table[3].volt=1018750;	// freq=2184 1137500
				fv_table->table[4].volt=993750;		// freq=2080 1181250
				fv_table->table[5].volt=968750;		// freq=1976 1125000
				fv_table->table[6].volt=931250;		// freq=1872 1081250
				fv_table->table[7].volt=887500;		// freq=1768 1037500
				fv_table->table[8].volt=850000;		// freq=1664 1000000
				fv_table->table[9].volt=812500;		// freq=1560 962500
				fv_table->table[10].volt=750000;	// freq=1352 887500
				fv_table->table[11].volt=681250;	// freq=1144 818750
				fv_table->table[12].volt=625000;	// freq=936  762500
				
				fv_table->table[13].volt=587500;	// freq=728  700000
				fv_table->table[14].volt=568750;	// freq=520  643750
				fv_table->table[15].volt=543750;	// freq=312  600000
				fv_table->table[16].volt=531250;	// freq=208  600000
		}
		// This is used to reveal MIF voltages, MIF level is 10
		//if(num_of_lv==10){
				//for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
		//}
		// Undervolt the GPU by 200000uV
		if(num_of_lv==9){
				//for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
				fv_table->table[0].volt=1000000;	// freq=1300	1200000
				fv_table->table[1].volt=893750;		// freq=1200	1093750
				fv_table->table[2].volt=818750;		// freq=1100	1018750
				fv_table->table[3].volt=743750;		// freq=1001	943750
				fv_table->table[4].volt=643750;		// freq=845		843750
				fv_table->table[5].volt=556250;		// freq=676		756250
				fv_table->table[6].volt=518750;		// freq=545		718750
				fv_table->table[7].volt=481250;		// freq=450		681250
				fv_table->table[8].volt=425000;		// freq=343		625000
		}
		table[i] = fv_table->table[i].volt;
		pr_info("  num_of_lv : %u, table[i].volt : %u,topser99voltage_table\n",
				fv_table->table[i].volt,num_of_lv);
	}
	return num_of_lv;

}

int fvmap_get_raw_voltage_table(unsigned int id)
{
	struct fvmap_header *fvmap_header;
	struct rate_volt_header *fv_table;
	int idx, i;
	int num_of_lv;
	unsigned int table[20];

	idx = GET_IDX(id);

	fvmap_header = sram_fvmap_base;
	fv_table = sram_fvmap_base + fvmap_header[idx].o_ratevolt;
	num_of_lv = fvmap_header[idx].num_of_lv;

	for (i = 0; i < num_of_lv; i++)
{if(num_of_lv==18){
			//	for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
				/*Undervolt little cores*/
				fv_table->table[0].volt=950000; 	//freq=2002 1168750
				fv_table->table[1].volt=912500; 	//freq=1898 1131250
				
				fv_table->table[2].volt=881250; 	//freq=1794 1043750
				fv_table->table[3].volt=850000; 	//freq=1690 1031250
				fv_table->table[4].volt=825000; 	//freq=1586 1031250
				fv_table->table[5].volt=806250; 	//freq=1482 987500
				fv_table->table[6].volt=775000; 	//freq=1352 931250
				fv_table->table[7].volt=750000; 	//freq=1248 893750
				fv_table->table[8].volt=731250; 	//freq=1144 856250
				fv_table->table[9].volt=706250; 	//freq=1014 806250
				fv_table->table[10].volt=662500; 	//freq=902  768750
				
				fv_table->table[11].volt=643750; 	//freq=839  750000
				fv_table->table[12].volt=618750; 	//freq=757  731250
				fv_table->table[13].volt=606250; 	//freq=676  706250
				fv_table->table[14].volt=568750; 	//freq=546  668750
				fv_table->table[15].volt=550000;	//freq=449  637500
				fv_table->table[16].volt=531250;	//freq=343  612500
				fv_table->table[17].volt=500000; 	//freq=208  600000
		}
		if(num_of_lv==17){
				//for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
				/*Undervolt the BIG cores*/
				fv_table->table[0].volt=1062500;	// freq=2496 1256250
				
				fv_table->table[1].volt=1050000;	// freq=2392 1256250
				fv_table->table[2].volt=1037500;	// freq=2288 1187500
				fv_table->table[3].volt=1018750;	// freq=2184 1137500
				fv_table->table[4].volt=993750;		// freq=2080 1181250
				fv_table->table[5].volt=968750;		// freq=1976 1125000
				fv_table->table[6].volt=931250;		// freq=1872 1081250
				fv_table->table[7].volt=887500;		// freq=1768 1037500
				fv_table->table[8].volt=850000;		// freq=1664 1000000
				fv_table->table[9].volt=812500;		// freq=1560 962500
				fv_table->table[10].volt=750000;	// freq=1352 887500
				fv_table->table[11].volt=681250;	// freq=1144 818750
				fv_table->table[12].volt=625000;	// freq=936  762500
				
				fv_table->table[13].volt=587500;	// freq=728  700000
				fv_table->table[14].volt=568750;	// freq=520  643750
				fv_table->table[15].volt=543750;	// freq=312  600000
				fv_table->table[16].volt=531250;	// freq=208  600000
		}
		// This is used to reveal MIF voltages, MIF level is 10
		//if(num_of_lv==10){
				//for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
		//}
		// Undervolt the GPU by 200000uV
		if(num_of_lv==9){
				//for(topser =0 ;topser < num_of_lv; topser++){
					//fv_table->table[topser].volt-=6250;
					//break;
					//}
				fv_table->table[0].volt=1000000;	// freq=1300	1200000
				fv_table->table[1].volt=893750;		// freq=1200	1093750
				fv_table->table[2].volt=818750;		// freq=1100	1018750
				fv_table->table[3].volt=743750;		// freq=1001	943750
				fv_table->table[4].volt=643750;		// freq=845		843750
				fv_table->table[5].volt=556250;		// freq=676		756250
				fv_table->table[6].volt=518750;		// freq=545		718750
				fv_table->table[7].volt=481250;		// freq=450		681250
				fv_table->table[8].volt=425000;		// freq=343		625000
		}
		table[i] = fv_table->table[i].volt;
}
	for (i = 0; i < num_of_lv; i++)
		printk("dvfs id : %d  %d Khz : %d uv\n", ACPM_VCLK_TYPE | id, fv_table->table[i].rate, table[i]);

	return 0;
}

static void check_percent_margin(struct rate_volt_header *head, unsigned int num_of_lv)
{
	int org_volt;
	int percent_volt;
	int i;

	if (!volt_offset_percent)
		return;

	for (i = 0; i < num_of_lv; i++) {
		org_volt = head->table[i].volt;
		percent_volt = org_volt * volt_offset_percent / 100;
		head->table[i].volt = org_volt + rounddown(percent_volt, STEP_UV);
	}
}

static void fvmap_copy_from_sram(void __iomem *map_base, void __iomem *sram_base)
{
	struct fvmap_header *fvmap_header, *header;
	struct rate_volt_header *old, *new;
	struct clocks *clks;
	struct pll_header *plls;
	struct vclk *vclk;
	struct cmucal_clk *clk_node;
	unsigned int paddr_offset, fvaddr_offset;
	int size, margin;
	int i, j;

	fvmap_header = map_base;
	header = sram_base;

	size = cmucal_get_list_size(ACPM_VCLK_TYPE);

	for (i = 0; i < size; i++) {
		/* load fvmap info */
		fvmap_header[i].dvfs_type = header[i].dvfs_type;
		fvmap_header[i].num_of_lv = header[i].num_of_lv;
		fvmap_header[i].num_of_members = header[i].num_of_members;
		fvmap_header[i].num_of_pll = header[i].num_of_pll;
		fvmap_header[i].num_of_mux = header[i].num_of_mux;
		fvmap_header[i].num_of_div = header[i].num_of_div;
		fvmap_header[i].gearratio = header[i].gearratio;
		fvmap_header[i].init_lv = header[i].init_lv;
		fvmap_header[i].num_of_gate = header[i].num_of_gate;
		fvmap_header[i].reserved[0] = header[i].reserved[0];
		fvmap_header[i].reserved[1] = header[i].reserved[1];
		fvmap_header[i].block_addr[0] = header[i].block_addr[0];
		fvmap_header[i].block_addr[1] = header[i].block_addr[1];
		fvmap_header[i].block_addr[2] = header[i].block_addr[2];
		fvmap_header[i].o_members = header[i].o_members;
		fvmap_header[i].o_ratevolt = header[i].o_ratevolt;
		fvmap_header[i].o_tables = header[i].o_tables;

		vclk = cmucal_get_node(ACPM_VCLK_TYPE | i);
		if (vclk == NULL)
			continue;
		pr_info("dvfs_type : %s - id : %x\n",
			vclk->name, fvmap_header[i].dvfs_type);
		pr_info("  num_of_lv      : %d\n", fvmap_header[i].num_of_lv);
		pr_info("  num_of_members : %d\n", fvmap_header[i].num_of_members);

		old = sram_base + fvmap_header[i].o_ratevolt;
		new = map_base + fvmap_header[i].o_ratevolt;

		check_percent_margin(old, fvmap_header[i].num_of_lv);

		margin = init_margin_table[vclk->margin_id];
		if (margin)
			cal_dfs_set_volt_margin(i | ACPM_VCLK_TYPE, margin);

		for (j = 0; j < fvmap_header[i].num_of_lv; j++) {
			
			new->table[j].rate = old->table[j].rate;
			new->table[j].volt = old->table[j].volt;
			
		}

		for (j = 0; j < fvmap_header[i].num_of_pll; j++) {
			clks = sram_base + fvmap_header[i].o_members;
			plls = sram_base + clks->addr[j];
			clk_node = cmucal_get_node(vclk->list[j]);
			if (clk_node == NULL)
				continue;
			paddr_offset = clk_node->paddr & 0xFFFF;
			fvaddr_offset = plls->addr & 0xFFFF;
			if (paddr_offset == fvaddr_offset)
				continue;

			clk_node->paddr += fvaddr_offset - paddr_offset;
			clk_node->pll_con0 += fvaddr_offset - paddr_offset;
			if (clk_node->pll_con1)
				clk_node->pll_con1 += fvaddr_offset - paddr_offset;
		}
	}
}

int fvmap_init(void __iomem *sram_base)
{
	void __iomem *map_base;

	map_base = kzalloc(FVMAP_SIZE, GFP_KERNEL);

	fvmap_base = map_base;
	sram_fvmap_base = sram_base;
	pr_info("%s:fvmap initialize %pK\n", __func__, sram_base);
	fvmap_copy_from_sram(map_base, sram_base);

	return 0;
}
