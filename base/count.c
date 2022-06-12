#include<stdio.h>

#define PAGE_SIZE 4096
#define PHY_MEM (128 * 1024 * 1024)
#define NR_PDE 1024
#define NR_PTE 1024
#define PT_SIZE ((NR_PTE) * (PAGE_SIZE))
#define make_pde(addr) ((((unsigned)(addr)) & 0xfffff000) | 0x7)//低3位全置1，4-12位置0，其它位不变
#define make_pte(addr) ((((unsigned)(addr)) & 0xfffff000) | 0x7)
int pdir[1024],ptable[32*1024];

int main(){

   unsigned pdir_idx, ptable_idx, pframe_idx;

   
   int *p=ptable;

	pframe_idx = 0;
	for (pdir_idx = 0; pdir_idx < PHY_MEM / PT_SIZE; pdir_idx++)//PHY_MEM / PT_SIZE=32
	{

		pdir[pdir_idx] = make_pde(ptable);
		
		pdir[pdir_idx + 0xc0000000 / PT_SIZE] = make_pde(ptable);
		//内核区也fill，pdir单纯为指针，可以动内核区
		//不对，是在copy的后3/4放一份复制的地址
		//也不是后3/4，只能说是跨过0xc0000000后的内核区的页表映射，3GB的位置，也可以说是虚拟地址空间的3/4
		//是理应映射内核区位置的页表信息改为映射物理内存信息
		
		for (ptable_idx = 0; ptable_idx < NR_PTE; ptable_idx++)
		{
			*p = make_pte(pframe_idx << 12);
			//？   低十二位自动置0，表示每一个物理页的框号，ptable直接取这20位作为page_frame并把低三位置1
			
			pframe_idx++;//下一个物理页框号，最后应该等于32*1024，再乘4KB就是128MB
			p++;//页表下一项
		}
   }
}
 